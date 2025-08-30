// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\SimulationFunctionLibrary.h"

#include "EngineUtils.h"
#include "GlobalGraph.h"
#include "SimulationSystemSettings.h"
#include "NativePriorityQueue.h"
#include "ProfileIDController.h"
#include "SimulationSystemFunctionsImplementation.h"
#include "SimulationSystemSubsystem.h"
#include "Vertex.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Vertex.h"
#include "NavHeuristics/NavHeuristic_Base.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

AGlobalGraph* USimulationFunctionLibrary::GetGlobalGraph(UObject* Context)
{
	static AGlobalGraph* GraphPtr = nullptr; // TODO: remove this fix and fix normally!
	TArray<AActor*> Graphs;
	UGameplayStatics::GetAllActorsOfClass(Context, AGlobalGraph::StaticClass(), Graphs);
	if(!Graphs.Num() && GraphPtr){
		return GraphPtr;
	}
	ensureMsgf(Graphs.Num(), TEXT("Unable to find any global graphs!"));
	ensureMsgf(!(Graphs.Num()-1), TEXT("There is more that one global graphs!"));
	GraphPtr = Cast<AGlobalGraph>(Graphs[0]);
	return GraphPtr;
}

UProfileIDController* USimulationFunctionLibrary::GetProfileIDController(UObject* Context)
{
#if WITH_EDITOR
	const auto Graph = GetGlobalGraph(Context);
	if(!IsValid(Graph))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to access global graph actor in world [%s]"),
		       *(Context ? (Context->GetWorld() ? Context->GetWorld()->GetName() : FString("null")) : FString(
			       "null (context)")));
		return nullptr;
	}
	return Graph->GetProfileIDsController();
#else
	return GetGlobalGraph(Context)->GetProfileIDsController();
#endif
}

void USimulationFunctionLibrary::AsProfile(const FSimProfileHolder& Holder, USimProfileBase*& Profile, bool& Success)
{
	Success = Holder.Holder.GetCurrentSubtypeIndex() == 1;
	if(Success)
	{
		Profile = Holder.Holder.GetSubtype<USimProfileBase*>();
	}
}

void USimulationFunctionLibrary::AsVertex(const FSimProfileHolder& Holder, FSimVertexID& VertexID, bool& Success)
{
	Success = Holder.Holder.GetCurrentSubtypeIndex() == 0;
	if(Success)
	{
		VertexID = Holder.Holder.GetSubtype<TWeakPtr<Simulation::Vertex>>().Pin()->GetVertexID();
	}
}

FSimVertexID USimulationFunctionLibrary::GetWayStart(const FGraphWay& Way)
{
	if(!Way.IsValid())
	{
		return FSimVertexID::Invalid;
	}
	return Way.GetStart().Pin()->GetVertexID();
}

FSimVertexID USimulationFunctionLibrary::GetWayEnd(const FGraphWay& Way)
{
	if(!Way.IsValid())
	{
		return FSimVertexID::Invalid;
	}
	return Way.GetEnd().Pin()->GetVertexID();
}

FSimVertexID USimulationFunctionLibrary::GetWayCurrentGraphPoint(const FGraphWay& Way)
{
	if(!Way.IsValid())
	{
		return FSimVertexID::Invalid;
	}
	return Way.GetCurrent().Pin()->GetVertexID();
}

FSimVertexID USimulationFunctionLibrary::GetWayNextGraphPoint(const FGraphWay& Way)
{
	if(!Way.IsValid() || Way.GetCurrent()==Way.GetEnd())
	{
		return FSimVertexID::Invalid;
	}
	return Way.GetNext().Pin()->GetVertexID();
}

FGraphWay USimulationFunctionLibrary::FindWay(UObject* Context, const FSimVertexID& Start, const FSimVertexID& Dest,
	TSet<UNavHeuristic_Base*> Params)
{
	if(!Params.Num())
	{
		return FGraphWay();
	}
	UWorld* World = Context->GetWorld();
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(World);
	auto StartVertex = GlobalGraph->GetVertexByID(Start);
	auto EndVertex = GlobalGraph->GetVertexByID(Dest);
	if(FVector::Dist(StartVertex.Pin()->GetLocation(), EndVertex.Pin()->GetLocation()) >= GetMutableDefault<USimulationSystemSettings>()->FindWayMaxSearchDistance)
	{
		//UE_LOG(LogTemp, Log, TEXT("USimulationFunctionLibrary::Find way: vertex [%s] locates too far from vertex [%s]"), )
		return FGraphWay();
	}
	auto GetScore = [&](const TWeakPtr<Simulation::Vertex>& Current, const TWeakPtr<Simulation::Vertex>& Next)
	{
		double Score = 0;
		for(auto& Param : Params)
		{
			Score = Param->NativeEvaluate(Score, StartVertex, Current, Next, EndVertex);
		}
		return Score;
	};
	TMap<TWeakPtr<Simulation::Vertex>, TPair<TWeakPtr<Simulation::Vertex>, double>> Tree;
	TNativePriorityQueue<TWeakPtr<Simulation::Vertex>> Priority;
	Tree.Add(StartVertex, TPair<TWeakPtr<Simulation::Vertex>, double>{nullptr, 0});
	Priority.Push(StartVertex, 0);
	while(!Priority.IsEmpty())
	{
		auto Current = Priority.Pop();
		if(Current == EndVertex)
		{
			break;
		}
		for(auto& Edge : Current.Pin()->GetEdges())
		{
			auto Next = Edge.Pin()->GetOpposite(Current);
			if(FVector::Dist(StartVertex.Pin()->GetLocation(), Next.Pin()->GetLocation()) >= GetMutableDefault<USimulationSystemSettings>()->FindWayMaxSearchDistance)
			{
				continue;
			}
			double NewScore = Tree[Current].Value + FVector::Dist(Current.Pin()->GetLocation(), Next.Pin()->GetLocation());
			if(!Tree.Contains(Next) || NewScore < Tree[Next].Value)
			{
				double PriorityScore = NewScore + GetScore(Current, Next);
				Priority.Push(Next, PriorityScore);
				Tree.Add(Next, TPair<TWeakPtr<Simulation::Vertex>, double>{Current, NewScore});
			}
		}
	}
	if(!Tree.Contains(EndVertex))
	{
		return FGraphWay();
	}
	FGraphWay NewWay;
	TArray<TWeakPtr<Simulation::Vertex>> Stack;
	Stack.Push(EndVertex);
	auto Node = Tree[EndVertex];
	while(Node.Key.IsValid())
	{
		Stack.Push(Node.Key);
		Node = Tree[Node.Key];
	}
	while (Stack.Num())
	{
		NewWay.GetWay().Push(Stack.Pop());
	}
	return NewWay;
}

USimulationSystemSubsystem* USimulationFunctionLibrary::GetSimulationSystemSubsystem(UObject* Context)
{
	auto GI = UGameplayStatics::GetGameInstance(Context);
	return GI->GetSubsystem<USimulationSystemSubsystem>();
}

USimulationSystemFunctionsImplementation* USimulationFunctionLibrary::GetFunctions(UObject* Context)
{
	auto Functions = GetSimulationSystemSubsystem(Context)->GetSimulationSystemFunctions();
	if (ensureMsgf(IsValid(Functions), TEXT("Unable to get SimulationSystemFunctions from subsystem!")))
	{
		return Functions;
	}
	return nullptr;
}

void USimulationFunctionLibrary::SaveObjectData(UObject* Object, TArray<uint8>& Data)
{
	FMemoryWriter MemoryWriter(Data, true);
	FObjectAndNameAsStringProxyArchive SaveArchive(MemoryWriter, false);
	Object->Serialize(SaveArchive);
}

void USimulationFunctionLibrary::LoadObjectData(UObject* Object, const TArray<uint8>& Data)
{
	FMemoryReader MemoryReader(Data, true);
	FObjectAndNameAsStringProxyArchive SaveArchive(MemoryReader, false);
	Object->Serialize(SaveArchive);
}
