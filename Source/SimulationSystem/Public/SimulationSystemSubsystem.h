// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProfileGenerator.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SimulationSystemSubsystemStructs.h"
#include "SimulationSystemSubsystem.generated.h"

class USimProfileBase;
class UProfileGenerator;
class USimulationSystemFunctionsImplementation;

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationSystemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	USimulationSystemFunctionsImplementation* SimulationSystemFunctions = nullptr;

	UPROPERTY()
	TMap<TSubclassOf<UProfileGenerator>, TObjectPtr<UProfileGenerator>> Generators = {};
	FCriticalSection GeneratorMutex;

	UPROPERTY()
	TMap<FName, FPawnsOfClass> ClassComposedData = {};

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	USimulationSystemFunctionsImplementation* GetSimulationSystemFunctions() const {return SimulationSystemFunctions;}
	USimProfileBase* ExecuteGenerator(UObject* Context, const FGeneratorHandleBase& handle);
};
