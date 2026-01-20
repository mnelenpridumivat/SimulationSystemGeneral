#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "GraphOfflineSpeedFragment.generated.h"

USTRUCT(BlueprintType)
struct FGraphOfflineSpeedFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float Speed = 0.0f;
};