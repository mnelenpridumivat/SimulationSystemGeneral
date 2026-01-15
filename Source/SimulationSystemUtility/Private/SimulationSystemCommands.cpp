#include "SimulationSystemCommands.h"

#define LOCTEXT_NAMESPACE "FSimilationSystemModule"

void FSimulationSystemCommands::RegisterCommands()
{
	UI_COMMAND(RebuildAllAction, "RebuildAll", "Rebuild All", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control|EModifierKey::Shift|EModifierKey::Alt, EKeys::B));
	UI_COMMAND(RebuildSpawnAction, "RebuildSpawn", "Rebuild Spawn", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control|EModifierKey::Shift|EModifierKey::Alt, EKeys::S));
}

const FSimulationSystemCommands& FSimulationSystemCommands::Get()
{
	return TCommands<FSimulationSystemCommands>::Get();
}

#undef LOCTEXT_NAMESPACE
