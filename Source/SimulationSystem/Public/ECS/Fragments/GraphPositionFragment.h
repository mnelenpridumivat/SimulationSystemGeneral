#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "SimVertexID.h"
#include "GraphPositionFragment.generated.h"

USTRUCT()
struct FGraphPositionFragment : public FMassFragment
{
	GENERATED_BODY()

	FSimVertexID Position = FSimVertexID::Invalid;
};
