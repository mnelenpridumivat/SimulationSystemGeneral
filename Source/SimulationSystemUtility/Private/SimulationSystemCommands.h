#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FSimulationSystemCommands : public TCommands<FSimulationSystemCommands>
{
public:
	FSimulationSystemCommands(): TCommands<FSimulationSystemCommands>(TEXT("SimSystemUtilityLauncher"), NSLOCTEXT("Contexts", "SimSystemUtilityLauncher", "Simulation System Tools"),
	NAME_None, FAppStyle::Get().GetStyleSetName()){}

	/*FSimulationSystemCommands():
		TCommands<FSimulationSystemCommands>(
			TEXT("RebuildAll"),
			NSLOCTEXT("Contexts", "SimulationPlugin", "SimulationSystem Plugin"),
			NAME_None,
			FSimulationSystemStyle::GetStyleSetName()
		)
	{
		
	}*/

	// TCommands<> interface
	virtual void RegisterCommands() override;
	FORCENOINLINE static const FSimulationSystemCommands& Get();

	TSharedPtr< FUICommandInfo > RebuildAllAction;
	
};
