#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "SimVertexID.h"
#include "GraphPositionFragment.generated.h"

USTRUCT(BlueprintType)
struct FGraphPositionFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FSimVertexID Position = FSimVertexID::Invalid;

	//Test
	UPROPERTY(BlueprintReadOnly)
	bool WantWalk = true;
};
