// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationTableTrait.h"
#include "AutoKeyRegistration.h"
#include "SimulationSquadHandlerTrait.generated.h"

SIMULATION_DATATABLE_KEY_REGISTER(Squad);

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationSquadHandlerTrait : public USimulationTableTrait
{
	GENERATED_BODY()
	
public:
	USimulationSquadHandlerTrait();
	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	virtual void SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity, const FSimulationTraitOverrides& OverrideData) override;

};
