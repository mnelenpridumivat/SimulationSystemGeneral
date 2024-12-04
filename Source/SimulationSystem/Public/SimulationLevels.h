#pragma once
#include "CoreMinimal.h"
#include "SimulationLevels.generated.h"

UENUM()
enum ESimulationLevels{
	ESimulationLevels_Online UMETA(DisplayName="Online"),
	ESimulationLevels_Buffered UMETA(DisplayName="Buffered"),
	ESimulationLevels_Offline UMETA(DisplayName="Offline"),
	ESimulationLevels_MAX UMETA(Hidden)
};
