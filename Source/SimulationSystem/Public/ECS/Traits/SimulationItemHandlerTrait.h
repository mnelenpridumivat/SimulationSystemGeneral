// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutoKeyRegistration.h"
#include "SimulationTableTrait.h"
#include "SimulationItemHandlerTrait.generated.h"

SIMULATION_DATATABLE_KEY_REGISTER(Item);

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationItemHandlerTrait : public USimulationTableTrait
{
	GENERATED_BODY()
	
public:
	USimulationItemHandlerTrait();
	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	virtual void SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity, const FSimulationTraitOverrides& OverrideData) override;

};
