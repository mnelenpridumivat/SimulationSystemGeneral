#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "EntityStackFragment.generated.h"

USTRUCT()
struct FEntityStackFragment : public FMassFragment
{
	GENERATED_BODY()

	uint32 StackedNum = 0;
};
