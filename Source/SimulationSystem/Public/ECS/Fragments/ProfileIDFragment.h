#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "SimProfileID.h"
#include "ProfileIDFragment.generated.h"

USTRUCT(BlueprintType)
struct FProfileIDFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FSimProfileID ProfileID;
	
};