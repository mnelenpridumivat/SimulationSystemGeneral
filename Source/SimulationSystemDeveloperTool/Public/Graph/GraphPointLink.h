// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalGraph.h"
#include "GraphPointBase.h"
#include "GraphPointLink.generated.h"

UCLASS()
class SIMULATIONSYSTEMDEVELOPERTOOL_API AGraphPointLink : public AGraphPointBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraphPointLink();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	const FString GetChunkName();
	bool ConnectsChunk(AGraphAsset* GraphAsset);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<AGraphAsset*> ChunksToConnect;
};
