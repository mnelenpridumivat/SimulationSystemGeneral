#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "OnlineActorFragment.generated.h"

USTRUCT(BlueprintType)
struct FOnlineActorFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* Actor = nullptr;
	
};
