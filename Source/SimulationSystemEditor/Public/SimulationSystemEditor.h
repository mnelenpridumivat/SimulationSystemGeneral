#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimulationSystemEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
	
	/*virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() const override { return MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() const override { return ToolBarExtensibilityManager; }

private:
    void AddToolbarExtension(FToolBarBuilder& ToolBarBuilder);
    void RebuildAllButtonClicked();*/

private:
    void SetupSettings();
    void UnsetupSettings();
	
	/*FSlateIcon GetLevelToolbarIcon() const;
	
	void BindCommands();
	void ExtendLevelEditorToolbar(FToolBarBuilder& ToolbarBuilder);
	TSharedRef<SWidget> GenerateMenuContent() const;
	
	static void RebuildAll();

	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
    TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<FExtender> ToolbarExtender;*/
};
