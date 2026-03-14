#include "SimulationSystemEditor.h"

#include "ISettingsModule.h"
#include "LevelEditor.h"
//#include "RebuildAllStyle.h"
#include "SimulationSystemDeveloperSettings.h"
#include "SimulationSystemEditorFunctionLibrary.h"
//#include "SimulationSystemCommands.h"
#include "EntityDataHandleLayout.h"
#include "SimulationDataTableKeyLayout.h"
#include "SquadDataHandleLayout.h"
#include "SquadDataNPCLayout.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FSimulationSystemEditorModule"

void FSimulationSystemEditorModule::StartupModule()
{
	SetupSettings();

	RegisterCustomPropertyTypeLayout("SquadNPCData",
	                                 FOnGetPropertyTypeCustomizationInstance::CreateStatic(
		                                 &FSquadDataNPCLayout::MakeInstance));
	RegisterCustomPropertyTypeLayout("SquadDataHandle",
	                                 FOnGetPropertyTypeCustomizationInstance::CreateStatic(
		                                 &FSquadDataHandleLayout::MakeInstance));
	RegisterCustomPropertyTypeLayout("SimulationArchetypeHandle",
	                                 FOnGetPropertyTypeCustomizationInstance::CreateStatic(
	                                 &FEntityDataHandleLayout::MakeInstance));
	RegisterCustomPropertyTypeLayout("SimulationSystemEntityDataTableKey",
									 FOnGetPropertyTypeCustomizationInstance::CreateStatic(
										 &FSimulationDataTableKeyLayout::MakeInstance));
	
}

void FSimulationSystemEditorModule::UnsetupSettings()
{
	
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Editor", "Plugins", "SimulationSystemDeveloper");
	}
}

void FSimulationSystemEditorModule::RegisterCustomPropertyTypeLayout(FName PropertyTypeName,
	FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	check(PropertyTypeName != NAME_None);

	RegisteredPropertyTypes.Add(PropertyTypeName);

	static FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}

void FSimulationSystemEditorModule::RegisterCustomClassLayout(FName ClassName,
	FOnGetDetailCustomizationInstance DetailLayoutDelegate)
{
	check(ClassName != NAME_None);

	RegisteredClassNames.Add(ClassName);

	static FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
	PropertyModule.RegisterCustomClassLayout( ClassName, DetailLayoutDelegate );
}

void FSimulationSystemEditorModule::ShutdownModule()
{
	UnsetupSettings();
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
    
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