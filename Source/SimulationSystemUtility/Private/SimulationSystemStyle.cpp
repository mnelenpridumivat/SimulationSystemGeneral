#include "SimulationSystemStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyle.h"

//TSharedPtr< FSlateStyleSet > FSimulationSystemStyle::StyleInstance = nullptr;

#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( FSimulationSystemStyle::InResources( RelativePath, ".png" ), __VA_ARGS__ )

void FSimulationSystemStyle::Initialize()
{
	if (GetStyleSetInstance().IsValid())
	{
		return;
	}

	GetStyleSetInstance() = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	GetStyleSetInstance()->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	GetStyleSetInstance()->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	SetBrushes();
	SetIcons();

	FSlateStyleRegistry::RegisterSlateStyle(*GetStyleSetInstance().Get());
}

void FSimulationSystemStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*GetStyleSetInstance().Get());
	ensure(GetStyleSetInstance().IsUnique());
	GetStyleSetInstance().Reset();
}

void FSimulationSystemStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FSimulationSystemStyle::Get()
{
	return *GetStyleSetInstance();
}

FName FSimulationSystemStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SimulationSystemStyle"));
	return StyleSetName;
}

void FSimulationSystemStyle::SetBrushes()
{
}

/*#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )*/

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

void FSimulationSystemStyle::SetIcons()
{
	GetStyleSetInstance()->Set("SimulationSystemUtilityLauncherIcon", new IMAGE_BRUSH(TEXT("Icons/ToolsIcon_40"), Icon40x40));
	GetStyleSetInstance()->Set("SimulationSystemLauncherWarningIcon", new IMAGE_BRUSH(TEXT("Icons/ToolsWarningIcon_40"), Icon40x40));
	GetStyleSetInstance()->Set("SimulationSystemLauncherErrorIcon", new IMAGE_BRUSH(TEXT("Icons/ToolsErrorIcon_40"), Icon40x40));
	//StyleSetInstance->Set("DiscordIcon", new IMAGE_BRUSH(TEXT("Icons/discord-mark-white_16"), Icon16x16));
}

FString FSimulationSystemStyle::InResources(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("SMSystem"))->GetBaseDir() / TEXT("Resources");
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr<FSlateStyleSet>& FSimulationSystemStyle::GetStyleSetInstance()
{
	static TSharedPtr< FSlateStyleSet > StyleSetInstance;
	return StyleSetInstance;
}

/*TSharedRef<FSlateStyleSet> FSimulationSystemStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("RestartEditorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("RestartEditor")->GetBaseDir() / TEXT("Resources"));

	Style->Set("RestartEditor.PluginAction", new IMAGE_BRUSH(TEXT("ButtonIcon_40x"), Icon40x40));
	Style->Set("RestartEditor.PluginAction.Small", new IMAGE_BRUSH(TEXT("ButtonIcon_20x"), Icon20x20));

	return Style;
}*/

#undef DEFAULT_FONT
#undef IMAGE_BRUSH

/*#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT*/
