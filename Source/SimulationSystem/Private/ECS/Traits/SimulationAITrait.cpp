// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationAITrait.h"

#include "ActionPlanner.h"
#include "ActionPlannerOwner.h"
#include "AIFragment.h"
#include "MassEntityTemplateRegistry.h"

void USimulationAITrait::SetupDefaultTableSettings_Implementation(FInstancedStruct& data)
{
	data.InitializeAs(FSimulationTraitOverridesAI::StaticStruct());
	auto& Overrides = data.GetMutable<FSimulationTraitOverridesAI>();
	Overrides.AIPlanner = DefaultAIPlanner;
}

void USimulationAITrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FAIFragment>();
}

void USimulationAITrait::SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity,
	const FInstancedStruct& OverrideData)
{
	if(!ensure(OverrideData.IsValid()) || !ensure(OverrideData.GetScriptStruct() == FSimulationTraitOverridesAI::StaticStruct()))
	{
		return;
	}
	auto& Data = OverrideData.Get<FSimulationTraitOverridesAI>();
	auto& AIFragment = Manager.GetFragmentDataChecked<FAIFragment>(Entity);
	AIFragment.ActionPlanner = NewObject<UActionPlanner>(GetWorld(), Data.AIPlanner);
	auto OwnerHandle = NewObject<UActionPlannerOwner>(GetWorld());
	OwnerHandle->SetEntityOwner(Entity);
	AIFragment.ActionPlanner->SetParentObject(OwnerHandle);
}
