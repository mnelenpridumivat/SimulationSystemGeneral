#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "RebuildAllStyle.h"

class FRebuildAllAddCommands : public TCommands<FRebuildAllAddCommands>
{
public:

	FRebuildAllAddCommands():
		TCommands<FRebuildAllAddCommands>(
			TEXT("RebuildAll"),
			NSLOCTEXT("Contexts", "SimulationPlugin", "SimulationSystem Plugin"),
			NAME_None,
			FRebuildAllStyle::GetStyleSetName()
		)
	{
		
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > RebuildAllAction;
	
};
