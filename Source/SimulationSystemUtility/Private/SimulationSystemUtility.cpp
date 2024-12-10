#include "SimulationSystemUtility.h"
#include "SimulationSystemCommands.h"
#include "SimulationSystemStyle.h"
#include "LevelEditor.h"
#include "SimulationSystemEditorFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "FSimulationSystemUtilityModule"

void FSimulationSystemUtilityModule::StartupModule()
{
	MenuExtensibilityManager = MakeShared<FExtensibilityManager>();
	ToolBarExtensibilityManager = MakeShared<FExtensibilityManager>();

	FSimulationSystemCommands::Register();
	FSimulationSystemStyle::Initialize();
	
	PluginCommands = MakeShared<FUICommandList>();
	BindCommands();

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	ToolbarExtender = MakeShared<FExtender>();
	ToolbarExtender->AddToolBarExtension("Content",
		EExtensionHook::After,
		PluginCommands,
		FToolBarExtensionDelegate::CreateRaw(this, &FSimulationSystemUtilityModule::ExtendLevelEditorToolbar));
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
    
}

void FSimulationSystemUtilityModule::ShutdownModule()
{

	FSimulationSystemCommands::Unregister();
	FSimulationSystemStyle::Shutdown();
    
}

void FSimulationSystemUtilityModule::AddToolbarExtension(FToolBarBuilder& ToolBarBuilder)
{
	ToolBarBuilder.AddToolBarButton(FSimulationSystemCommands::Get().RebuildAllAction);
}

/*void FSimulationSystemUtilityModule::RebuildAllButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Rebuild All clicked!"));
	USimulationSystemEditorFunctionLibrary::FullRebuild();
}*/

FSlateIcon FSimulationSystemUtilityModule::GetLevelToolbarIcon() const
{
	return FSlateIcon(TEXT("SMUtilityLauncherStyle"), "SMUtilityLauncherErrorIcon");
}

void FSimulationSystemUtilityModule::BindCommands()
{
	const FSimulationSystemCommands& Commands = FSimulationSystemCommands::Get();
	PluginCommands->MapAction(Commands.RebuildAllAction, FExecuteAction::CreateStatic(&FSimulationSystemUtilityModule::RebuildAll));
}

void FSimulationSystemUtilityModule::ExtendLevelEditorToolbar(FToolBarBuilder& ToolbarBuilder)
{
	TAttribute<FSlateIcon> IconGetter =
		TAttribute<FSlateIcon>::Create(TAttribute<FSlateIcon>::FGetter::CreateRaw(this,
			&FSimulationSystemUtilityModule::GetLevelToolbarIcon));
	
	ToolbarBuilder.AddComboButton(FUIAction(),
						FOnGetContent::CreateRaw(this, &FSimulationSystemUtilityModule::GenerateMenuContent),
						LOCTEXT("SimulationSystemToolbarMenu_Label", "Simulation System"),
						LOCTEXT("SimulationSystemMenu_Tooltip", "Simulation System utilities."),
						IconGetter);
}

TSharedRef<SWidget> FSimulationSystemUtilityModule::GenerateMenuContent() const
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder(bShouldCloseWindowAfterMenuSelection, PluginCommands, GetMenuExtensibilityManager()->GetAllExtenders());

	MenuBuilder.BeginSection("SimulationSystem", LOCTEXT("SimulationSystemHeading", "Simulation System"));
	{
		MenuBuilder.AddMenuEntry(FSimulationSystemCommands::Get().RebuildAllAction, NAME_None,
			TAttribute<FText>(), TAttribute<FText>(),
			FSlateIcon(FSimulationSystemStyle::Get().GetStyleSetName(), TEXT("ClassIcon.SMInstance")));

		/*MenuBuilder.AddMenuEntry(FSMUtilityLauncherCommands::Get().CreateNodeClass, NAME_None,
			TAttribute<FText>(), TAttribute<FText>(),
			FSlateIcon(FSMEditorStyle::Get()->GetStyleSetName(), TEXT("ClassIcon.SMNodeInstance")));

		// Open an existing Blueprint Class...
		const FSlateIcon OpenBPIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.OpenClassBlueprint");
		MenuBuilder.AddSubMenu(
			LOCTEXT("OpenLogicDriverBlueprintClassSubMenu", "Open Logic Driver Class"),
			LOCTEXT("OpenLogicDriverBlueprintClassSubMenu_ToolTip", "Open an existing LogicDriver Blueprint Class in this project."),
			FNewMenuDelegate::CreateStatic(&FSMUtilityLauncherModule::MakeOpenBlueprintClassMenu),
			false,
			OpenBPIcon);*/
	}
	MenuBuilder.EndSection();
	
	/*MenuBuilder.BeginSection(TEXT("Tools"), LOCTEXT("LogicDriverToolsHeading", "Tools"));
	{
		// Add a null widget so the section is created and can be extended from other modules.
		MenuBuilder.AddWidget(SNullWidget::NullWidget, FText::GetEmpty());
	}
	MenuBuilder.EndSection();*/

	return MenuBuilder.MakeWidget();
}

void FSimulationSystemUtilityModule::RebuildAll()
{
	//UE_LOG(LogTemp, Log, TEXT("Rebuild All clicked!"));
	USimulationSystemEditorFunctionLibrary::FullRebuild();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSimulationSystemUtilityModule, SimulationSystemUtility)