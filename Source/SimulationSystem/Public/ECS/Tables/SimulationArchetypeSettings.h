#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"

#include "SimulationArchetypeSettings.generated.h"

class UActionPlanner;
class USimulationTrait;
class USimulationArchetype;

USTRUCT(BlueprintType)
struct FSimulationArchetypeSettings : public FTableRowBase
{
	GENERATED_BODY()
	
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USimulationArchetype> Archetype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<USimulationTrait>, FInstancedStruct> Overrides;
};