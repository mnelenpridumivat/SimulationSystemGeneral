#pragma once

#include "CoreMinimal.h"
#include "SimulationSystemProfileType.generated.h"

UENUM(BlueprintType)
enum class ESimualtionSystemProfileType : uint8
{
	Classes, // Old profile types, created in OOP style.
	ECS, // New profile types, created using ECS
	MAX // Invalid value
};