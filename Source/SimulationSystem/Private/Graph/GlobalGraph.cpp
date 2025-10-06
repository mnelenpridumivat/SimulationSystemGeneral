// Fill out your copyright notice in the Description page of Project Settings.

#include "GlobalGraph.h"

#include "AISimProfileBase.h"
#include "EngineUtils.h"
//#include "ExposedFunctionLibrary.h"
#include "AISimProfilePawn.h"
#include "GraphAsset.h"
#include "ProfileIDController.h"
#include "SimProfileCamp.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationState.h"
#include "SimulationSystemSettings.h"
#include "SquadTask_MoveToCamp.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "Profiles/SimProfileBase.h"
#include "Profiles/AISimProfileSquad.h"

void FSerializedProfile::ExtractFirstChildren(FProfilesSerializedView& ExtractedChildren)
{
	ExtractedChildren.Objects.Empty();
	if(!ensure(Children.IsValidIndex(0))
		|| !ensure(GlobalParent))
	{
		return;
	}
	ExtractedChildren.Objects.Reserve(Children[0].Children.Num());
	for (auto Child : Children[0].Children)
	{
		ExtractedChildren.Objects.Add(&GlobalParent->Objects[Child]);
	}
	Children.RemoveAt(0);
}

FSerializedProfileView FSerializedProfile::AddChild()
{
	if (!ensure(GlobalParent))
	{
		return FSerializedProfileView();
	}
	auto NewElemView = GlobalParent->AddLast();
	Children.Last().Children.Add(NewElemView.Index);
	auto& NewChild = NewElemView.GetElem();
	NewChild.Parent = this;
	NewChild.GlobalParent = GlobalParent;
	return NewElemView;
}

void FSerializedProfile::NextSet()
{
	Children.Add({});
}

FSerializedProfile& FSerializedProfileView::GetElem()
{
	if (!ensure(GlobalParent) || !ensure(GlobalParent->Objects.IsValidIndex(Index)))
	{
		static FSerializedProfile Invalid;
		return Invalid;
	}
	return GlobalParent->Objects[Index];
}

FSerializedProfileView FProfilesSerialized::AddLast()
{
	FSerializedProfileView ReturnProfile;
	ReturnProfile.GlobalParent = this;
	ReturnProfile.Index = Objects.Add({});
	return ReturnProfile;
}

void AGlobalGraph::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ...
	
	LoadGraph();
	LoadIndex = 0;
	AsyncLoadChunks(); // TODO: Remove temporary
	LoadObjects_Initial();
}

AGlobalGraph::AGlobalGraph()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
	//PrimaryActorTick.TickInterval = 0.5f;
	// ...
}

AGlobalGraph::~AGlobalGraph()
{
	UE_LOG(LogTemp, Log, TEXT("Destructor of global graph called!"));
}

void AGlobalGraph::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if(!IsValid(ProfileIDController))
	{
		ProfileIDController = NewObject<UProfileIDController>(this);
	}
}

TWeakPtr<Simulation::Vertex> AGlobalGraph::GetVertexByID(const FSimVertexID& ID)
{
	if (!ensureMsgf(ID.IsValid(), TEXT("Accessed invalid SimVertexID")))
	{
		return nullptr;
	}
	if(!ID.ChunkID)
	{
		return Links[ID.VertexID];
	}
	return LocalGraphs[ID.ChunkID-1]->GetVertex(ID);
}

FVector AGlobalGraph::GetVertexLocationByID(const FSimVertexID& ID)
{
	if (!ensureMsgf(ID.IsValid(), TEXT("Accessed invalid SimVertexID")))
	{
		return FVector::ZeroVector;
	}
	if(!ID.ChunkID)
	{
		return VerticesSerialized[ID];
	}
	return LocalGraphs[ID.ChunkID-1]->GetVertexLocationByID(ID);
}

void AGlobalGraph::DrawGraph(FColor Color, float LifeTime, float Thickness)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraphAsset::StaticClass(), Actors);
	for (auto& Actor : Actors)
	{
		Cast<AGraphAsset>(Actor)->DrawGraph(Color, LifeTime, Thickness);
	}
}

void AGlobalGraph::LoadGraph()
{
	for(auto& VertexData : VerticesSerialized)
	{
		Links.Add(MakeShared<Simulation::Vertex>(VertexData.Key, VertexData.Value));
	}
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraphAsset::StaticClass(), Actors);
	LocalGraphs.Empty(LocalGraphs.Num());
	for (auto& Actor : Actors)
	{
		auto CastedActor = Cast<AGraphAsset>(Actor);
		if(!LocalGraphs.IsValidIndex(CastedActor->GetChunkIndex()-1))
		{
			LocalGraphs.SetNumZeroed(CastedActor->GetChunkIndex());
		}
		LocalGraphs[CastedActor->GetChunkIndex()-1] = CastedActor;
		CastedActor->LoadGraph();
	}
	if (ensure(!IsValid(CommunityRelationsRegistry)))
	{
		auto RelationRegistryClass = GetDefault<USimulationSystemSettings>()->CommunityRelationTableClass;
		if (ensureMsgf(
			!RelationRegistryClass.IsNull(),
			TEXT("CommunityRelationTableClass is not set in SimulationSystemSettings!")))
		{
			CommunityRelationsRegistry = NewObject<UCommunityRelationTable>(GetWorld(), RelationRegistryClass.LoadSynchronous());
		}
	}
}

void AGlobalGraph::LoadObjects_Initial()
{
	for(auto& LocalGraph : LocalGraphs)
	{
		LocalGraph->LoadObjects_Initial();
	}
	TArray<USimProfileBase*> Profiles;
	int Num = GetProfiles(Profiles);
	for (auto Profile : Profiles)
	{
		Profile->OnPostRegistered();
	}
	SetActorTickEnabled(true);
}

void AGlobalGraph::LoadObjects_Save(USimulationState* Save)
{
	auto Profiles = Save->GetProfiles();
	for(auto& Profile : Profiles)
	{
		AddProfileOnGraph(Profile.Value, Profile.Key);
	}
	for (auto& Profile : Profiles)
	{
		Profile.Value->OnPostRegistered();
	}
	SetActorTickEnabled(true);
}

void AGlobalGraph::SaveObjects(USimulationState* Save)
{
	TMap<FSimVertexID, USimProfileBase*> ProfilesSerialized;
	for(auto& Profile : ProfileHolders)
	{
		bool Success = false;
		FSimVertexID VertexID = FSimVertexID::Invalid;
		USimulationFunctionLibrary::AsVertex(Profile.Value, VertexID, Success);
		if(!Success)
		{
			continue;
		}
		ProfilesSerialized.Add(VertexID, Profile.Key);
	}
	Save->SetProfiles(ProfilesSerialized);
}

void AGlobalGraph::UnloadGraph()
{
	Links.Empty();
	for(auto& LocalGraph : LocalGraphs)
	{
		LocalGraph->UnloadGraph();
	}
	if (ensure(IsValid(CommunityRelationsRegistry)))
	{
		CommunityRelationsRegistry = nullptr;
	}
}

void AGlobalGraph::UnloadObjects()
{
	SetActorTickEnabled(false);
	ProfileIDController->ClearAllProfiles();
	Players.Empty();
	for(auto& elem : ProfileHolders)
	{
		AActor* OnlineActor = elem.Key->GetOnlineActor();
		if(IsValid(OnlineActor))
		{
			OnlineActor->Destroy();
		}
	}
	ProfileHolders.Empty();
	GEngine->ForceGarbageCollection(true);
}

FSimVertexID AGlobalGraph::GetProfileLocationOnGraph(USimProfileBase* Profile)
{
	FSimVertexID VertexID = {static_cast<uint16>(-1), static_cast<uint8>(-1), static_cast<uint16>(-1)};
	while(!VertexID.IsValid())
	{
		auto HolderIt = ProfileHolders.Find(Profile);
		if (!ensure(HolderIt))
		{
			break;
		}
		auto Holder = *HolderIt;
		bool bSuccess = false;
		USimulationFunctionLibrary::AsVertex(Holder, VertexID, bSuccess);
		if(!bSuccess)
		{
			USimulationFunctionLibrary::AsProfile(Holder, Profile, bSuccess);
			if (!ensureMsgf(bSuccess, TEXT("Unable to get holder of profile [%s]"), *Profile->GetClass()->GetName()))
			{
				break;
			}
		}
	}
	return VertexID;
}

void AGlobalGraph::AddProfileOnGraph(USimProfileBase* Profile, const FSimVertexID& Vertex)
{
	if (!ensureMsgf(IsValid(Profile), TEXT("Profile is not valid")))
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Add profile [%s] (global graph context = [%s])"), *Profile->GetName(), *GetPathName());
	Profile->Rename(nullptr, GetWorld());
	Profile->SetProfileID(ProfileIDController->RegisterProfile(Profile));
	ProfileHolders.Add(Profile, FSimProfileHolder::FromVertex(GetWorld(), Vertex));
	Profile->OnRegistered();
}

void AGlobalGraph::RegisterChildProfile(USimProfileBase* Profile, USimProfileBase* Parent)
{
	if (!ensureMsgf(IsValid(Profile), TEXT("Profile is not valid")))
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Add profile [%s] (global graph context = [%s])"), *Profile->GetName(), *GetPathName());
	Profile->Rename(nullptr, GetWorld());
	Profile->SetProfileID(ProfileIDController->RegisterProfile(Profile));
	ProfileHolders.Add(Profile, FSimProfileHolder::FromProfile(Parent));
	Profile->OnRegistered();
}

TArray<USimProfileBase*> AGlobalGraph::GetProfilesByClass(TSubclassOf<USimProfileBase> Class)
{
	return ProfileIDController->GetProfiles(Class);
}

AGraphAsset* AGlobalGraph::GetChunkByID(const FSimVertexID& ID)
{
	if (!ensure(ID.IsValid()) || !ID.ChunkID || !ensure(LocalGraphs.IsValidIndex(ID.ChunkID - 1)))
	{
		return nullptr;
	}
	return LocalGraphs[ID.ChunkID - 1];
}

int AGlobalGraph::GetProfilesInChunk(int ChunkIndex, TArray<USimProfileBase*>& Profiles)
{
	Profiles.Empty();
	if (!ChunkIndex)
	{
		for(auto& Profile : ProfileHolders)
		{
			if(GetProfileLocationOnGraph(Profile.Key).ChunkID == ChunkIndex)
			{
				Profiles.Add(Profile.Key);
			}
		}
	} else if (ensureMsgf(
		LocalGraphs.IsValidIndex(ChunkIndex),
		TEXT("Call GetProfilesInChunk with invalid ChunkIndex [%d]!"),
		ChunkIndex
	))
	{
		auto CurrentGraph = LocalGraphs[ChunkIndex];
		if (ensure(CurrentGraph))
		{
			auto Registry = CurrentGraph->GetRegistry();
			if (ensure(Registry))
			{
				Registry->GetAllProfiles(Profiles);
			}
		}
	}
	return Profiles.Num();
}

void AGlobalGraph::SetProfileLocation_World(USimProfileBase* Profile, const FSimVertexID& Vertex)
{
	auto Holder = ProfileHolders.Find(Profile);
	Profile->OnExit(*Holder);
	*Holder = FSimProfileHolder::FromVertex(GetWorld(), Vertex);
	Profile->OnEnter(*Holder);
}

void AGlobalGraph::SetProfileLocation_Child(USimProfileBase* Profile, USimProfileBase* Parent)
{
	auto Holder = ProfileHolders.Find(Profile);
	Profile->OnExit(*Holder);
	*Holder = FSimProfileHolder::FromProfile(Parent);
	Profile->OnEnter(*Holder);
}

void AGlobalGraph::SetChunks(TArray<AActor*> Array)
{
	LocalGraphs.Empty();
	for(auto& elem : Array)
	{
		LocalGraphs.Add(Cast<AGraphAsset>(elem));
	}
}

void AGlobalGraph::AsyncLoadChunks()
{
	auto Level = UGameplayStatics::GetStreamingLevel(GetWorld(), LocalGraphs[LoadIndex]->GetCurrentLevel());
	FLatentActionInfo info(0, LoadUUID++, TEXT("AsyncLoadChunksCompleted"), this);
	UGameplayStatics::LoadStreamLevel(GetWorld(), LocalGraphs[LoadIndex]->GetCurrentLevel(), true, false, info);
}

void AGlobalGraph::AsyncLoadChunksCompleted()
{
	++LoadIndex;
	if(LoadIndex==LocalGraphs.Num())
	{
		return;
	}
	AsyncLoadChunks();
}

USquadTaskBase* AGlobalGraph::CreateNewTask(UAISimProfileSquad* Squad)
{
	auto NewTask = NewObject<USquadTask_MoveToCamp>(GetWorld());
	auto Camps = GetProfilesByClass(USimProfileCamp::StaticClass());
	if(!Camps.Num())
	{
		return nullptr;
	}
	NewTask->SetupTask(GetProfileLocationOnGraph(Squad), GetProfileLocationOnGraph(Camps[FMath::RandRange(0, Camps.Num()-1)]));
	return NewTask;
}

// Called when the game starts
void AGlobalGraph::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGlobalGraph::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Log, TEXT("Destruction of global graph actor!"));
}


// Called every frame
void AGlobalGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TArray<USimProfileBase*> Profiles;
	int Num = GetProfiles(Profiles);
	auto func = [&](int32 Index)
	{
		// TODO^ Fix normally player pawn simulation switching
		if(Profiles[Index]->IsA(UAISimProfilePawn::StaticClass()))
		{
			Profiles[Index]->SetSimLevel(ESimulationLevels_Online);
			return;
		}

		float& Time = Profiles[Index]->TimeSinceLastTick;
		Time += DeltaTime;
		FVector ProfileLocation = GetVertexLocationByID(GetProfileLocationOnGraph(Profiles[Index]));
		FVector ClosestPlayerLocation;
		APawn* PlayerPawn = nullptr;
		for(TActorIterator<APlayerController> It(GetWorld()); It; ++It)
		{
			if(!PlayerPawn)
			{
				PlayerPawn = (*It)->GetPawn();
				if(!IsValid(PlayerPawn))
				{
					continue;
				}
				ClosestPlayerLocation = PlayerPawn->GetActorLocation();
				continue;
			}
			PlayerPawn = (*It)->GetPawn();
			if(!IsValid(PlayerPawn))
			{
				continue;
			}
			if(FVector::DistSquared(ProfileLocation, ClosestPlayerLocation) > FVector::DistSquared(ProfileLocation, PlayerPawn->GetActorLocation()))
			{
				ClosestPlayerLocation = PlayerPawn->GetActorLocation();
			}
		}
		if(!PlayerPawn)
		{
			return;
		}
		if(FVector::DistSquared(ProfileLocation, ClosestPlayerLocation) < FMath::Square(GetMutableDefault<USimulationSystemSettings>()->OnlineRadius))
		{
			if(Profiles[Index]->GetSimLevel() != ESimulationLevels_Online)
			{
				if(Profiles[Index]->GetSimLevel() == ESimulationLevels_Offline)
				{
					UE_LOG(LogTemp, Log, TEXT("Transfer profile %s from offline to online"), *Profiles[Index]->GetName());
					Profiles[Index]->TransitOfflineOnline();
				} else
				{
					UE_LOG(LogTemp, Log, TEXT("Transfer profile %s from buffered to online"), *Profiles[Index]->GetName());
					Profiles[Index]->TransitBufferedOnline();
				}
				Profiles[Index]->SetSimLevel(ESimulationLevels_Online);
			}
		} else if (FVector::DistSquared(ProfileLocation, ClosestPlayerLocation) < FMath::Square(GetMutableDefault<USimulationSystemSettings>()->BufferedRadius))
		{
			if(Profiles[Index]->GetSimLevel() != ESimulationLevels_Buffered)
			{
				if(Profiles[Index]->GetSimLevel() == ESimulationLevels_Online)
				{
					UE_LOG(LogTemp, Log, TEXT("Transfer profile %s from online to buffered"), *Profiles[Index]->GetName());
					Profiles[Index]->TransitOnlineBuffered();
				} else
				{
					UE_LOG(LogTemp, Log, TEXT("Transfer profile %s from offline to buffered"), *Profiles[Index]->GetName());
					Profiles[Index]->TransitOfflineBuffered();
				}
				Profiles[Index]->SetSimLevel(ESimulationLevels_Buffered);
			}
		} else
		{
			if(Profiles[Index]->GetSimLevel() != ESimulationLevels_Offline)
			{
				if(Profiles[Index]->GetSimLevel() == ESimulationLevels_Online)
				{
					UE_LOG(LogTemp, Log, TEXT("Transfer profile %s from online to offline"), *Profiles[Index]->GetName());
					Profiles[Index]->TransitOnlineOffline();
				} else
				{
					UE_LOG(LogTemp, Log, TEXT("Transfer profile %s from buffered to offline"), *Profiles[Index]->GetName());
					Profiles[Index]->TransitBufferedOffline();
				}
				Profiles[Index]->SetSimLevel(ESimulationLevels_Offline);
			}
		}
		constexpr int Tolerance = 1000;
		switch (Profiles[Index]->GetSimLevel())
		{
		case ESimulationLevels_Offline:
			{
				auto MinDeltaTime = GetDefault<USimulationSystemSettings>()->TickOffline * Profiles[Index]->GetTickOfflineCoeff();
				if((int)((Time * Tolerance)-(MinDeltaTime * Tolerance)) < 0)
				{
					return;
				}
				ParallelTick(Profiles[Index], Time);
				Time = (float)((int)(Time*Tolerance)%(int)(MinDeltaTime * Tolerance))/Tolerance;
				break;
			}
		case ESimulationLevels_Buffered:
			{
				auto MinDeltaTime = GetDefault<USimulationSystemSettings>()->TickBuffered * Profiles[Index]->GetTickBufferedCoeff();
				if((int)((Time * Tolerance)-(MinDeltaTime * Tolerance)) < 0)
				{
					return;
				}
				ParallelTick(Profiles[Index], Time);
				Time = (float)((int)(Time*Tolerance)%(int)(MinDeltaTime * Tolerance))/Tolerance;
				break;
			}
		case ESimulationLevels_Online:
			{
				auto MinDeltaTime = GetDefault<USimulationSystemSettings>()->TickOnline * Profiles[Index]->GetTickOnlineCoeff();
				if((int)((Time * Tolerance)-(MinDeltaTime * Tolerance)) < 0)
				{
					return;
				}
				ParallelTick(Profiles[Index], Time);
				Time = (float)((int)(Time*Tolerance)%(int)(MinDeltaTime * Tolerance))/Tolerance;
				break;
			}
		default:
			{
				UE_LOG(LogTemp, Fatal, TEXT("Profile [%s] has invalid simulation level active!"), *Profiles[Index]->GetName());
			}
		}
	};
	if constexpr (constexpr bool PARALLEL_SIM_PROCESS = true)
	{
		for(int32 i = 0; i < Num; ++i)
		{
			func(i);
		}
	} else {
		ParallelFor(Num, func);
	}
}

void AGlobalGraph::ParallelTick(USimProfileBase* Profile, float DeltaTime)
{
	float Closest = -1;
	for(auto& Player : Players)
	{
		float Dist = FVector::Dist(
			GetVertexLocationByID(GetProfileLocationOnGraph(Profile)),
			GetVertexLocationByID(GetProfileLocationOnGraph(Player))
			);
		if(Closest < 0 || Dist < Closest)
		{
			Closest = Dist;
		}
	}
	if(Closest >= 0)
	{
		if(Closest <= GetMutableDefault<USimulationSystemSettings>()->OnlineRadius)
		{
			Profile->SetSimLevel(ESimulationLevels_Online);
		}
		else if(Closest <= GetMutableDefault<USimulationSystemSettings>()->BufferedRadius)
		{
			Profile->SetSimLevel(ESimulationLevels_Buffered);
		}
		else
		{
			Profile->SetSimLevel(ESimulationLevels_Offline);
		}
	}
	Profile->Tick(DeltaTime);
}

