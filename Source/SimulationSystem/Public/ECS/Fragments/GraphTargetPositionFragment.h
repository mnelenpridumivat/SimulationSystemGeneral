#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "SimVertexID.h"
#include "GraphTargetPositionFragment.generated.h"

USTRUCT(BlueprintType)
struct FGraphTargetPositionFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FSimVertexID TargetPosition = FSimVertexID::Invalid;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FSimVertexID> Way = {};
	
	UPROPERTY(BlueprintReadOnly)
	float MovedDistance = 0.0f;
};