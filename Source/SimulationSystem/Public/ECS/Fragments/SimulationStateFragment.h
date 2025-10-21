#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "SimulationLevels.h"
#include "SimulationStateFragment.generated.h"

USTRUCT()
struct FSimulationStateFragment : public FMassFragment
{
	GENERATED_BODY()

	ESimulationLevels SimulationLevel;
};
