#pragma once

#include "CoreMinimal.h"
#include "SimulationArchetypeHandle.h"
#include "ProfileGeneratorItem.generated.h"

USTRUCT(BlueprintType)
struct FItemSetData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSimulationArchetypeHandle> Items = {};
	
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