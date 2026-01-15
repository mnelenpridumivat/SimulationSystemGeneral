// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityConfigAsset.h"
#include "SimulationArchetype.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationArchetype : public UMassEntityConfigAsset
{
	GENERATED_BODY()

#if WITH_EDITOR
	void VerifyDebugTrait();
#endif
 
public:
	USimulationArchetype();
 
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostLoad() override;
#endif
};
