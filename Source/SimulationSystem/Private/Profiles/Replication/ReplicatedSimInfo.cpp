// Fill out your copyright notice in the Description page of Project Settings.


#include "Replication/ReplicatedSimInfo.h"

#include "GlobalGraph.h"
#include "ProfileIDController.h"
#include "SimProfileBase.h"
#include "SimulationFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

void AReplicatedSimInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AReplicatedSimInfo, ProfileID);
	DOREPLIFETIME(AReplicatedSimInfo, ReplicatedInfoChildren);
}

void AReplicatedSimInfo::GetChildren(TArray<FSimProfileID>& ChildrenArray) const
{
	ChildrenArray = ReplicatedInfoChildren;
}

void AReplicatedSimInfo::SetProfileID(FSimProfileID NewProfileID)
{
#if WITH_EDITOR
	if(GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("Attempt to set profile ID to [%s, %u] from client!"), *NewProfileID.Class->GetName(), NewProfileID.ProfileID);
		return;
	}
#else
	ensureMsgf(GetNetMode() == NM_Client, TEXT("Attempt to set profile ID to [%s, %u] from client!"), *NewProfileID.Class->GetName(), NewProfileID.ProfileID);
#endif
	FReplicatedSimProfileID ReplicatedProfileID;
	ReplicatedProfileID.Class = NewProfileID.Class;
	ReplicatedProfileID.ProfileID = *reinterpret_cast<int*>(&NewProfileID.ProfileID);
	Multicast_SetProfileID(ReplicatedProfileID);
}

FSimProfileID AReplicatedSimInfo::GetProfileID() const
{
	return ProfileID;
}

void AReplicatedSimInfo::AddChild(USimProfileBase* Profile)
{
	ensureMsgf(!ReplicatedInfoChildren.Contains(Profile->GetProfileID()), TEXT("Attempt to add profile [%s] second time in [%s]!"),
	           *Profile->GetName(), *GetName());
	ReplicatedInfoChildren.Add(Profile->GetProfileID());
}

void AReplicatedSimInfo::RemoveChild(USimProfileBase* Profile)
{
	ReplicatedInfoChildren.RemoveSwap(Profile->GetProfileID());
}

void AReplicatedSimInfo::BeginDestroy()
{
	Super::BeginDestroy();
#if WITH_EDITOR
	const auto IDController = USimulationFunctionLibrary::GetProfileIDController(GetWorld());
	if(!IsValid(IDController))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to access global graph actor in world [%s]"), *(GetWorld() ? GetWorld()->GetName() : FString("null")));
		return;
	}
	IDController->RemoveReplicatedSimInfo(this);
#else
	USimulationFunctionLibrary::GetProfileIDController(GetWorld())->RemoveReplicatedSimInfo(this);
#endif
}

void AReplicatedSimInfo::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITOR
	const auto IDController = USimulationFunctionLibrary::GetProfileIDController(GetWorld());
	if(!IsValid(IDController))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to access global graph actor in world [%s]"), *(GetWorld() ? GetWorld()->GetName() : FString("null")));
		return;
	}
	IDController->AddReplicatedSimInfo(this);
#else
	USimulationFunctionLibrary::GetProfileIDController(GetWorld())->AddReplicatedSimInfo(this);
#endif
}

void AReplicatedSimInfo::Multicast_SetProfileID_Implementation(FReplicatedSimProfileID ReplicatedProfileID)
{
	ProfileID.Class = ReplicatedProfileID.Class;
	ProfileID.ProfileID = *reinterpret_cast<uint32*>(&ReplicatedProfileID.ProfileID);
}
