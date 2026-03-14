#pragma once

#include "CoreMinimal.h"
#include "SimulationArchetypeHandle.h"
#include "ProfileGeneratorItem.generated.h"

USTRUCT(BlueprintType)
struct FItemSetElemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSimulationArchetypeHandle Item = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(HideInDetailPanel, ClampMin="0", UIMin="0"))
	int AmountMin = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(HideInDetailPanel, ClampMin="0", UIMin="0"))
	int AmountMax = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(HideInDetailPanel, ClampMin="0.0", ClampMax="1.0", UIMin="0", UIMax="1.0"))
	float Probability = 1.0f;
	
};

USTRUCT(BlueprintType)
struct FItemSetData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemSetElemData> Items = {};
	
};

USTRUCT(BlueprintType)
struct FItemSetHandler
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RowName;
};

class UProfileGenerator;

USTRUCT(BlueprintType)
struct FProfileGeneratorItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UProfileGenerator> Generator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(HideInDetailPanel, ClampMin="0", UIMin="0"))
	int AmountMin = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(HideInDetailPanel, ClampMin="0", UIMin="0"))
	int AmountMax = 1;
	
};