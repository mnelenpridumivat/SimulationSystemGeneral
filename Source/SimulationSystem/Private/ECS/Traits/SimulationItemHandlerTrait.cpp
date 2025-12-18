// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationItemHandlerTrait.h"

#include "EntityStorageFragment.h"
#include "MassEntityTemplateRegistry.h"
#include "SimulationArchetypeSettings.h"
#include "SimulationSquadHandlerTrait.h"
#include "SimulationSystemSettings.h"

USimulationItemHandlerTrait::USimulationItemHandlerTrait()
{
	bCanBeRandom = true;
	DefaultsDataTable = USimulationSystemSettings::GetDataTableByKey(SIMULATION_DATATABLE_KEY(Item));
}

void USimulationItemHandlerTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext,
	const UWorld& World) const
{
	BuildContext.AddFragment<FEntityStorageFragment>();
}

void USimulationItemHandlerTrait::SetupEntity(FMassEntityManager& Manager, FMassEntityHandle Entity,
	const FSimulationTraitOverrides& OverrideData)
{
	Super::SetupEntity(Manager, Entity, OverrideData);
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
}
