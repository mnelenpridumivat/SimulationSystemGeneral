// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionPlannerOwner.h"
#include "GraphSerialized.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Vertex.h"
#include "Edge.h"
#include "GraphAsset.h"
#include "SimVertexID.h"
#include "SimulationSystemEditorFunctionLibrary.generated.h"

class UActionPlannerOwner;
struct FMassFragment;
class AGraphPointBase;
class AGraphPointLink;
class AGraphAsset;

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMEDITOR_API USimulationSystemEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static void RebuildSelectedLocalGraph(UWorld* World,
		AGraphAsset* LocalGraph,
		const TMap<AGraphPointLink*,
		TSharedPtr<Simulation::Vertex>>& Links,
		FGraphSerialized& ReturnGraph,
		FProfilesSerialized& ReturnProfiles);
	static void RebuildLocalGraphSpawn(UWorld* World, AGraphAsset* LocalGraph, FProfilesSerialized& ReturnProfiles);
	static FName GetLayerName(UObject* Obj);
	static bool IsUniqueTriple(
		const TArray<TTuple<TWeakPtr<Simulation::Edge>, TWeakPtr<Simulation::Edge>, TWeakPtr<Simulation::Edge>>>& Trilpes,
		TTuple<TWeakPtr<Simulation::Edge>, TWeakPtr<Simulation::Edge>, TWeakPtr<Simulation::Edge>> NewTuple
		);
	static TWeakPtr<Simulation::Vertex> GetClosestVertex(FVector Location, const TArray<FGraphLayer>& GraphPart, int LayerIndex);
	static TWeakPtr<Simulation::Vertex> GetClosestVertex(FVector Location, const FGraphLayer& GraphLayer);
	
	static void SaveProfile(USimProfileBase* Profile, FProfilesSerialized& ReturnProfiles, FSimVertexID VertexID = FSimVertexID::Invalid);

	static void CleanUp();
	static void Build();

public:
	
	static void FullRebuild();
	static void SpawnRebuild();
	static void DebugDrawGraphEditor();

	static void Delay(TFunction<void()> InLambda, float InMinimumSeconds, int32 InMinimumFrames);
	
};
