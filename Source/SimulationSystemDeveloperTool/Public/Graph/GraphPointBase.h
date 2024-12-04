// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimVertexID.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GraphPointBase.generated.h"

UCLASS(Config=Editor, defaultconfig)
class SIMULATIONSYSTEMDEVELOPERTOOL_API AGraphPointBase : public AActor
{
public:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraphPointBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	FSimVertexID ID;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVertexID(FSimVertexID NewID);
	FSimVertexID GetVertexID();
};
