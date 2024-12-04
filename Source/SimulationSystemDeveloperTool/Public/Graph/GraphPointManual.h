// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphPointBase.h"
#include "GraphPointManual.generated.h"

UCLASS()
class SIMULATIONSYSTEMDEVELOPERTOOL_API AGraphPointManual : public AGraphPointBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraphPointManual();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
