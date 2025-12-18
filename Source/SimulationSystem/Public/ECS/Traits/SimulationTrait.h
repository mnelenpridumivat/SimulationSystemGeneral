// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTypes.h"
#include "SimulationTrait.generated.h"

struct FSimulationTraitOverrides;
/**
 * 
 */
UCLASS(Abstract)
class SIMULATIONSYSTEM_API USimulationTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:

	/*UFUNCTION(BlueprintCallable)
	virtual bool IsTableBasedTrait() { return false; }

	UFUNCTION(BlueprintNativeEvent)
	void SetupDefaultTableSettings(FSimulationTraitOverrides& data);*/
};
