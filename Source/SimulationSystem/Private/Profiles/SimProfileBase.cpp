// Fill out your copyright notice in the Description page of Project Settings.


#include "Profiles/SimProfileBase.h"

#include "NavigationSystem.h"
#include "ProfileComponent.h"
#include "ProfileIDController.h"
#include "SimActorInterface.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSettings.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "ReplicatedSimInfo.h"
#include "Kismet/GameplayStatics.h"

void USimProfileBase::OnRegistered_Implementation()
{
	
}

bool USimProfileBase::IsMovable_Implementation()
{
	return true;
}

USimProfileBase::USimProfileBase()
{
	ProfileID = UProfileIDController::InvalidID;
}

void USimProfileBase::OnCreated_Implementation()
{
}

void USimProfileBase::OnLoaded_Implementation()
{
}

USimProfileBase* USimProfileBase::DeepCopyProfile()
{
	auto Copy = DuplicateObject(this, GetWorld());
	Copy->OnCreated();
	return Copy;
}

void USimProfileBase::TransitOfflineBuffered()
{
}

void USimProfileBase::TransitBufferedOffline()
{
}

void USimProfileBase::TransitBufferedOnline()
{
	if(OnlineActorClass.IsNull())
	{
		return;
	}
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	StreamableHandle = Streamable.RequestAsyncLoad(
		OnlineActorClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(
			this,
			&USimProfileBase::OnOnlineActorClassLoaded
			));
}

void USimProfileBase::TransitOnlineBuffered()
{
	if(StreamableHandle.IsValid())
	{
		StreamableHandle.Get()->CancelHandle();
		StreamableHandle.Reset();
	}
	if(IsValid(OnlineActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Destroying online actor %s for profile %s!"), *OnlineActor->GetName(), *GetName());
		OnlineActor->Destroy();
	}
}

void USimProfileBase::OnOnlineActorClassLoaded()
{
	//OnlineActor = GetWorld()->SpawnActor(OnlineActorClass.Get(), &OnlineLocation);
	FNavLocation ProjectedPoint(OnlineLocation.GetLocation());
	// TODO: Add switcher if force spawn on navmesh
	/*auto Navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	auto Project = [&](float ProjectDistance)
	{
		return Navigation->ProjectPointToNavigation(
		OnlineLocation.GetLocation(),
		ProjectedPoint,
		FVector(ProjectDistance)
		);
	};
	if(!Project(100.0f))
	{
		if(!Project(GetMutableDefault<USimulationSystemSettings>()->OnlineRadius/10))
		{
			if(!Project(GetMutableDefault<USimulationSystemSettings>()->OnlineRadius))
			{
				UE_LOG(LogTemp, Error, TEXT("Unable to find any projection on navmesh at location [%s]"), *OnlineLocation.ToString());
				return;
			}
		}
	}*/
	FVector SpawnLocation = ProjectedPoint.Location;
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	OnlineActor = GetWorld()->SpawnActor(OnlineActorClass.Get(), &SpawnLocation, nullptr, params);
	if(!OnlineActor->GetClass()->ImplementsInterface(USimActorInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn actor from profile without ISimActorInterface!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Online actor %s for profile %s spawned!"), *OnlineActor->GetName(), *GetName());
	ISimActorInterface::Execute_GetProfileComponent(OnlineActor)->SetProfileID(ProfileID);
}

void USimProfileBase::SetOnlineLocation(FVector Vector)
{
	OnlineLocation.SetLocation(Vector);
}

FTransform USimProfileBase::GetOnlineLocation() const
{
	if(IsValid(OnlineActor))
	{
		return OnlineActor->GetActorTransform();
	}
	return OnlineLocation;
}

void USimProfileBase::Tick_Implementation(float DeltaTime)
{
}

void USimProfileBase::OnExit_Implementation(const FSimProfileHolder& Location)
{
	bool Success;
	USimProfileBase* Parent = nullptr;
	USimulationFunctionLibrary::AsProfile(Location, Parent, Success);
	if(Success)
	{
		ISimProfileContainer::Execute_RemoveItem(Parent, this);
	}
}

void USimProfileBase::OnEnter_Implementation(const FSimProfileHolder& Location)
{
	
	bool Success;
	USimProfileBase* Parent = nullptr;
	USimulationFunctionLibrary::AsProfile(Location, Parent, Success);
	if(Success)
	{
		ISimProfileContainer::Execute_AddItem(Parent, this);
	}
}

void USimProfileBase::SetupReplicatedData(AReplicatedSimInfo* ReplicatedInfo) const
{
	ReplicatedInfo->SetProfileID(ProfileID);
}
