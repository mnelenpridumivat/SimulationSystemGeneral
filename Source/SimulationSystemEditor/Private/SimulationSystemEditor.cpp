#include "SimulationSystemEditor.h"

#include "ISettingsModule.h"
#include "LevelEditor.h"
#include "RebuildAllStyle.h"
#include "SimulationSystemDeveloperSettings.h"
#include "SimulationSystemEditorFunctionLibrary.h"
#include "../Public/RebuildAllAddCommands.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FSimulationSystemEditorModule"

void FSimulationSystemEditorModule::StartupModule()
{
	SetupSettings();
	FRebuildAllStyle::Initialize();
	FRebuildAllStyle::ReloadTextures();

	FRebuildAllAddCommands::Register();
	
	RebuildAllCommands = MakeShareable(new FUICommandList);
	
	RebuildAllCommands->MapAction(
		FRebuildAllAddCommands::Get().RebuildAllAction,
		FExecuteAction::CreateRaw(this, &FSimulationSystemEditorModule::RebuildAllButtonClicked),
		FCanExecuteAction());
	
	TSharedPtr<FExtender> MyExtender = MakeShareable(new FExtender);
	MyExtender->AddToolBarExtension(
		"Compile",
		EExtensionHook::After,
		RebuildAllCommands,
		FToolBarExtensionDelegate::CreateRaw(this, &FSimulationSystemEditorModule::AddToolbarExtension));

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(MyExtender);
}

void FSimulationSystemEditorModule::UnsetupSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Editor", "Plugins", "SimulationSystemDeveloper");
		//SettingsModule->UnregisterSettings("Project", "Plugins", "LogicDriverEditor");
		//SettingsModule->UnregisterSettings("Project", "Plugins", "LogicDriverRuntime");
	}
}

void FSimulationSystemEditorModule::ShutdownModule()
{
	UnsetupSettings();
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FRebuildAllStyle::Shutdown();
	FRebuildAllAddCommands::Unregister();
    
}

void FSimulationSystemEditorModule::AddToolbarExtension(FToolBarBuilder& ToolBarBuilder)
{
	ToolBarBuilder.AddToolBarButton(FRebuildAllAddCommands::Get().RebuildAllAction);
	/*struct Local
	{
		static void FillToolbar(FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.BeginSection("Command");
			{
				ToolbarBuilder.AddToolBarButton(FFlipbookEditorCommands::Get().AddKeyFrame);
			}
			ToolbarBuilder.EndSection();
		}
	};*/
	
}

void FSimulationSystemEditorModule::RebuildAllButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Rebuild All clicked!"));
	USimulationSystemEditorFunctionLibrary::FullRebuild();
}

void FSimulationSystemEditorModule::SetupSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Editor", "Plugins", "SimulationSystemDeveloper",
			LOCTEXT("SimulationSystemDeveloperSettingsName", "Simulation System (Developer)"),
			LOCTEXT("SimulationSystemDeveloperSettingsDescription", "Configs for development time"),
			GetMutableDefault<USimulationSystemDeveloperSettings>());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSimulationSystemEditorModule, SimulationSystemEditor)