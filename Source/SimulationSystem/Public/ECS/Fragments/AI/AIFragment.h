#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "AIFragment.generated.h"

class UActionPlanner;

USTRUCT()
struct FAIFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UActionPlanner> ActionPlanner;
};
