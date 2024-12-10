#include "SimulationSystemCommands.h"

#define LOCTEXT_NAMESPACE "FSimilationSystemModule"

void FSimulationSystemCommands::RegisterCommands()
{
	UI_COMMAND(RebuildAllAction, "RebuildAll", "Rebuild All", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control|EModifierKey::Shift|EModifierKey::Alt, EKeys::B));
}

const FSimulationSystemCommands& FSimulationSystemCommands::Get()
{
	return TCommands<FSimulationSystemCommands>::Get();
}

#undef LOCTEXT_NAMESPACE
