#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "OnlineActorClassFragment.generated.h"

USTRUCT(BlueprintType)
struct FOnlineActorClassFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass = nullptr;
	
};
