#include "Tables/SimulationArchetypeSettings.h"

#include "MassEntityTraitBase.h"
#include "SimulationArchetype.h"
#include "SimulationTableTrait.h"
#include "SimulationTrait.h"

void FSimulationArchetypeSettings::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	FTableRowBase::OnDataTableChanged(InDataTable, InRowName);

	if (Archetype.IsNull())
	{
		Overrides.Empty();
		return;
	}

	auto Traits = Archetype.LoadSynchronous()->GetConfig().GetTraits();
	TSet<TSubclassOf<USimulationTableTrait>> OldTraits;
	Overrides.GetKeys(OldTraits);

	for (auto Trait : Traits)
	{
		if (!IsValid(Trait))
		{
			continue;
		}
		auto TraitClass = Trait->GetClass();
		if (!TraitClass->IsChildOf(USimulationTableTrait::StaticClass()))
		{
			continue;
		}
		auto CastedTrait = Cast<USimulationTableTrait>(Trait);
		if (!OldTraits.Contains(TraitClass))
		{
			auto& Value = Overrides.Add(TraitClass);
			CastedTrait->SetupDefaultTableSettings(Value);
		} else
		{
			OldTraits.Remove(TraitClass);
		}
	}

	for (auto Trait : OldTraits)
	{
		Overrides.Remove(Trait);
	}
	
}
