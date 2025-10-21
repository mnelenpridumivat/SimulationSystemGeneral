// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationBaseTrait.h"
#include "SimulationSquadTrait.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationSquadTrait : public USimulationBaseTrait
{
	GENERATED_BODY()
	
public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
