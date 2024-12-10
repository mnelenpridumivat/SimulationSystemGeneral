using UnrealBuildTool;

public class SimulationSystemUtility : ModuleRules
{
    public SimulationSystemUtility(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "SimulationSystemEditor",
                "Projects",
                "InputCore",
                "UnrealEd",
                "EditorStyle",
                "ToolWidgets",
                "ApplicationCore",
            }
        );
    }
}