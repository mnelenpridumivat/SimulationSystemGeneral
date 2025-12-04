#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "OnlineActorClassFragment.generated.h"

USTRUCT()
struct FOnlineActorClassFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AActor> ActorClass = nullptr;
	
};
