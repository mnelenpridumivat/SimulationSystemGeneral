// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphSerialized.h"
#include "Engine/DataAsset.h"
#include "GraphDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UGraphDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	FGraphSerialized Graph;
	
	UPROPERTY(VisibleAnywhere)
	FProfilesSerialized Profiles;
};
