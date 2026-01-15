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

USimulationSquadHandlerTrait::USimulationSquadHandlerTrait()
{
	bCanBeRandom = false;
	DefaultsDataTable = USimulationSystemSettings::GetDataTableByKey(SIMULATION_DATATABLE_KEY(Squad));
}

void USimulationSquadHandlerTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext,
	const UWorld& World) const
{
	BuildContext.AddFragment<FSquadStorageFragment>();
}

void USimulationSquadHandlerTrait::SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity,
	const FSimulationTraitOverrides& OverrideData)
{
	Super::SetupEntity(Context, Manager, Entity, OverrideData);
	if (!ensure(Manager.IsEntityValid(Entity)))
	{
		return;
	}
	if (!DefaultsDataTable)
	{
		DefaultsDataTable = USimulationSystemSettings::GetDataTableByKey(SIMULATION_DATATABLE_KEY(Squad));
		if (!ensure(DefaultsDataTable))
		{
			return;
		}
	}
	auto RowData = DefaultsDataTable->FindRow<FSquadData>(OverrideData.RowName, nullptr);
	if (!ensure(RowData))
	{
		return;
	}
	TArray<FSimulationArchetypeHandle> NPCs;
	if (RowData->Random)
	{
		auto Num = FMath::RandRange(RowData->Min, RowData->Max);
		for (int i = 0; i < Num; i++)
		{
			const auto& TableNPCs = RowData->NPCs;
			NPCs.Add(TableNPCs[FMath::RandRange(0, TableNPCs.Num()-1)]);
		}
	}
	else
	{
		NPCs = RowData->NPCs;
	}
	auto& StorageFragment = Manager.GetFragmentDataChecked<FSquadStorageFragment>(Entity);
	auto& PosFrag = Manager.GetFragmentDataChecked<FGraphPositionFragment>(Entity);
	for (auto& NPC : NPCs)
	{
		StorageFragment.Children.Add(
			USimulationFunctionLibrary::GetSimulationSystemSubsystem(Context)->SpawnProfile(Context, NPC, PosFrag.Position));
	}
}
