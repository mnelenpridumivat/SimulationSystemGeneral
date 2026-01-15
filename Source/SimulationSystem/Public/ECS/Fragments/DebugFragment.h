#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "DebugFragment.generated.h"

class USimulationArchetype;

USTRUCT()
struct FDebugFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	TSoftObjectPtr<USimulationArchetype> Archetype = nullptr;
};
