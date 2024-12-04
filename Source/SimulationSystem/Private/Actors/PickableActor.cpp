// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PickableActor.h"

#include "ProfileGenerator.h"
#include "SimProfileBase.h"


UProfileComponent* APickableActor::GetProfileComponent_Implementation()
{
	return ProfileComponent;
}

// Sets default values
APickableActor::APickableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProfileComponent = CreateDefaultSubobject<UProfileComponent>("ProfileComponent");
}

// Called when the game starts or when spawned
void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

