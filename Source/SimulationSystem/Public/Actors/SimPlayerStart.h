// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimVertexID.h"
#include "SimActorInterface.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/EditorSimActor.h"
#include "SimPlayerStart.generated.h"

class AGraphAsset;

UCLASS()
class SIMULATIONSYSTEM_API ASimPlayerStart : public APlayerStart, public ISimActorInterface
{
public:
	virtual bool UseInSimulation_Implementation() override {return true;}
	virtual void DirectSetGraphVertex_Implementation(const FSimVertexID& SimVertexID) override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimPlayerStart(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure)
	FORCEINLINE FSimVertexID GetSimVertexID(){return SpawnVertexID;}

	UPROPERTY(EditInstanceOnly)
	AGraphAsset* InAsset = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	FSimVertexID SpawnVertexID = FSimVertexID::Invalid;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
