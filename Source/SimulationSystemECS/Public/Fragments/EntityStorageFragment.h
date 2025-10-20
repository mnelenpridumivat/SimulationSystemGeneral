#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "EntityStorageFragment.generated.h"

USTRUCT()
struct FEntityStorageFragment : public FMassFragment
{
	GENERATED_BODY()

	TArray<FMassEntityHandle> Children = {};
};