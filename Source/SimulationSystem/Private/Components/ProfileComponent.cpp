// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProfileComponent.h"

#include "GlobalGraph.h"
#include "ProfileGenerator.h"
#include "ProfileIDController.h"
#include "SimActorInterface.h"
#include "SimProfileBase.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSettings.h"
#include "Net/UnrealNetwork.h"
//#include "ReplicatedSimInfo.h"


void UProfileComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UProfileComponent, ReplicatedInfo);
	DOREPLIFETIME(UProfileComponent, ProfileID);
	DOREPLIFETIME(UProfileComponent, Profile);
}

// Sets default values for this component's properties
UProfileComponent::UProfileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bReplicateUsingRegisteredSubObjectList = true;

	ProfileID = UProfileIDController::InvalidID;

	// ...
}


// Called when the game starts
void UProfileComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->IsEditorOnly())
	{
		return;
	}

	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	if (!ensureMsgf(IsValid(GlobalGraph), TEXT("Unable to access GlobalGraph!")))
	{
		return;
	}
	
	if (ProfileID != UProfileIDController::InvalidID)
	{
		Profile = GlobalGraph->GetProfileIDsController()->GetProfile(ProfileID);
		if (ensure(IsValid(Profile)))
		{
			AddReplicatedSubObject(Profile);
		}
	}
}

void UProfileComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetOwner()->IsEditorOnly())
	{
		return;
	}

	if (ProfileID != UProfileIDController::InvalidID && ensure(IsValid(Profile)))
	{
		RemoveReplicatedSubObject(Profile);
	}
}

USimProfileBase* UProfileComponent::CreateNewProfile_Implementation()
{
	if (!ensureMsgf(
		IsValid(ProfileBase),
		TEXT("ProfileComponent [%s] inside [%s] has no ProfileBase set!"),
		*GetName(),
		*GetOwner()->GetName()))
	{
		return nullptr;
	}
	return NewObject<USimProfileBase>(GetWorld(), ProfileBase->GetClass(), NAME_None, RF_NoFlags, ProfileBase);
}


// Called every frame
void UProfileComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UProfileComponent::SetProfileID(const FSimProfileID& NewProfileID)
{
	ProfileID = NewProfileID;
}

USimProfileBase* UProfileComponent::InitProfile()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	auto NewProfile = CreateNewProfile();
	ProfileID = GlobalGraph->GetProfileIDsController()->RegisterProfile(NewProfile);
	NewProfile->SetProfileID(ProfileID);
	return NewProfile;
}

USimProfileBase* UProfileComponent::GetProfile()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	return GlobalGraph->GetProfileIDsController()->GetProfile(ProfileID);
}

