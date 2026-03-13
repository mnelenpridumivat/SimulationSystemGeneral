// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "FragmentAddedObserver.h"
#include "FragmentsInitChecker.h"
#include "MassEntityTypes.h"
#include "RegisteredInitFragments.h"
#include "Base/GraphTargetPositionFragment.h"
#include "Subsystems/EngineSubsystem.h"
#include "SimulationFragmentInitSubsystem.generated.h"

USTRUCT()
struct FEntityFragmentKey
{
	GENERATED_BODY()

	UPROPERTY()
	FMassEntityHandle Entity;

	UPROPERTY()
	UScriptStruct* Type;

	bool operator==(const FEntityFragmentKey& Other) const
	{
		return Entity == Other.Entity && Type == Other.Type;
	}
};

FORCEINLINE uint32 GetTypeHash(const FEntityFragmentKey& Key)
{
	return HashCombine(GetTypeHash(Key.Entity), GetTypeHash(Key.Type));
}

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API USimulationFragmentInitSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	using InitFunc = TFunction<void(FMassEntityHandle)>;

	TMap<FEntityFragmentKey, InitFunc> DefferedInits;
	
	void BindDefferedInit(FEntityFragmentKey Key, InitFunc Func);

public:

	void CallInitIfCan(FEntityFragmentKey Key);

	template<typename TFragment>
	void VerifiedBindDefferedInit(FMassEntityHandle Key, InitFunc Func)
	{
		static_assert(TIsInTypeList<TFragment, FRegisteredInitFragments>::Value,
		"Fragment used with special init logic must be registered in FRegisteredInitFragments typelist (FragmentAddedObserver.h)!");
		BindDefferedInit({Key, TFragment::StaticStruct()}, Func);
	}
};
