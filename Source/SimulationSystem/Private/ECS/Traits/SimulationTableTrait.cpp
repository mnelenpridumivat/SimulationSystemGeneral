// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationTableTrait.h"

#include "Tables/SimulationArchetypeSettings.h"

void USimulationTableTrait::SetupDefaultTableSettings_Implementation(FSimulationTraitOverrides& data)
{
	data.CanBeRandom = CanBeRandom();
	data.Trait = GetClass();
	data.RowName = DefaultRowName;
	data.UseRandom = DefaultUseRandom;
}

void USimulationTableTrait::SetupEntity(UObject* Context, FMassEntityManager& Manager, FMassEntityHandle Entity, const FSimulationTraitOverrides& OverrideData)
{
}

#if WITH_EDITOR
TArray<FName> USimulationTableTrait::GetRowNameOptions()
{
	if (!ensure(CanBeRandom() || !DefaultUseRandom))
	{
		return TArray<FName>();
	}
	if (DefaultUseRandom)
	{
		if (IsValid(RandomsDataTable))
		{
			return RandomsDataTable->GetRowNames();
		}
	} else
	{
		if (IsValid(DefaultsDataTable))
		{
			return DefaultsDataTable->GetRowNames();
		}
	}
	return TArray<FName>();
}
#endif
