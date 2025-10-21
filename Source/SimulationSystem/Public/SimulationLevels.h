#pragma once
#include "CoreMinimal.h"
#include "SimulationLevels.generated.h"

UENUM(BlueprintType)
enum class ESimulationLevels : uint8
{
	Online UMETA(DisplayName="Online"),
	Buffered UMETA(DisplayName="Buffered"),
	Offline UMETA(DisplayName="Offline"),
	MAX UMETA(Hidden)
};
