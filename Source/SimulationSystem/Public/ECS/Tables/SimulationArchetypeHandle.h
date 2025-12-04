#pragma once

#include "CoreMinimal.h"
#include "SimulationArchetypeHandle.generated.h"

USTRUCT(BlueprintType)
struct FSimulationArchetypeHandle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
};