// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationSystemSubsystemStructs.h"
#include "UObject/Object.h"
#include "SimulationSystemFunctionsImplementation.generated.h"

class USimProfileBase;
class UAISimProfilePawn;
class UAISimProfileSquad;

/**
 * 
 */
UCLASS(Blueprintable)
class SIMULATIONSYSTEM_API USimulationSystemFunctionsImplementation : public UObject
{
	GENERATED_BODY()

public:

	/*
	 * Spawning new profile of pawn
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAISimProfilePawn* SpawnPawn(UObject* Outer, FName PawnName);

	/*
	 * Compose from data table all pawn variations for each pawn class
	 */
	UFUNCTION(BlueprintNativeEvent)
	void PreprocessPawnData(TMap<FName, FPawnsOfClass>& ComposedData);
	
};
