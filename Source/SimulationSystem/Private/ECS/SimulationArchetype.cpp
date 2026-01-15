// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationArchetype.h"

#include "SimulationDebugTrait.h"

#if WITH_EDITOR
void USimulationArchetype::VerifyDebugTrait()
{
	if (!FindTrait(USimulationDebugTrait::StaticClass()))
	{
		auto Trait = Cast<USimulationDebugTrait>(AddTrait(USimulationDebugTrait::StaticClass()));
		if (ensure(Trait))
		{
			Trait->SetArchetype(this);
		}
	}
}
#endif

USimulationArchetype::USimulationArchetype()
{
}

#if WITH_EDITOR
void USimulationArchetype::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	VerifyDebugTrait();
}

void USimulationArchetype::PostLoad()
{
	Super::PostLoad();
	VerifyDebugTrait();
}
#endif
