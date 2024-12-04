#include "..\Public\RebuildAllAddCommands.h"

#define LOCTEXT_NAMESPACE "FSimilationSystemModule"

void FRebuildAllAddCommands::RegisterCommands()
{
	UI_COMMAND(RebuildAllAction, "RebuildAll", "Rebuild All", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control|EModifierKey::Shift|EModifierKey::Alt, EKeys::B));
}

#undef LOCTEXT_NAMESPACE
