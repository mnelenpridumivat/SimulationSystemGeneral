// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphWay.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Profiles/SimProfileHolder.h"
#include "SimulationFunctionLibrary.generated.h"

class USimulationSystemSubsystem;
class USimulationSystemFunctionsImplementation;
class UAISimProfileSquad;
class UNavHeuristic_Base;
class USimProfileBase;
class AGlobalGraph;

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static AGlobalGraph* GetGlobalGraph(UObject* Context);

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static UProfileIDController* GetProfileIDController(UObject* Context);

	UFUNCTION(BlueprintPure)
	static void AsProfile(const FSimProfileHolder& Holder, USimProfileBase*& Profile, bool& Success);

	UFUNCTION(BlueprintPure)
	static void AsVertex(const FSimProfileHolder& Holder, FSimVertexID& VertexID, bool& Success);

	UFUNCTION(BlueprintPure)
	static FSimVertexID GetWayStart(const FGraphWay& Way);

	UFUNCTION(BlueprintPure)
	static FSimVertexID GetWayEnd(const FGraphWay& Way);

	UFUNCTION(BlueprintPure)
	static FSimVertexID GetWayCurrentGraphPoint(const FGraphWay& Way);

	UFUNCTION(BlueprintPure)
	static FSimVertexID GetWayNextGraphPoint(const FGraphWay& Way);

	UFUNCTION(BlueprintPure, meta=(WorldContext=Context))
	static FGraphWay FindWay(UObject* Context, const FSimVertexID& Start, const FSimVertexID& Dest, TSet<UNavHeuristic_Base*> Params);

	static USimulationSystemSubsystem* GetSimulationSystemSubsystem(UObject* Context);
	
	UFUNCTION(BlueprintPure, meta=(WorldContext=Context))
	static USimulationSystemFunctionsImplementation* GetFunctions(UObject* Context);

	static void SaveObjectData(UObject* Object, TArray<uint8>& Data);
	static void LoadObjectData(UObject* Object, const TArray<uint8>& Data);
	
};
