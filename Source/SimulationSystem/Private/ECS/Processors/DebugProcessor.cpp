// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugProcessor.h"

#include "DebugFragment.h"
#include "MassExecutionContext.h"

FCriticalSection FEntitiesForDebugContainer::EntitiesCS;
TArray<FMassEntityHandle> FEntitiesForDebugContainer::Entities = {};

UDebugProcessor::UDebugProcessor()
{
#if !UE_BUILD_SHIPPING
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (uint8)EProcessorExecutionFlags::All;
#endif
}

void UDebugProcessor::ConfigureQueries()
{
#if !UE_BUILD_SHIPPING
	EntityQuery.AddRequirement<FDebugFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.RegisterWithProcessor(*this);
#endif
}

void UDebugProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
#if !UE_BUILD_SHIPPING
	FScopeLock g(&FEntitiesForDebugContainer::EntitiesCS);
	FEntitiesForDebugContainer::Entities.Empty(FEntitiesForDebugContainer::Entities.Num());
	FEntitiesForDebugContainer::Entities.Reserve(EntityQuery.GetNumMatchingEntities(EntityManager));
	EntityQuery.ForEachEntityChunk(EntityManager, Context, ([&](FMassExecutionContext& LocContext)
	{
		FEntitiesForDebugContainer::Entities.Append(LocContext.GetEntities());
	}));

	for (auto& Entity : FEntitiesForDebugContainer::Entities)
	{
		if (ensure(EntityManager.IsEntityValid(Entity)))
		{
			auto Ptr = EntityManager.GetFragmentDataPtr<FDebugFragment>(Entity);
			ensure(!Ptr->Archetype.IsNull());	
		}
	}
#endif
}
