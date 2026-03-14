// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTypes.h"
#include "SimulationTrait.generated.h"
/**
 * 
 */
UCLASS(Abstract)
class SIMULATIONSYSTEM_API USimulationTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetupDefaultTableSettings(FInstancedStruct& data);

	// This is a post-spawn init, possibly with spawning other entities
	virtual void SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity, const FInstancedStruct& OverrideData);
};
