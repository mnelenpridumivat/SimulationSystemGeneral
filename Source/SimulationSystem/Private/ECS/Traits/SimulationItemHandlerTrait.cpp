// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationItemHandlerTrait.h"

#include "EntityStorageFragment.h"
#include "GraphPositionFragment.h"
#include "MassEntityTemplateRegistry.h"
#include "ProfileGeneratorItem.h"
#include "SimulationArchetypeSettings.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSquadHandlerTrait.h"
#include "SimulationSystemSettings.h"
#include "SimulationSystemSubsystem.h"

void USimulationItemHandlerTrait::SetupDefaultTableSettings_Implementation(FInstancedStruct& data)
{
	data.InitializeAs(FSimulationTraitOverridesItem::StaticStruct());
	auto& TableData = data.GetMutable<FSimulationTraitOverridesItem>();
	TableData.Items = Items;
	TableData.Row = Row;
	TableData.TableValue = TableValue;
	
}

void USimulationItemHandlerTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext,
                                                const UWorld& World) const
{
	BuildContext.AddFragment<FSquadStorageFragment>();
}

void USimulationItemHandlerTrait::SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity,
	const FInstancedStruct& OverrideData)
{
	Super::SetupEntity(Context, Manager, Entity, OverrideData);
	if (!ensure(OverrideData.IsValid()) || !ensure(OverrideData.GetScriptStruct() == FSimulationTraitOverridesItem::StaticStruct()))
	{
		return;
	}
	auto& TableData = OverrideData.Get<FSimulationTraitOverridesItem>();
	if (!ensure(Manager.IsEntityValid(Entity)))
	{
		return;
	}
	const FItemSetData* ItemSetData = nullptr;
	if (TableData.TableValue)
	{
		auto TableSets = USimulationSystemSettings::GetDataTableByKey(SIMULATION_DATATABLE_KEY(ItemSet));
		if (!ensure(TableSets))
		{
			return;
		}
		ItemSetData = TableSets->FindRow<FItemSetData>(TableData.Row.RowName, nullptr);
		if (!ensure(ItemSetData))
		{
			return;
		}
	} else
	{
		ItemSetData = &TableData.Items;
	}
	
	auto Subsystem = USimulationFunctionLibrary::GetSimulationSystemSubsystem(GetWorld());
	auto& StorageFragment = Manager.GetFragmentDataChecked<FItemStorageFragment>(Entity);
	auto& PosFrag = Manager.GetFragmentDataChecked<FGraphPositionFragment>(Entity);
	for (auto& elem : ItemSetData->Items)
	{
		auto Num = ensure(elem.AmountMin <= elem.AmountMax) ? elem.AmountMin : elem.AmountMax;
		for (auto i = Num; i < elem.AmountMax; i++)
		{
			if (FMath::RandRange(0.0, 1.0) < elem.Probability)
			{
				Num++;
			}
		}
		for (auto i = 0; i < Num; i++)
		{
			StorageFragment.Children.Add(Subsystem->SpawnProfile(GetWorld(), elem.Item, PosFrag.Position));
		}
	}
}
