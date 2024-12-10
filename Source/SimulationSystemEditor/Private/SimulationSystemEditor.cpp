#include "SimulationSystemEditor.h"

#include "ISettingsModule.h"
#include "LevelEditor.h"
//#include "RebuildAllStyle.h"
#include "SimulationSystemDeveloperSettings.h"
#include "SimulationSystemEditorFunctionLibrary.h"
//#include "SimulationSystemCommands.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FSimulationSystemEditorModule"

void FSimulationSystemEditorModule::StartupModule()
{
	SetupSettings();
	//FRebuildAllStyle::Initialize();
	//FRebuildAllStyle::ReloadTextures();

	//FSimulationSystemCommands::Register();
	
	/*RebuildAllCommands = MakeShareable(new FUICommandList);
	
	RebuildAllCommands->MapAction(
		FRebuildAllAddCommands::Get().RebuildAllAction,
		FExecuteAction::CreateRaw(this, &FSimulationSystemEditorModule::RebuildAllButtonClicked),
		FCanExecuteAction());
	
	TSharedPtr<FExtender> MyExtender = MakeShareable(new FExtender);
	MyExtender->AddToolBarExtension(
		"Compile",
		EExtensionHook::After,
		RebuildAllCommands,
	FToolBarExtensionDelegate::CreateRaw(this, &FSimulationSystemEditorModule::AddToolbarExtension));*/
	
	/*PluginCommands = MakeShared<FUICommandList>();
	BindCommands();

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	ToolbarExtender = MakeShared<FExtender>();
	ToolbarExtender->AddToolBarExtension("Content",
		EExtensionHook::After,
		PluginCommands,
		FToolBarExtensionDelegate::CreateRaw(this, &FSimulationSystemEditorModule::ExtendLevelEditorToolbar));
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);*/
}

void FSimulationSystemEditorModule::UnsetupSettings()
{
	//FSimulationSystemCommands::Unregister();
	
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Editor", "Plugins", "SimulationSystemDeveloper");
		//SettingsModule->UnregisterSettings("Project", "Plugins", "LogicDriverEditor");
		//SettingsModule->UnregisterSettings("Project", "Plugins", "LogicDriverRuntime");
	}
}

/*FSlateIcon FSimulationSystemEditorModule::GetLevelToolbarIcon() const
{
	return FSlateIcon(TEXT("SMUtilityLauncherStyle"), "SMUtilityLauncherErrorIcon");
}*/

/*void FSimulationSystemEditorModule::BindCommands()
{
	const FSimulationSystemCommands& Commands = FSimulationSystemCommands::Get();
	PluginCommands->MapAction(Commands.RebuildAllAction, FExecuteAction::CreateStatic(&FSimulationSystemEditorModule::RebuildAll));
}*/

/*void FSimulationSystemEditorModule::ExtendLevelEditorToolbar(FToolBarBuilder& ToolbarBuilder)
{
	TAttribute<FSlateIcon> IconGetter =
		TAttribute<FSlateIcon>::Create(TAttribute<FSlateIcon>::FGetter::CreateRaw(this,
			&FSimulationSystemEditorModule::GetLevelToolbarIcon));
	
	ToolbarBuilder.AddComboButton(FUIAction(),
						FOnGetContent::CreateRaw(this, &FSimulationSystemEditorModule::GenerateMenuContent),
						LOCTEXT("SimulationSystemToolbarMenu_Label", "Simulation System"),
						LOCTEXT("SimulationSystemMenu_Tooltip", "Simulation System utilities."),
						IconGetter);
}*/

/*TSharedRef<SWidget> FSimulationSystemEditorModule::GenerateMenuContent() const
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder(bShouldCloseWindowAfterMenuSelection, PluginCommands, GetMenuExtensibilityManager()->GetAllExtenders());

	MenuBuilder.BeginSection(TEXT("LogicDriver"), LOCTEXT("LogicDriverHeading", "Logic Driver"));
	{
		MenuBuilder.AddMenuEntry(LOCTEXT("License_Name", "License"),
			LOCTEXT("License_Tooltip", "View Logic Driver license information."), 
			GetLicenseIcon(),
			FExecuteAction::CreateStatic(&FSMUtilityLauncherModule::OpenLicenseInformation));
		
		const FSlateIcon SupportIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.VisitSupportWebSite");
		MenuBuilder.AddSubMenu(
			LOCTEXT("OpenLogicDriverSupportSubMenu", "Support"),
			LOCTEXT("OpenLogicDriverSupportSubMenu_ToolTip", "View support options for Logic Driver."),
			FNewMenuDelegate::CreateStatic(&FSMUtilityLauncherModule::MakeSupportMenu),
			false,
			SupportIcon);
	}
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection("BlueprintClass", LOCTEXT("BlueprintClassHeading", "Blueprint Class"));
	{
		MenuBuilder.AddMenuEntry(FSMUtilityLauncherCommands::Get().CreateStateMachineClass, NAME_None,
			TAttribute<FText>(), TAttribute<FText>(),
			FSlateIcon(FSMEditorStyle::Get()->GetStyleSetName(), TEXT("ClassIcon.SMInstance")));

		MenuBuilder.AddMenuEntry(FSMUtilityLauncherCommands::Get().CreateNodeClass, NAME_None,
			TAttribute<FText>(), TAttribute<FText>(),
			FSlateIcon(FSMEditorStyle::Get()->GetStyleSetName(), TEXT("ClassIcon.SMNodeInstance")));

		// Open an existing Blueprint Class...
		const FSlateIcon OpenBPIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.OpenClassBlueprint");
		MenuBuilder.AddSubMenu(
			LOCTEXT("OpenLogicDriverBlueprintClassSubMenu", "Open Logic Driver Class"),
			LOCTEXT("OpenLogicDriverBlueprintClassSubMenu_ToolTip", "Open an existing LogicDriver Blueprint Class in this project."),
			FNewMenuDelegate::CreateStatic(&FSMUtilityLauncherModule::MakeOpenBlueprintClassMenu),
			false,
			OpenBPIcon);
	}
	MenuBuilder.EndSection();
	
	MenuBuilder.BeginSection(TEXT("Tools"), LOCTEXT("LogicDriverToolsHeading", "Tools"));
	{
		// Add a null widget so the section is created and can be extended from other modules.
		MenuBuilder.AddWidget(SNullWidget::NullWidget, FText::GetEmpty());
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}*/

/*void FSimulationSystemEditorModule::RebuildAll()
{
}*/

void FSimulationSystemEditorModule::ShutdownModule()
{
	UnsetupSettings();
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	//FRebuildAllStyle::Shutdown();
	/*FSimulationSystemCommands::Unregister();
	if (ToolbarExtender.IsValid())
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
		LevelEditorModule.GetToolBarExtensibilityManager()->RemoveExtender(ToolbarExtender);
		ToolbarExtender.Reset();
	}*/
    
}

/*void FSimulationSystemEditorModule::AddToolbarExtension(FToolBarBuilder& ToolBarBuilder)
{
	ToolBarBuilder.AddToolBarButton(FSimulationSystemCommands::Get().RebuildAllAction);
	
}*/

/*void FSimulationSystemEditorModule::RebuildAllButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Rebuild All clicked!"));
	USimulationSystemEditorFunctionLibrary::FullRebuild();
}*/

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