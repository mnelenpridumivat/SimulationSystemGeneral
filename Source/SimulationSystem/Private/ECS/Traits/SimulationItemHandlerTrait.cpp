// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationItemHandlerTrait.h"

#include "EntityStorageFragment.h"
#include "MassEntityTemplateRegistry.h"
#include "ProfileGeneratorItem.h"
#include "SimulationArchetypeSettings.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSquadHandlerTrait.h"
#include "SimulationSystemSettings.h"
#include "SimulationSystemSubsystem.h"

USimulationItemHandlerTrait::USimulationItemHandlerTrait()
{
	bCanBeRandom = true;
	DefaultsDataTable = USimulationSystemSettings::GetDataTableByKey(SIMULATION_DATATABLE_KEY(Item));
}

void USimulationItemHandlerTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext,
	const UWorld& World) const
{
	BuildContext.AddFragment<FSquadStorageFragment>();
}

void USimulationItemHandlerTrait::SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity,
	const FSimulationTraitOverrides& OverrideData)
{
	Super::SetupEntity(Context, Manager, Entity, OverrideData);
	if (!ensure(Manager.IsEntityValid(Entity)))
	{
		return;
	}
	if (!DefaultsDataTable)
	{
		DefaultsDataTable = USimulationSystemSettings::GetDataTableByKey(SIMULATION_DATATABLE_KEY(Item));
		if (!ensure(DefaultsDataTable))
		{
			return;
		}
	}
	auto RowData = DefaultsDataTable->FindRow<FItemSetData>(OverrideData.RowName, nullptr);
	if (!ensure(RowData))
	{
		return;
	}
	auto& StorageFragment = Manager.GetFragmentDataChecked<FItemStorageFragment>(Entity);
	for (auto& Item : RowData->Items)
	{
		StorageFragment.Children.Add(
			USimulationFunctionLibrary::GetSimulationSystemSubsystem(GetWorld())->SpawnProfile(GetWorld(), Item));
	}
}
