// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationNPCHandlerTrait.h"

#include "EntityStorageFragment.h"
#include "GraphPositionFragment.h"
#include "MassEntityTemplateRegistry.h"
#include "ProfileGeneratorSquad.h"
#include "SimulationArchetypeSettings.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSettings.h"
#include "SimulationSystemSubsystem.h"

void USimulationNPCHandlerTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext,
	const UWorld& World) const
{
	BuildContext.AddFragment<FNPCsStorageFragment>();
}

void USimulationNPCHandlerTrait::SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity,
	const FInstancedStruct& OverrideData)
{
	Super::SetupEntity(Context, Manager, Entity, OverrideData);
	if (!ensure(OverrideData.IsValid()) || !ensure(OverrideData.GetScriptStruct() == FSimulationTraitOverridesNPC::StaticStruct()))
	{
		return;
	}
	auto& TableData = OverrideData.Get<FSimulationTraitOverridesNPC>();
	if (!ensure(Manager.IsEntityValid(Entity)))
	{
		return;
	}
	auto& StorageFragment = Manager.GetFragmentDataChecked<FNPCsStorageFragment>(Entity);
	auto& PosFrag = Manager.GetFragmentDataChecked<FGraphPositionFragment>(Entity);
	TArray<FSimulationArchetypeHandle> NPCToSpawn;
	if(TableData.Random)
	{
		FRandomStream Stream(FPlatformTime::Seconds());
		int Num = Stream.RandRange(TableData.Min, TableData.Max);
		NPCToSpawn.Reserve(Num);
		for(int i = 0; i < Num; ++i)
		{
			NPCToSpawn.Add(TableData.NPCs[Stream.RandRange(0, TableData.NPCs.Num()-1)]);
		}
	}
	else
	{
		NPCToSpawn = TableData.NPCs;
	}
	for (auto& Squad : NPCToSpawn)
	{
		StorageFragment.Children.Add(
		USimulationFunctionLibrary::GetSimulationSystemSubsystem(Context)->SpawnProfile(Context, {Squad.Name}, PosFrag.Position));
	}
}

void USimulationNPCHandlerTrait::SetupDefaultTableSettings_Implementation(FInstancedStruct& data)
{
	data.InitializeAs(FSimulationTraitOverridesNPC::StaticStruct());
	auto& TableData = data.GetMutable<FSimulationTraitOverridesNPC>();
	TableData.NPCs = DefaultNPCs;
	TableData.Random = Random;
	TableData.Min = Min;
	TableData.Max = Max;
}
