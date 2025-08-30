// Fill out your copyright notice in the Description page of Project Settings.


#include "DefSimActor.h"

#include "ProfileComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ADefSimActor::ADefSimActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProfileComponent = CreateDefaultSubobject<UProfileComponent>("ProfileComponent");
}

// Called when the game starts or when spawned
void ADefSimActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefSimActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADefSimActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADefSimActor, ProfileComponent);
}

