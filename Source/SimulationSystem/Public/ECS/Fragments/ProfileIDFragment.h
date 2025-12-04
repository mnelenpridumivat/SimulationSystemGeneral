#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "SimProfileID.h"
#include "ProfileIDFragment.generated.h"

USTRUCT()
struct FProfileIDFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	FSimProfileID ProfileID;
	
};