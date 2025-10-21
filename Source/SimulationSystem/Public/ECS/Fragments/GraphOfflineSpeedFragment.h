#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "GraphOfflineSpeedFragment.generated.h"

USTRUCT()
struct FGraphOfflineSpeedFragment : public FMassFragment
{
	GENERATED_BODY()

	float Speed = 0.0f;
};