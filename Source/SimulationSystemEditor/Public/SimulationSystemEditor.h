#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimulationSystemEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void SetupSettings();
    void UnsetupSettings();
	
    TSet< FName > RegisteredClassNames;
    TSet< FName > RegisteredPropertyTypes;

    void RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate);
    void RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate );
};
