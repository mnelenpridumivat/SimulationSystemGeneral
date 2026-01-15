// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadsDebugDrawProcessor.h"

#include "GlobalGraph.h"
#include "GraphPositionFragment.h"
#include "GraphTargetPositionFragment.h"
#include "MassExecutionContext.h"
#include "SimulationFunctionLibrary.h"

FCriticalSection FSquadsDebugDrawContainer::Lock;
TArray<FVector> FSquadsDebugDrawContainer::GraphPositions;
TArray<FVector> FSquadsDebugDrawContainer::EntityPositions;
TArray<TPair<FVector, FVector>> FSquadsDebugDrawContainer::WayLines;

USquadsDebugDrawProcessor::USquadsDebugDrawProcessor()
{
#if !UE_BUILD_SHIPPING
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (uint8)EProcessorExecutionFlags::All;
#endif
}

void USquadsDebugDrawProcessor::ConfigureQueries()
{
#if !UE_BUILD_SHIPPING
	EntityQuery.AddRequirement<FGraphPositionFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FGraphTargetPositionFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::Optional);
	EntityQuery.RegisterWithProcessor(*this);
#endif
}

void USquadsDebugDrawProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	FScopeLock g(&FSquadsDebugDrawContainer::Lock);
#if !UE_BUILD_SHIPPING
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(Context.GetWorld());
	EntityQuery.ForEachEntityChunk(EntityManager, Context, ([&GlobalGraph, &EntityManager](FMassExecutionContext& Context)
	{
		auto Positions = Context.GetFragmentView<FGraphPositionFragment>();
		FSquadsDebugDrawContainer::GraphPositions.Empty(FSquadsDebugDrawContainer::GraphPositions.Num());
		FSquadsDebugDrawContainer::GraphPositions.Reserve(Positions.Num());
		for (auto& Position : Positions)
		{
			auto Location = GlobalGraph->GetVertexLocationByID(Position.Position);
			FSquadsDebugDrawContainer::GraphPositions.Add(Location);
		}
		auto Targets = Context.GetFragmentView<FGraphTargetPositionFragment>();
		FSquadsDebugDrawContainer::EntityPositions.Empty(FSquadsDebugDrawContainer::EntityPositions.Num());
		FSquadsDebugDrawContainer::EntityPositions.Reserve(Targets.Num());
		FSquadsDebugDrawContainer::WayLines.Empty(FSquadsDebugDrawContainer::WayLines.Num());
		constexpr size_t WayLinesCoeff = 2*50; // 50 pairs
		FSquadsDebugDrawContainer::WayLines.Reserve(Targets.Num()*WayLinesCoeff);
		for (auto& Target : Targets)
		{
			for (int i = 1; i < Target.Way.Num(); ++i)
			{
				auto A = Target.Way[i];
				auto B = Target.Way[i - 1];
				if (!ensure(A.IsValid() && B.IsValid()))
				{
					continue;
				}
				auto APos = GlobalGraph->GetVertexLocationByID(A);
				auto BPos = GlobalGraph->GetVertexLocationByID(B);
				FSquadsDebugDrawContainer::WayLines.Add({APos, BPos});
			}
		}
		for (auto& Target : Targets){
			auto Current = Target.Way.Last();
			if (!ensure(Current.IsValid()) || !ensure(Target.TargetPosition.IsValid()))
			{
				continue;
			}
			if (Current == Target.TargetPosition)
			{
				continue;
			}
			auto Next = Target.Way.Last(1);
			if (!ensure(Next.IsValid()))
			{
				continue;
			}
			auto CurrentPos = GlobalGraph->GetVertexLocationByID(Current);
			auto NextPos = GlobalGraph->GetVertexLocationByID(Target.Way.Last(1));
			auto MovedDist = Target.MovedDistance;
			auto InterpPos = FMath::Lerp(CurrentPos, NextPos, MovedDist/(NextPos-CurrentPos).Length());
			FSquadsDebugDrawContainer::GraphPositions.Add(InterpPos);
		}
	}));
#endif
}
