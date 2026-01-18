// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProcessor.h"

#include "ActionPlanner.h"
#include "AIFragment.h"
#include "MassExecutionContext.h"

UAIProcessor::UAIProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (uint8)EProcessorExecutionFlags::Server;
}

void UAIProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FAIFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.RegisterWithProcessor(*this);
}

void UAIProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, ([&EntityManager](FMassExecutionContext& Context)
	{
		const auto AIFragments = Context.GetMutableFragmentView<FAIFragment>();
		for (auto& Fragment : AIFragments)
		{
			auto Planner = Fragment.ActionPlanner;
			if (ensure(IsValid(Planner)))
			{
				Planner->Execute();
			}
		}
	}));
}
