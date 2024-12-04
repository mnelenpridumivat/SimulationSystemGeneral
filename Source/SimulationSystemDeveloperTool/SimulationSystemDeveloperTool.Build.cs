using System.IO;
using UnrealBuildTool;

public class SimulationSystemDeveloperTool : ModuleRules
{
    public SimulationSystemDeveloperTool(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicIncludePaths.Add(ModuleDirectory);
        PublicIncludePaths.AddRange(Directory.GetDirectories(Path.Combine(ModuleDirectory, "Public"), "*", SearchOption.AllDirectories));
        PrivateIncludePaths.AddRange(Directory.GetDirectories(Path.Combine(ModuleDirectory, "Private"), "*", SearchOption.AllDirectories));
        
        /*PublicIncludePaths.AddRange(
            new string[]
            {
                "SimulationSystem"
            }
        );*/

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "SimulationSystem"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "ImGui"
            }
        );
        
        // Tell the compiler we want to import the ImPlot symbols when linking against ImGui plugin 
        PrivateDefinitions.Add(string.Format("IMPLOT_API=DLLIMPORT"));
    }
}