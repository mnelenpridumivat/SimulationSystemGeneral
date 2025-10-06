// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProfileGenerator.h"
#include "SimulationSystemSubsystemStructs.h"
#include "SimulationSystemSubsystem.generated.h"

class AGlobalGraph;
class USimProfileBase;
class UProfileGenerator;
class USimulationSystemFunctionsImplementation;

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationSystemSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	USimulationSystemFunctionsImplementation* SimulationSystemFunctions = nullptr;

	UPROPERTY()
	FGeneratorPool GeneratorPool;
	FCriticalSection GeneratorMutex;

	UPROPERTY()
	TMap<FName, FPawnsOfClass> ClassComposedData = {};

	UPROPERTY()
	AGlobalGraph* GlobalGraph = nullptr; // use to store GlobalGraph ref to avoid searching across all object if we need to access it

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	USimulationSystemFunctionsImplementation* GetSimulationSystemFunctions() const {return SimulationSystemFunctions;}
	USimProfileBase* ExecuteGenerator(UObject* Context, const FGeneratorHandleBase& handle);

	void GetAllPawnClasses(TArray<FName>& OutPawnClasses);
	FName GetRandomNPCOfClass(FName NPCClass);

	void SetGlobalGraph(AGlobalGraph* GraphPtr);
	AGlobalGraph* GetGlobalGraph() const;
};
