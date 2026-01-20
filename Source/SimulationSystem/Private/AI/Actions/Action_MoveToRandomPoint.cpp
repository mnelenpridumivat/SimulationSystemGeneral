// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Action_MoveToRandomPoint.h"

#include "ActionPlannerOwner.h"
#include "GlobalGraph.h"
#include "MassEntitySubsystem.h"
#include "NavHeuristic_DistToDest.h"
#include "SimulationFragmentInitSubsystem.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSubsystem.h"
#include "Base/GraphPositionFragment.h"
#include "Base/GraphTargetPositionFragment.h"

void UAction_MoveToRandomPoint::NativeStartAction(FMassCommandBuffer& CmdBuffer)
{
	Super::NativeStartAction(CmdBuffer);

	auto OwnerObj = GetOwningObject();
	if (!ensure(OwnerObj->GetProfileType() == ESimualtionSystemProfileType::ECS))
	{
		return;
	}

	auto Entity = OwnerObj->GetEntityOwner();

	GEngine->GetEngineSubsystem<USimulationFragmentInitSubsystem>()->
		VerifiedBindDefferedInit<FGraphTargetPositionFragment>(Entity, [this](FMassEntityHandle Entity)
		{
			auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	
			auto Subsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
			auto& Manager = Subsystem->GetEntityManager();
			auto View = Manager.GetFragmentDataChecked<FGraphPositionFragment>(Entity);
			auto StartPos = View.Position;
		
			using VertType = TWeakPtr<Simulation::Vertex>;
			VertType StartVert = GlobalGraph->GetVertexByID(StartPos);
		
			TSet<VertType> Candidates = {StartVert};
			TQueue<VertType> ToCheck;
			ToCheck.Enqueue(StartVert);
		
			while (!ToCheck.IsEmpty())
			{
				VertType Vert;
				ToCheck.Dequeue(Vert);
				if (!ensure(Vert.IsValid()))
				{
					continue;
				}
				
				auto Pinned = Vert.Pin();
				auto Edges = Vert.Pin()->GetEdges();
				for (auto Edge : Edges)
				{
					auto NewCandidate = Edge.Pin()->GetOpposite(Vert);
					if (Candidates.Contains(NewCandidate))
					{
						continue;
					}
					ToCheck.Enqueue(NewCandidate);
					Candidates.Add(NewCandidate);
				}
			}
		
			Candidates.Remove(StartVert);
			if (!ensure(Candidates.IsEmpty()))
			{
				AbortAction();
				return;
			}

			auto RandomPoint = Candidates.Array()[FMath::RandRange(0, Candidates.Num() - 1)];
			auto& Fragment = Manager.GetFragmentDataChecked<FGraphTargetPositionFragment>(Entity);
			Fragment.Way = USimulationFunctionLibrary::FindWay(GetWorld(), StartPos, RandomPoint.Pin()->GetVertexID(), {NewObject<UNavHeuristic_DistToDest>()}).GetWay();
			
		});
	
}
