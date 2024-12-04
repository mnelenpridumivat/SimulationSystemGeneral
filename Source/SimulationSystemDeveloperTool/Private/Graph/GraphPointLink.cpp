// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GraphPointLink.h"


// Sets default values
AGraphPointLink::AGraphPointLink()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGraphPointLink::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGraphPointLink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FString AGraphPointLink::GetChunkName()
{
	return "";
}

bool AGraphPointLink::ConnectsChunk(AGraphAsset* GraphAsset)
{
	return ChunksToConnect.Contains(GraphAsset);
}

