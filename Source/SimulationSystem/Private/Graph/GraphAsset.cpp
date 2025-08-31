// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GraphAsset.h"

#include "DrawDebugHelpers.h"
//#include "ExposedFunctionLibrary.h"
#include "GlobalGraph.h"
#include "SimProfileBase.h"
#include "SimulationFunctionLibrary.h"
#include "SimProfileContainer.h"
#include "SimVertexID.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


// Sets default values
AGraphAsset::AGraphAsset()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//CurrentChunkSelector = CreateDefaultSubobject<UNameSelectorComponent>("CurrentChunkSelector");
	SizeBox = CreateDefaultSubobject<UBoxComponent>("SizeBox");
	SizeBox->SetupAttachment(RootComponent);
}

TWeakPtr<Simulation::Vertex> AGraphAsset::GetVertex(const FSimVertexID& ID)
{
	auto& KeyArr = ChunkGraphs[ID.LevelID].Key;
	return KeyArr[ID.VertexID];
}
void AGraphAsset::DrawGraph(FColor Color, float LifeTime, float Thickness)
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	for(auto& Edge : Graph.Edges)
	{
		auto VertexOne = GlobalGraph->GetVertexLocationByID(Edge.VertexOne);
		auto VertexTwo = GlobalGraph->GetVertexLocationByID(Edge.VertexTwo);
		DrawDebugLine(GetWorld(), VertexOne, VertexTwo, Color, false, LifeTime, 0, Thickness);
	}
}

FVector AGraphAsset::GetVertexLocationByID(const FSimVertexID& ID)
{
	return Graph.Vertices[ID];
}

// Called when the game starts or when spawned
void AGraphAsset::BeginPlay()
{
	Super::BeginPlay();
	/*if(LevelName != NAME_None)
	{
		FLatentActionInfo info;
		UGameplayStatics::LoadStreamLevel(GetWorld(), LevelName, true, false, info);
	}*/
}

void AGraphAsset::LoadObjects_Save()
{
}

void AGraphAsset::LoadObjects_Initial()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	for(int i = 0 ; i < InitialProfiles.Objects.Num(); ++i)
	{
		auto& VertexID = InitialProfiles.Objects[i].VertexLocation;
		auto Profile = LoadProfile(i);
		GlobalGraph->AddProfileOnGraph(Profile, VertexID);
		//GlobalGraph->AddProfileOnGraph(Profile->DeepCopyProfile(), VertexID);
		//Profile->OnCreated();
	}
}

FGraphSerialized AGraphAsset::SaveObjects()
{
	FGraphSerialized SavedData;
	
	return SavedData;
}

void AGraphAsset::UnloadGraph()
{
	ChunkGraphs.Empty();
}

int AGraphAsset::GetChunkIndex()
{
	return ChunkIndex;
}

void AGraphAsset::SetChunkIndex(int NewChunkIndex)
{
	ChunkIndex = NewChunkIndex;
}

FVector AGraphAsset::GetZoneSize()
{
	return SizeBox->GetScaledBoxExtent();
}

FName AGraphAsset::GetCurrentLevel()
{
	return LevelName;
}

// Called every frame
void AGraphAsset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGraphAsset::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnloadGraph();
}

USimProfileBase* AGraphAsset::LoadProfile(int& Index, int ChildrenNum)
{
	// TODO: Think about serialization for non-default containers (like Zone17 NPC)
	const auto& CurrentProfileData = InitialProfiles.Objects[Index];
	USimProfileBase* Profile = NewObject<USimProfileBase>(GetWorld(), CurrentProfileData.ObjectClass);
	USimulationFunctionLibrary::LoadObjectData(Profile, CurrentProfileData.ObjectData.SerializedObject);
	if(Profile->GetClass()->ImplementsInterface(USimProfileContainer::StaticClass()))
	{
		for(uint32 i = 0; i < CurrentProfileData.ChildrenNum; ++i)
		{
			ISimProfileContainer::Execute_AddItem(Profile, LoadProfile(++Index));
		}
	}
	return Profile;
}

void AGraphAsset::LoadGraph()
{
	UnloadGraph();
	//ChunkGraphs.Init(LevelGraph(), Graph.Vertices.Num());
	for(auto& VertexData : Graph.Vertices)
	{
		if(!ChunkGraphs.IsValidIndex(VertexData.Key.LevelID))
		{
			ChunkGraphs.SetNumZeroed(VertexData.Key.LevelID+1);
		}
		auto& KeyArr = ChunkGraphs[VertexData.Key.LevelID].Key;
		KeyArr.Add(MakeShared<Simulation::Vertex>(VertexData.Key, VertexData.Value));
	}
	const auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	for(auto& EdgeData : Graph.Edges)
	{
		auto LayerIndex = EdgeData.VertexOne.LevelID;
		auto VertexOne = GlobalGraph->GetVertexByID(EdgeData.VertexOne);
		auto VertexTwo = GlobalGraph->GetVertexByID(EdgeData.VertexTwo);
		ChunkGraphs[LayerIndex].Value.Add(MakeShared<Simulation::Edge>(VertexOne, VertexTwo));
		VertexOne.Pin()->AddEdge(ChunkGraphs[LayerIndex].Value.Last());
		VertexTwo.Pin()->AddEdge(ChunkGraphs[LayerIndex].Value.Last());
	}
	//for(auto& ObjectData : Graph.Objects)
	//{
	//	auto& Vertex = ChunkGraphs[ObjectData.Key.LevelID].Key[ObjectData.Key.VertexID];
	//	auto Object = UExposedFunctionLibrary::DeserializeObject(GetWorld(), ObjectData.Value);
	//}
}

