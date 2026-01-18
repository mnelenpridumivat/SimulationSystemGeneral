// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationTrait.h"
#include "SimulationAITrait.generated.h"

class UActionPlanner;
/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationAITrait : public USimulationTrait
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UActionPlanner> DefaultAIPlanner = nullptr;
	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	//virtual void SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity, const FSimulationTraitOverrides& OverrideData) override;

};
