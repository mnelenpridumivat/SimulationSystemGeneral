// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationSquadHandlerTrait.h"

#include "EntityStorageFragment.h"
#include "GraphPositionFragment.h"
#include "MassEntityTemplateRegistry.h"
#include "ProfileGeneratorSquad.h"
#include "SimulationArchetypeSettings.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSettings.h"
#include "SimulationSystemSubsystem.h"

void USimulationSquadHandlerTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext,
	const UWorld& World) const
{
	BuildContext.AddFragment<FSquadStorageFragment>();
}

void USimulationSquadHandlerTrait::SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity,
	const FInstancedStruct& OverrideData)
{
	Super::SetupEntity(Context, Manager, Entity, OverrideData);
	if (!ensure(OverrideData.IsValid()) || !ensure(OverrideData.GetScriptStruct() == FSimulationTraitOverridesSquad::StaticStruct()))
	{
		return;
	}
	auto& TableData = OverrideData.Get<FSimulationTraitOverridesSquad>();
	if (!ensure(Manager.IsEntityValid(Entity)))
	{
		return;
	}
	auto DataTable = USimulationSystemSettings::GetDataTableByKey(SIMULATION_DATATABLE_KEY(Squad));
	if (!ensure(DataTable))
	{
		return;
	}
	auto& StorageFragment = Manager.GetFragmentDataChecked<FSquadStorageFragment>(Entity);
	auto& PosFrag = Manager.GetFragmentDataChecked<FGraphPositionFragment>(Entity);
	for (auto& Squad : TableData.Squads)
	{
		StorageFragment.Children.Add(
		USimulationFunctionLibrary::GetSimulationSystemSubsystem(Context)->SpawnProfile(Context, Squad, PosFrag.Position));
	}
}

void USimulationSquadHandlerTrait::SetupDefaultTableSettings_Implementation(FInstancedStruct& data)
{
	data.InitializeAs(FSimulationTraitOverridesSquad::StaticStruct());
	auto& TableData = data.GetMutable<FSimulationTraitOverridesSquad>();
	TableData.Squads = DefaultSquads;
}
