#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "EntityStorageFragment.generated.h"

USTRUCT()
struct FSquadStorageFragment : public FMassFragment
{
	GENERATED_BODY()

	TArray<FMassEntityHandle> Children = {};
};

USTRUCT()
struct FItemStorageFragment : public FMassFragment
{
	GENERATED_BODY()

	TArray<FMassEntityHandle> Children = {};
};

USTRUCT()
struct FNPCsStorageFragment : public FMassFragment
{
	GENERATED_BODY()

	TArray<FMassEntityHandle> Children = {};
};