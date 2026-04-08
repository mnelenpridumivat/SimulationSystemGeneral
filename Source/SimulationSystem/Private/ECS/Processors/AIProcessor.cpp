// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProcessor.h"

#include "ActionPlanner.h"
#include "AIFragment.h"
#include "AIInfoFragment.h"
#include "MassExecutionContext.h"

UAIProcessor::UAIProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (uint8)EProcessorExecutionFlags::Server;
}

void UAIProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FAIFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FAIInfoFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.RegisterWithProcessor(*this);
}

void UAIProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	FMassCommandBuffer& CommandBuffer = Context.Defer();
	EntityQuery.ForEachEntityChunk(EntityManager, Context, ([&CommandBuffer](FMassExecutionContext& Context)
	{
		const auto AIFragments = Context.GetMutableFragmentView<FAIFragment>();
		const auto AIInfoFragments = Context.GetMutableFragmentView<FAIInfoFragment>();
		for (int i = 0; i < Context.GetNumEntities(); ++i)
		{
			auto& Fragment = AIFragments[i];
			auto& InfoFragment = AIInfoFragments[i];
			auto Planner = Fragment.ActionPlanner;
			if (ensure(IsValid(Planner)))
			{
				Planner->MoveKeys(InfoFragment.Facts);
			}
		}
	}));
}
