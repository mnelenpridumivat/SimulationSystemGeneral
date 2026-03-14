// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimulationTrait.h"
#include "AutoKeyRegistration.h"
#include "ProfileGeneratorSquad.h"
#include "SimulationNPCHandlerTrait.generated.h"

SIMULATION_DATATABLE_KEY_REGISTER(NPCs);

USTRUCT(BlueprintType)
struct FSimulationTraitOverridesNPC
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FSimulationArchetypeHandle> NPCs;

	UPROPERTY(EditAnywhere, meta = (EditCondition="Random", ClampMin = "1", UIMin = "1"))
	int Min = 1;

	UPROPERTY(EditAnywhere, meta = (EditCondition="Random", ClampMin = "1", UIMin = "1"))
	int Max = 1;

	UPROPERTY(EditAnywhere)
	bool Random = false;
};

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationNPCHandlerTrait : public USimulationTrait
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TArray<FSimulationArchetypeHandle> DefaultNPCs;

	UPROPERTY(EditAnywhere, meta = (EditCondition="Random", ClampMin = "1", UIMin = "1"))
	int Min = 1;

	UPROPERTY(EditAnywhere, meta = (EditCondition="Random", ClampMin = "1", UIMin = "1"))
	int Max = 1;

	UPROPERTY(EditAnywhere)
	bool Random = false;
	
	
public:	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	virtual void SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity, const FInstancedStruct& OverrideData) override;
	virtual void SetupDefaultTableSettings_Implementation(FInstancedStruct& data) override;
};
