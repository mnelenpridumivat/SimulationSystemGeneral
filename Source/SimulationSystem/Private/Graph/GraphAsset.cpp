// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GraphAsset.h"

#include "DrawDebugHelpers.h"
//#include "ExposedFunctionLibrary.h"
#include "GlobalGraph.h"
#include "GraphDataAsset.h"
#include "LocalGraphRegistry.h"
#include "SimProfileBase.h"
#include "SimulationFunctionLibrary.h"
#include "SimProfileContainer.h"
#include "SimulationSystemSettings.h"
#include "SimulationSystemSubsystem.h"
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
#if WITH_EDITOR
	if (!ensure(ChunkGraphs.IsValidIndex(ID.LevelID)))
	{
		return nullptr;
	}
#endif
	auto& KeyArr = ChunkGraphs[ID.LevelID].Vertices;
#if WITH_EDITOR
	if (!ensure(KeyArr.IsValidIndex(ID.VertexID)))
	{
		return nullptr;
	}
#endif
	return KeyArr[ID.VertexID];
}
void AGraphAsset::DrawGraph(FColor Color, float LifeTime, float Thickness)
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	for (const auto& Layer : ChunkGraphs)
	{
		for (const auto& Edge : Layer.Edges)
		{
			auto VertexOne = GlobalGraph->GetVertexLocationByID(Edge->GetVertexOne().Pin()->GetVertexID());
			auto VertexTwo = GlobalGraph->GetVertexLocationByID(Edge->GetVertexTwo().Pin()->GetVertexID());
			// TODO: Optimize this
			DrawDebugLine(GetWorld(), VertexOne, VertexTwo, Color, false, LifeTime, 0, Thickness);
		}
	}
}

#if WITH_EDITOR
bool AGraphAsset::LayerWithNameExists(FName Name)
{
	for (const auto& Layer : ChunkGraphs)
	{
		if (Layer.LayerName == Name)
		{
			return true;
		}
	}
	return false;
}

FGraphLayer& AGraphAsset::GetLayerByName(FName Name)
{
	static FGraphLayer Backup;
	Backup.Edges.Empty();
	Backup.Vertices.Empty();
	if (!ensure(LayerWithNameExists(Name)))
	{
		return Backup;
	}
	for (auto& Layer : ChunkGraphs)
	{
		if (Layer.LayerName == Name)
		{
			return Layer;
		}
	}
	check(false); // Something too dangerous happened
	return Backup;
}
#endif

FVector AGraphAsset::GetVertexLocationByID(const FSimVertexID& ID)
{
	auto& KeyArr = ChunkGraphs[ID.LevelID].Vertices;
	return KeyArr[ID.VertexID]->GetLocation();
}

FSimVertexID AGraphAsset::FindClosestVertex(const FVector& Location, int LayerIndex)
{
	if (!ensureMsgf(
		ChunkGraphs.IsValidIndex(LayerIndex),
		TEXT("Attempt to search closest vertex in chunk [%s] in invalid layer [%d]"),
		*GetFName().ToString(), LayerIndex))
	{
		return FSimVertexID::Invalid;
	}
	TWeakPtr<Simulation::Vertex> ClosestVertex = ChunkGraphs[LayerIndex].Vertices[0];
	long double ClosestDistanceSq = FVector::DistSquared(Location, ClosestVertex.Pin()->GetLocation());
	bool FoundCloser;
	do
	{
		FoundCloser = false;
		auto NewCloser = ClosestVertex;
		for(auto& Edge : ClosestVertex.Pin()->GetEdges())
		{
			auto Opposite = Edge.Pin()->GetOpposite(ClosestVertex);
			float OppositeDistanceSq = FVector::DistSquared(Opposite.Pin()->GetLocation(), Location);
			if(OppositeDistanceSq < ClosestDistanceSq)
			{
				FoundCloser = true;
				ClosestDistanceSq = OppositeDistanceSq;
				NewCloser = Opposite;
			}
		}
		ClosestVertex = NewCloser;
	} while (FoundCloser);
	return ClosestVertex.Pin()->GetVertexID();
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

/*void AGraphAsset::LoadObjects_Initial()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	for (auto& elem : InitialProfiles.Objects)
	{
		elem.GlobalParent = &InitialProfiles;
		for (auto& Chunk : elem.Children)
		{
			for (int Index : Chunk.Children)
			{
				InitialProfiles.Objects[Index].Parent = &elem;
			}
		}
	}
	for (auto& elem : InitialProfiles.Objects)
	{
		if (!elem.Parent)
		{
			auto& VertexID = elem.VertexLocation;
			auto Profile = LoadProfile(elem);
			GlobalGraph->AddProfileOnGraph(Profile, VertexID);
		}
	}
}*/

FGraphSerialized AGraphAsset::SaveObjects()
{
	FGraphSerialized SavedData;
	
	return SavedData;
}

void AGraphAsset::UnloadGraph()
{
	ChunkGraphs.Empty();
	Registry = nullptr;
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

#if WITH_EDITOR
void AGraphAsset::SetGraph(UGraphDataAsset* GraphSerialized)
{
	Graph = GraphSerialized;
}
#endif

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

/*USimProfileBase* AGraphAsset::LoadProfile(FSerializedProfile& Data)
{
	USimProfileBase* Profile = NewObject<USimProfileBase>(GetWorld(), Data.ObjectClass);
	USimulationFunctionLibrary::LoadObjectData(Profile, Data.ObjectData);
	Profile->NativeLoad(Data);
	return Profile;
}*/

bool AGraphAsset::LoadGraph()
{
	UnloadGraph();
	if (ensureMsgf(
		!Graph.IsNull(),
		TEXT("Graph file reference in %s is invalid"),
		*GetName()))
	{
		UGraphDataAsset* GraphFile = Graph.LoadSynchronous();
		if (ensureMsgf(
			IsValid(GraphFile),
			TEXT("Unable to load graph in [%s] from invalid graph file!"),
			*GetName()))
		{
			{
				// Vertices
				const auto& Data = GraphFile->Graph;
				ChunkGraphs.SetNumZeroed(Data.Layers.Num());
				const auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
				if (ensureMsgf(
					IsValid(GlobalGraph),
					TEXT("Global graph is invalid during graph loading!")))
				{
					for (size_t i = 0; i < Data.Layers.Num(); i++)
					{
						const auto& CurrentLayer = Data.Layers[i];
						auto& RealLayer = ChunkGraphs[i];
#if WITH_EDITOR
						RealLayer.LayerName = CurrentLayer.LayerName;
#endif
						for (const auto& VertexData : CurrentLayer.Vertices)
						{
							RealLayer.Vertices.Add(MakeShared<Simulation::Vertex>(VertexData.Key, VertexData.Value));
						}
						for (const auto& EdgeData : CurrentLayer.Edges)
						{
							auto VertexOne = GlobalGraph->GetVertexByID(EdgeData.VertexOne);
							auto VertexTwo = GlobalGraph->GetVertexByID(EdgeData.VertexTwo);
#if WITH_EDITOR
							if (!ensure(VertexOne.IsValid() && VertexTwo.IsValid()))
							{
								continue;
							}
#endif
							RealLayer.Edges.Add(MakeShared<Simulation::Edge>(VertexOne, VertexTwo));
							VertexOne.Pin()->AddEdge(RealLayer.Edges.Last());
							VertexTwo.Pin()->AddEdge(RealLayer.Edges.Last());
						}
					}
				}
			}
			{
				// Profiles
				const auto& Data = GraphFile->Profiles;
				for (size_t i = 0; i < Data.Objects.Num(); i++)
				{
					auto& Current = Data.Objects[i];
					if (ensure(Current.HasData))
					{
						USimulationFunctionLibrary::GetSimulationSystemSubsystem(GetWorld())->
							SpawnProfile(GetWorld(), Current.Archetype, Current.VertexLocation);
					}
				}
			}
			
		}
	}
	else
	{
		return false;
	}
	if (ensureMsgf(
		!GetDefault<USimulationSystemSettings>()->LocalGraphRegistryClass.IsNull(),
		TEXT("There is no LocalGraphRegistry class in SimulationSystemSettings!")))
	{
		auto Class = GetDefault<USimulationSystemSettings>()->LocalGraphRegistryClass.LoadSynchronous();
		Registry = NewObject<ULocalGraphRegistry>(GetWorld(), Class);
		Registry->SetLocalGraph(this);
	}
	else
	{
		return false;
	}
	return true;
}

