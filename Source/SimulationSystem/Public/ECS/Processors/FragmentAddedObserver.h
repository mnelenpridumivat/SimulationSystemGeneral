// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FragmentsInitChecker.h"
#include "MassProcessor.h"
#include "SimulationFragmentInitSubsystem.h"
#include "FragmentAddedObserver.generated.h"

struct FGraphTargetPositionFragment;

// Add fragments for init here
using FRegisteredInitFragments = TTypeList<FGraphTargetPositionFragment>;
//using FRegisteredInitFragments = TTypeList<>;

/**
 * 
 */
UCLASS()
class SIMULATIONSYSTEM_API UFragmentAddedObserver : public UMassProcessor
{
	GENERATED_BODY()

	// FRegisteredInitFragments unwrapping
	template<typename List>
	struct FAddRequirements;
 
	template<typename Head, typename... Tail>
	struct FAddRequirements<TTypeList<Head, Tail...>>
	{
		static void Apply(FMassEntityQuery& Query)
		{
			Query.AddRequirement<Head>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::Optional);
			FAddRequirements<TTypeList<Tail...>>::Apply(Query);
		}
		
		static void Execute(USimulationFragmentInitSubsystem* Subsystem, FMassEntityHandle Entity)
		{
			Subsystem->CallInitIfCan({Entity, Head::StaticStruct()});
			FAddRequirements<TTypeList<Tail...>>::Execute(Subsystem, Entity);
		}
	};
 
	template<>
	struct FAddRequirements<TTypeList<>>
	{
		static void Apply(FMassEntityQuery& Query) {}
		static void Execute(USimulationFragmentInitSubsystem* Subsystem, FMassEntityHandle Entity) {}
	};

public:
	UFragmentAddedObserver();

	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
	
};
