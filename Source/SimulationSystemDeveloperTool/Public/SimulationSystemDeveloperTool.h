#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "FragmentExposeNodeFactory.h"

class FSimulationSystemDeveloperToolModule : public IModuleInterface
{
    TSharedPtr<FFragmentExposeNodeFactory> Factory;
    
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
