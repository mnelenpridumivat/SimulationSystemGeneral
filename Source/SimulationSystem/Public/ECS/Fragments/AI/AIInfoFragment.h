#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "AIInfoFragment.generated.h"

USTRUCT()
struct FAIInfoFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	TSet<FName> Facts;
};

USTRUCT()
struct FAIInfoTransferTag : public FMassTag
{
	GENERATED_BODY()
};
