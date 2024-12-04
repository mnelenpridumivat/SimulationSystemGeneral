// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProfileComponent.h"

#include "GlobalGraph.h"
#include "ProfileGenerator.h"
#include "ProfileIDController.h"
#include "SimProfileBase.h"
#include "SimulationFunctionLibrary.h"
#include "SimulationSystemSettings.h"
#include "Net/UnrealNetwork.h"
#include "ReplicatedSimInfo.h"


void UProfileComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UProfileComponent, ReplicatedInfo);
}

// Sets default values for this component's properties
UProfileComponent::UProfileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ProfileID = UProfileIDController::InvalidID;

	// ...
}


// Called when the game starts
void UProfileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

USimProfileBase* UProfileComponent::CreateNewProfile_Implementation()
{
	return ProfileBase; //NewObject<USimProfileBase>(GetWorld(), ProfileBase->GetClass(), NAME_None, RF_NoFlags, ProfileBase);
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
	ReplicatedInfo = GetWorld()->SpawnActor<AReplicatedSimInfo>(GetDefault<USimulationSystemSettings>()->ReplicatedSimInfoClass);
	GetProfile()->SetupReplicatedData(ReplicatedInfo);
}

USimProfileBase* UProfileComponent::InitProfile()
{
	/*FName Layer;
	FVector OwnerLocation;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit,
		OwnerLocation+FVector(0.0f, 0.0f, 10.0f),
		OwnerLocation-FVector(0.0f, 0.0f, 10.0f),
		GetMutableDefault<USim>()
		)*/
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	auto Profile = CreateNewProfile();
	ProfileID = GlobalGraph->GetProfileIDsController()->RegisterProfile(Profile);
	return Profile;
}

USimProfileBase* UProfileComponent::GetProfile()
{
	auto GlobalGraph = USimulationFunctionLibrary::GetGlobalGraph(GetWorld());
	return GlobalGraph->GetProfileIDsController()->GetProfile(ProfileID);
}

AReplicatedSimInfo* UProfileComponent::GetReplicatedInfo()
{
	return ReplicatedInfo;
}

/*USimProfileBase* UProfileComponent::CreateNewProfile()
{
	ensureMsgf(ConstructProfile.IsBound(),
		TEXT("Unable to create new profile for actor [%s] because to function is bound to UProfileComponent::ConstructProfile"),
		*GetOwner()->GetName());
	USimProfileBase* NewProfile = ConstructProfile.Execute();
	if(GetWorld()->WorldType == EWorldType::Editor)
	{
		return NewProfile;
	}
	
}

USimProfileBase* UProfileComponent::GetProfile()
{
	if(ProfileID==UProfileIDController::InvalidID)
	{
		ensureMsgf(ConstructProfile.IsBound(),
			TEXT("Unable to create new profile for actor [%s] because to function is bound to UProfileComponent::ConstructProfile"),
			*GetOwner()->GetName());
		USimProfileBase* NewProfile = ConstructProfile.Execute();
		USimulationFunctionLibrary::GetGlobalGraph(GetWorld())->AddProfileOnGraph()
		ProfileID = NewProfile->GetProfileID();
	}
}*/

