// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationBaseTrait.h"
#include "DEPRECATED_SimulationSmartTrait.generated.h"

/**
 * 
 */
UCLASS(Deprecated)
class SIMULATIONSYSTEM_API UDEPRECATED_SimulationSmartTrait : public USimulationBaseTrait
{
	GENERATED_BODY()
	
public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
