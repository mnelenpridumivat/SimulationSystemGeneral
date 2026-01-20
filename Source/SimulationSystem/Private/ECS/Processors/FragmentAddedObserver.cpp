// Fill out your copyright notice in the Description page of Project Settings.


#include "FragmentAddedObserver.h"

#include "MassExecutionContext.h"
#include "SimulationFragmentInitSubsystem.h"

UFragmentAddedObserver::UFragmentAddedObserver()
{
	bAutoRegisterWithProcessingPhases = false;
}

void UFragmentAddedObserver::ConfigureQueries()
{
	FAddRequirements<FRegisteredInitFragments>::Apply(EntityQuery);
}

void UFragmentAddedObserver::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	auto Subsystem = GEngine->GetEngineSubsystem<USimulationFragmentInitSubsystem>();
	
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [&](FMassExecutionContext& Context)
	{
		for (auto Entity : Context.GetEntities())
		{
			FAddRequirements<FRegisteredInitFragments>::Execute(Subsystem, Entity);
		}
	});
}
