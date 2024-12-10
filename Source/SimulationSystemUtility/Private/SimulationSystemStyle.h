#pragma once

#include "CoreMinimal.h"

class FSimulationSystemStyle
{
public:

	static void Initialize();

	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set for the Shooter game */
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

protected:
	static void SetBrushes();
	static void SetIcons();

private:
	static FString InResources(const FString& RelativePath, const ANSICHAR* Extension);

	static TSharedPtr< FSlateStyleSet >& GetStyleSetInstance();
	
	//static TSharedRef< class FSlateStyleSet > Create();
	//static TSharedPtr< FSlateStyleSet > StyleSetInstance;
	
};
