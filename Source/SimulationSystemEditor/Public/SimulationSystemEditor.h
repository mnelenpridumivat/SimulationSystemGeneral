#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimulationSystemEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void AddToolbarExtension(FToolBarBuilder& ToolBarBuilder);
    void RebuildAllButtonClicked();

    void SetupSettings();
    void UnsetupSettings();

    TSharedPtr<class FUICommandList> RebuildAllCommands;
    TSharedPtr< FUICommandInfo > RebuildAllButton;
};
