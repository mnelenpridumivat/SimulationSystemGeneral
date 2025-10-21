// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimulationSystem.h"

#include "ISettingsModule.h"
#include "SimulationSystemSettings.h"
#include "SimulationSystemProfileType.h"

ESimualtionSystemProfileType GetSimulationSystemProfileType()
{
	auto Settings = GetDefault<USimulationSystemSettings>();
	auto Type = Settings->ProfileType;
	if (Type != ESimualtionSystemProfileType::ECS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Profile type %s is not currently supported or outdated!"), *UEnum::GetValueAsString(Type))
	}
	return Type;
}

#define LOCTEXT_NAMESPACE "FSimulationSystemModule"

void FSimulationSystemModule::StartupModule()
{
	SetupSettings();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSimulationSystemModule::ShutdownModule()
{
	UnsetupSettings();
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FSimulationSystemModule::SetupSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "SimulationSystem",
			LOCTEXT("SimulationSystemSettingsName", "Simulation System"),
			LOCTEXT("SimulationSystemSettingsDescription", "Configs for runtime"),
			GetMutableDefault<USimulationSystemSettings>());
	}
}

void FSimulationSystemModule::UnsetupSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "SimulationSystem");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimulationSystemModule, SimulationSystem)