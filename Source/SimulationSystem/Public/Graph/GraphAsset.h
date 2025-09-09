// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Edge.h"
//#include "NameSelectorComponent.h"
//#include "ObjectSerialization.h"
#include "SimEdgeStruct.h"
#include "SimVertexID.h"
#include "GameFramework/Actor.h"
#include "Vertex.h"
#include "GraphSerialized.h"
#include "Components/BoxComponent.h"
#include "GraphAsset.generated.h"


UCLASS()
class SIMULATIONSYSTEM_API AGraphAsset : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraphAsset();

	TWeakPtr<Simulation::Vertex> GetVertex(const FSimVertexID& ID);

	void LoadGraph();
	void LoadObjects_Initial();
	void LoadObjects_Save(); //TODO: Accept SaveObject
	FGraphSerialized SaveObjects();
	void UnloadGraph();

	UFUNCTION(BlueprintPure)
	int GetChunkIndex();
	void SetChunkIndex(int NewChunkIndex);

	UFUNCTION(BlueprintPure)
	FVector GetZoneSize();

	UFUNCTION(BlueprintPure)
	FName GetCurrentLevel();
	
#if WITH_EDITOR
	void SetGraph(const FGraphSerialized& GraphSerialized){Graph = GraphSerialized;}
	void SetInitialProfiles(const FProfilesSerialized& ProfilesSerialized){InitialProfiles = ProfilesSerialized;}
	FProfilesSerialized& GetMutableInitialProfiles(){return InitialProfiles;}
#endif

	UFUNCTION(BlueprintPure)
	int GetLayersNum(){return ChunkGraphs.Num();}
	
	FVector GetVertexLocationByID(const FSimVertexID& ID);
	FSimVertexID FindClosestVertex(const FVector& Location, int LayerIndex);
	void DrawGraph(FColor Color, float LifeTime, float Thickness);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY()
	//UNameSelectorComponent* CurrentChunkSelector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* SizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LevelName;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	UPROPERTY()
	FGraphSerialized Graph;

	UPROPERTY()
	FProfilesSerialized InitialProfiles;
	
	USimProfileBase* LoadProfile(FSerializedProfile& Data);

	UPROPERTY()
	int ChunkIndex;

	using LevelGraph = TPair<TArray<TSharedPtr<Simulation::Vertex>>, TArray<TSharedPtr<Simulation::Edge>>>;

	TArray<LevelGraph> ChunkGraphs;
	
};
