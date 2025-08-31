// Fill out your copyright notice in the Description page of Project Settings.


#include "SimPlayerStart.h"

#include "ProfileComponent.h"


void ASimPlayerStart::DirectSetGraphVertex_Implementation(const FSimVertexID& SimVertexID)
{
	SpawnVertexID = SimVertexID;
}

// Sets default values
ASimPlayerStart::ASimPlayerStart(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASimPlayerStart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimPlayerStart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

