using System.IO;
using UnrealBuildTool;

public class SimulationSystemEditor : ModuleRules
{
    public SimulationSystemEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.Add(ModuleDirectory);
        PublicIncludePaths.AddRange(Directory.GetDirectories(Path.Combine(ModuleDirectory, "Public"), "*", SearchOption.AllDirectories));
        PrivateIncludePaths.AddRange(Directory.GetDirectories(Path.Combine(ModuleDirectory, "Private"), "*", SearchOption.AllDirectories));
        
        /*PublicIncludePaths.AddRange(
            new string[]
            {
                "SimulationSystem",
                "SimulationSystemDeveloperTool"
            }
        );*/

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "SimulationSystem",
                "SimulationSystemDeveloperTool"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "ToolMenus",
                "InputCore",
                "Projects",
                "EditorScriptingUtilities",
                "UnrealEd",
                "PropertyEditor",
                "EditorSubsystem"
            }
        );
    }
}