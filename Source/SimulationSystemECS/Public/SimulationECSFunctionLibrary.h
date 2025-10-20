// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimulationECSFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEMECS_API USimulationECSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static FMassEntityManager* GetEntityManager(UObject* Context);

public:
	static FMassEntityHandle CreateSquadArchetype(UObject* Context);
	static FMassEntityHandle CreateNPCArchetype(UObject* Context);
	static FMassEntityHandle CreateSmartArchetype(UObject* Context);
	static FMassEntityHandle CreateItemArchetype(UObject* Context);
};
