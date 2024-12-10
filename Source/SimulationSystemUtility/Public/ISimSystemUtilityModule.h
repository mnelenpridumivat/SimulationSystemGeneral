#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSimulationSystemUtility, Log, All);

#define SIMSYSTEM_UTILITY_LAUNCHER_MODULE_NAME "SimulationSystemUtility"

/**
 * The public interface to this module
 */
class ISimSystemUtilityModule : public IModuleInterface
{
public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static ISimSystemUtilityModule& Get()
	{
		return FModuleManager::LoadModuleChecked<ISimSystemUtilityModule>(SIMSYSTEM_UTILITY_LAUNCHER_MODULE_NAME);
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(SIMSYSTEM_UTILITY_LAUNCHER_MODULE_NAME);
	}

	virtual TSharedPtr<class FExtensibilityManager> GetMenuExtensibilityManager() const = 0;
	virtual TSharedPtr<class FExtensibilityManager> GetToolBarExtensibilityManager() const = 0;
};

