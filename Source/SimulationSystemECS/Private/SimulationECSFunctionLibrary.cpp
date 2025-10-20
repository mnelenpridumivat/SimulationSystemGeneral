// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationECSFunctionLibrary.h"

#include "EntityStorageFragment.h"
#include "GraphPositionFragment.h"
#include "MassEntitySubsystem.h"
#include "Tags/ItemTag.h"
#include "Tags/NPCTag.h"
#include "Tags/SmartTag.h"
#include "Tags/SquadTag.h"

FMassEntityManager* USimulationECSFunctionLibrary::GetEntityManager(UObject* Context)
{
	if (!ensure(IsValid(Context)))
	{
		return nullptr;
	}
	auto World = Context->GetWorld();
	if (!ensure(IsValid(World)))
	{
		return nullptr;
	}
	auto Subsystem = World->GetSubsystem<UMassEntitySubsystem>();
	if (!ensure(IsValid(Subsystem)))
	{
		return nullptr;
	}
	return &Subsystem->GetMutableEntityManager();
}

FMassEntityHandle USimulationECSFunctionLibrary::CreateSquadArchetype(UObject* Context)
{
	auto Manager = GetEntityManager(Context);
	if (!ensure(Manager))
	{
		return FMassEntityHandle();
	}
	static FMassArchetypeHandle ArchetypeHandle;
	if (!ArchetypeHandle.IsValid())
	{
		TArray<const UScriptStruct*> Fragments;
		// Fragments
		Fragments.Add(FGraphPositionFragment::StaticStruct());
		Fragments.Add(FEntityStorageFragment::StaticStruct());
		// Tags
		Fragments.Add(FSquadTag::StaticStruct());

		ArchetypeHandle = Manager->CreateArchetype(Fragments);
	}
	auto Entity = Manager->CreateEntity(ArchetypeHandle);

	return Entity;
}

FMassEntityHandle USimulationECSFunctionLibrary::CreateNPCArchetype(UObject* Context)
{
	auto Manager = GetEntityManager(Context);
	if (!ensure(Manager))
	{
		return FMassEntityHandle();
	}
	static FMassArchetypeHandle ArchetypeHandle;
	if (!ArchetypeHandle.IsValid())
	{
		TArray<const UScriptStruct*> Fragments;
		// Fragments
		Fragments.Add(FGraphPositionFragment::StaticStruct());
		Fragments.Add(FEntityStorageFragment::StaticStruct());
		// Tags
		Fragments.Add(FNPCTag::StaticStruct());

		ArchetypeHandle = Manager->CreateArchetype(Fragments);
	}
	auto Entity = Manager->CreateEntity(ArchetypeHandle);

	return Entity;
}

FMassEntityHandle USimulationECSFunctionLibrary::CreateSmartArchetype(UObject* Context)
{
	auto Manager = GetEntityManager(Context);
	if (!ensure(Manager))
	{
		return FMassEntityHandle();
	}
	static FMassArchetypeHandle ArchetypeHandle;
	if (!ArchetypeHandle.IsValid())
	{
		TArray<const UScriptStruct*> Fragments;
		// Fragments
		Fragments.Add(FGraphPositionFragment::StaticStruct());
		Fragments.Add(FEntityStorageFragment::StaticStruct());
		// Tags
		Fragments.Add(FSmartTag::StaticStruct());

		ArchetypeHandle = Manager->CreateArchetype(Fragments);
	}
	auto Entity = Manager->CreateEntity(ArchetypeHandle);

	return Entity;
}

FMassEntityHandle USimulationECSFunctionLibrary::CreateItemArchetype(UObject* Context)
{
	auto Manager = GetEntityManager(Context);
	if (!ensure(Manager))
	{
		return FMassEntityHandle();
	}
	static FMassArchetypeHandle ArchetypeHandle;
	if (!ArchetypeHandle.IsValid())
	{
		TArray<const UScriptStruct*> Fragments;
		// Fragments
		Fragments.Add(FGraphPositionFragment::StaticStruct());
		// Tags
		Fragments.Add(FItemTag::StaticStruct());

		ArchetypeHandle = Manager->CreateArchetype(Fragments);
	}
	auto Entity = Manager->CreateEntity(ArchetypeHandle);

	return Entity;
}
