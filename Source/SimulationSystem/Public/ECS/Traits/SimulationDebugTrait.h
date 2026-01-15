// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationTrait.h"
#include "SimulationDebugTrait.generated.h"

class USimulationArchetype;
/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationDebugTrait : public USimulationTrait
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	USimulationArchetype* Archetype;

public:
	inline void SetArchetype(USimulationArchetype* NewArchetype) {Archetype = NewArchetype;}
	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
