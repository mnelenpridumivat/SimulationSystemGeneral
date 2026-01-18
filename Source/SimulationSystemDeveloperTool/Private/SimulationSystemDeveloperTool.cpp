#include "SimulationSystemDeveloperTool.h"

#define LOCTEXT_NAMESPACE "FSimulationSystemDeveloperToolModule"

void FSimulationSystemDeveloperToolModule::StartupModule()
{
	Factory = MakeShared<FFragmentExposeNodeFactory>();
	FEdGraphUtilities::RegisterVisualNodeFactory(Factory);
}

void FSimulationSystemDeveloperToolModule::ShutdownModule()
{
    if (Factory.IsValid())
    {
	    FEdGraphUtilities::UnregisterVisualNodeFactory(Factory);
    	Factory.Reset();
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSimulationSystemDeveloperToolModule, SimulationSystemDeveloperTool)