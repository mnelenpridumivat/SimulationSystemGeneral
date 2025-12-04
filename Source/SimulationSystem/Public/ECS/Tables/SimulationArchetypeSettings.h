#pragma once

#include "CoreMinimal.h"

#include "SimulationArchetypeSettings.generated.h"

class USimulationTableTrait;
class USimulationArchetype;

USTRUCT(BlueprintType)
struct FSimulationTraitOverrides
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<USimulationTableTrait> Trait;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanBeRandom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "CanBeRandom"))
	bool UseRandom = false;
};

USTRUCT(BlueprintType)
struct FSimulationArchetypeSettings : public FTableRowBase
{
	GENERATED_BODY()
	
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USimulationArchetype> Archetype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<USimulationTableTrait>, FSimulationTraitOverrides> Overrides;
};