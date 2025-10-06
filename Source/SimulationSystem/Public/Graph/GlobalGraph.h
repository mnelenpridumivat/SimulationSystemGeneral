// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AISimProfileSquad.h"
#include "GameFramework/Actor.h"
#include "Edge.h"
#include "DEPRECATED_ReplicatedSimInfo.h"
#include "SimProfileID.h"
#include "SimVertexID.h"
#include "Profiles/SimProfileHolder.h"
#include "GlobalGraph.generated.h"


class UCommunityRelationTable;
class USimulationState;
class UProfileIDController;
struct FSimProfileID;
class USimProfileBase;
class AGraphAsset;

UCLASS()
class SIMULATIONSYSTEM_API AGlobalGraph : public AActor
{
public:
	virtual void PostInitializeComponents() override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	AGlobalGraph();
	~AGlobalGraph();

	virtual void OnConstruction(const FTransform& Transform) override;

	TWeakPtr<Simulation::Vertex> GetVertexByID(const FSimVertexID& ID);

#if WITH_EDITOR
	void ResetVertices(){VerticesSerialized.Empty();}
	void AddVertex(const FSimVertexID& VertexID, const FVector& Location){VerticesSerialized.Add(VertexID, Location);}
#endif
	FVector GetVertexLocationByID(const FSimVertexID& ID);
	void DrawGraph(FColor Color, float LifeTime, float Thickness);

	void LoadGraph();
	void LoadObjects_Initial();
	void LoadObjects_Save(USimulationState* Save); //TODO: Accept SaveObject
	void SaveObjects(USimulationState* Save); //TODO: Accept SaveObject and write here
	void UnloadGraph();
	void UnloadObjects();

	FSimVertexID GetProfileLocationOnGraph(USimProfileBase* Profile);

	void AddProfileOnGraph(USimProfileBase* Profile, const FSimVertexID& Vertex);
	void RegisterChildProfile(USimProfileBase* Profile, USimProfileBase* Parent);

	//FORCEINLINE UProfileIDController* GetProfileIDController(){return ProfileIDController;}

	int GetProfiles(TArray<USimProfileBase*>& Profiles){return ProfileHolders.GetKeys(Profiles);}
	TArray<USimProfileBase*> GetProfilesByClass(TSubclassOf<USimProfileBase> Class);

	const TArray<AGraphAsset*>& GetChunks(){return LocalGraphs;}
	AGraphAsset* GetChunkByID(const FSimVertexID& ID);
	
	int GetProfilesInChunk(int ChunkIndex, TArray<USimProfileBase*>& Profiles);
	
	/*float GetMinimumDistance();
	float GetDefaultDistance();
	float GetMaximumDistance();*/

	UFUNCTION()
	void AsyncLoadChunks();

	UFUNCTION()
	void AsyncLoadChunksCompleted();
	USquadTaskBase* CreateNewTask(UAISimProfileSquad* Squad);

	UFUNCTION(BlueprintCallable)
	void SetProfileLocation_World(USimProfileBase* Profile, const FSimVertexID& SimVertexID);

	UFUNCTION(BlueprintCallable)
	void SetProfileLocation_Child(USimProfileBase* Profile, USimProfileBase* Parent);
	
	void SetChunks(TArray<AActor*> Array);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ParallelTick(USimProfileBase* Profile, float DeltaTime);

	UFUNCTION(BlueprintPure)
	FORCEINLINE UProfileIDController* GetProfileIDsController(){return ProfileIDController;}

protected:

	int LoadUUID = 0;

	UPROPERTY(Config)
	TEnumAsByte<ECollisionChannel> LevelTraceChannel;

	UPROPERTY()
	TArray<AGraphAsset*> LocalGraphs;
	int LoadIndex = 0;

	UPROPERTY()
	TMap<FSimVertexID, FVector> VerticesSerialized;

	UPROPERTY()
	TObjectPtr<UProfileIDController> ProfileIDController = nullptr;

	UPROPERTY()
	TObjectPtr<UCommunityRelationTable> CommunityRelationsRegistry = nullptr;
	
	UPROPERTY(Transient)
	TMap<USimProfileBase*, FSimProfileHolder> ProfileHolders;

	TArray<TSharedPtr<Simulation::Vertex>> Links;

	UPROPERTY(Transient)
	TSet<UAISimProfileBase*> Players;
};
