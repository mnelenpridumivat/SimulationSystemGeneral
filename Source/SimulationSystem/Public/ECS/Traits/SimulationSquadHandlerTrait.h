// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationTrait.h"
#include "AutoKeyRegistration.h"
#include "ProfileGeneratorSquad.h"
#include "SimulationSquadHandlerTrait.generated.h"

SIMULATION_DATATABLE_KEY_REGISTER(Squad);

USTRUCT(BlueprintType)
struct FSimulationTraitOverridesSquad
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FSimulationArchetypeHandle> Squads;
};

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationSquadHandlerTrait : public USimulationTrait
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSimulationArchetypeHandle> DefaultSquads;
	
public:	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	virtual void SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity, const FInstancedStruct& OverrideData) override;
	virtual void SetupDefaultTableSettings_Implementation(FInstancedStruct& data) override;
};
