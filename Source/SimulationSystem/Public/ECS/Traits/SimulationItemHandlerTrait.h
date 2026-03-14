// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutoKeyRegistration.h"
#include "ProfileGeneratorItem.h"
#include "SimulationTrait.h"
#include "SimulationItemHandlerTrait.generated.h"

SIMULATION_DATATABLE_KEY_REGISTER(Item);
SIMULATION_DATATABLE_KEY_REGISTER(ItemRandom);
SIMULATION_DATATABLE_KEY_REGISTER(ItemSet);

USTRUCT(BlueprintType)
struct FSimulationTraitOverridesItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(EditCondition="!TableValue"))
	FItemSetData Items;

	UPROPERTY(EditAnywhere, meta=(EditCondition="TableValue"))
	FItemSetHandler Row;

	UPROPERTY(EditAnywhere)
	bool TableValue;
};

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationItemHandlerTrait : public USimulationTrait
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, meta=(EditCondition="!TableValue"))
	FItemSetData Items;

	UPROPERTY(EditAnywhere, meta=(EditCondition="TableValue"))
	FItemSetHandler Row;

	UPROPERTY(EditAnywhere)
	bool TableValue;
	
public:

	virtual void SetupDefaultTableSettings_Implementation(FInstancedStruct& data) override;
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	virtual void SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity, const FInstancedStruct& OverrideData) override;

};
