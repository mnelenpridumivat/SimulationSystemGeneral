#pragma once

#include "CoreMinimal.h"
#include "GraphWay.h"
#include "MassEntityTypes.h"
#include "SimVertexID.h"
#include "GraphTargetPositionFragment.generated.h"

USTRUCT(BlueprintType)
struct FGraphTargetPositionFragment : public FMassFragment
{
	GENERATED_BODY()

	FGraphTargetPositionFragment() = default;
	FGraphTargetPositionFragment(const FGraphWay& OtherWay);
	FGraphTargetPositionFragment& operator=(const FGraphWay& OtherWay);
	
	UPROPERTY(BlueprintReadOnly)
	FGraphWayIDArray Way = {};

	UPROPERTY(BlueprintReadOnly)
	int CurrentPosition = 0;
	
	UPROPERTY(BlueprintReadOnly)
	float MovedDistance = 0.0f;
};