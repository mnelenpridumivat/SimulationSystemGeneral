#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "SimulationLevels.h"
#include "SimulationStateFragment.generated.h"

USTRUCT(BlueprintType)
struct FSimulationStateFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	ESimulationLevels SimulationLevel;
};
