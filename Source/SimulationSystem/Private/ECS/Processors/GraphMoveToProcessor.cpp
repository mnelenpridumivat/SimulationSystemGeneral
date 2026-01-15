// Fill out your copyright notice in the Description page of Project Settings.


#include "Processors/GraphMoveToProcessor.h"

#include "GlobalGraph.h"
#include "GraphOfflineSpeedFragment.h"
#include "GraphPositionFragment.h"
#include "GraphTargetPositionFragment.h"
#include "MassExecutionContext.h"
#include "SimulationFunctionLibrary.h"

UGraphMoveToProcessor::UGraphMoveToProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (uint8)EProcessorExecutionFlags::All;
	
}

void UGraphMoveToProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FGraphPositionFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FGraphTargetPositionFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FGraphOfflineSpeedFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.RegisterWithProcessor(*this);
}

void UGraphMoveToProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ParallelForEachEntityChunk(EntityManager, Context, ([this, &EntityManager](FMassExecutionContext& Context)
	{
		const auto Positions = Context.GetMutableFragmentView<FGraphPositionFragment>();
		const auto Ways = Context.GetMutableFragmentView<FGraphTargetPositionFragment>();
		const auto Velocities = Context.GetFragmentView<FGraphOfflineSpeedFragment>();
		const auto Entities = Context.GetEntities();
		const float WorldDeltaTime = Context.GetDeltaTimeSeconds();

		for (int32 i = 0; i < Context.GetNumEntities(); ++i)
		{
			auto& Position = Positions[i];
			auto& Way = Ways[i];
			const auto& Velocity = Velocities[i];
			const auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(Context.GetWorld());

			auto CurrentPos = GlobalGraph->GetVertexLocationByID(Way.Way.Last());
			auto NextPos = GlobalGraph->GetVertexLocationByID(Way.Way.Last(1));
			auto MovedDist = Velocity.Speed*WorldDeltaTime + Way.MovedDistance;
			auto LeftDist = (NextPos-CurrentPos).Length() - MovedDist;
			if (LeftDist <= 0)
			{
				Way.MovedDistance = FMath::Abs(LeftDist);
				Way.Way.Pop();
				Position.Position = Way.Way.Last();
				if (Position.Position == Way.TargetPosition)
				{
					EntityManager.Defer().RemoveFragment<FGraphTargetPositionFragment>(Entities[i]);
				}
			} else
			{
				Way.MovedDistance = MovedDist;
			}
		}
	}
	));
}
