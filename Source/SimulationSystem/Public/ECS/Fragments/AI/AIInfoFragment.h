#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "AIInfoFragment.generated.h"

USTRUCT()
struct FAIInfoFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FName, bool> Facts;
};