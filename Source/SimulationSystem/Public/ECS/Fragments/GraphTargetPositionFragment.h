#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "SimVertexID.h"
#include "GraphTargetPositionFragment.generated.h"

USTRUCT()
struct FGraphTargetPositionFragment : public FMassFragment
{
	GENERATED_BODY()

	FSimVertexID TargetPosition = FSimVertexID::Invalid;
	TArray<FSimVertexID> Way = {};
	float MovedDistance = 0.0f;
};