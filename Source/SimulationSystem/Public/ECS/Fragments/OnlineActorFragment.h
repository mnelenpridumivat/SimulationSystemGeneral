#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "OnlineActorFragment.generated.h"

USTRUCT()
struct FOnlineActorFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Actor = nullptr;
	
};
