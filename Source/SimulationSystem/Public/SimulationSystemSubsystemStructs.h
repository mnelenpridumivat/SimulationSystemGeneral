// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationSystemSubsystemStructs.generated.h"

struct FGeneratorHandleBase;
class USimProfileBase;
class UProfileGenerator;
class USimulationSystemFunctionsImplementation;

// Pawn class preprocess

USTRUCT(BlueprintType)
struct SIMULATIONSYSTEM_API FPawnsOfClass
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FName> Variations;
};

// Generators

USTRUCT()
struct SIMULATIONSYSTEM_API FGeneratorPool
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TSubclassOf<UProfileGenerator>, TObjectPtr<UProfileGenerator>> Generators = {};

	USimProfileBase* ExecuteGenerator(UObject* Context, const FGeneratorHandleBase& handle);
	
};
