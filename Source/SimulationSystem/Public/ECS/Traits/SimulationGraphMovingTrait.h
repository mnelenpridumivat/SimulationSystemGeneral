// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationTrait.h"
#include "SimulationGraphMovingTrait.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationGraphMovingTrait : public USimulationTrait
{
	GENERATED_BODY()

public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
