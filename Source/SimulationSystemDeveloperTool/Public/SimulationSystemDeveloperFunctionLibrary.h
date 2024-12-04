// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimulationSystemDeveloperFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMDEVELOPERTOOL_API USimulationSystemDeveloperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	static void DebugDrawGraph(UWorld* World, float ShowTime);
	
};
