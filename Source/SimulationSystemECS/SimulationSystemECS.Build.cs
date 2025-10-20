using System.IO;
using UnrealBuildTool;

public class SimulationSystemECS : ModuleRules
{
    public SimulationSystemECS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
			
        PublicIncludePaths.Add(ModuleDirectory);
        PublicIncludePaths.AddRange(Directory.GetDirectories(Path.Combine(ModuleDirectory, "Public"), "*", SearchOption.AllDirectories));
        PrivateIncludePaths.AddRange(Directory.GetDirectories(Path.Combine(ModuleDirectory, "Private"), "*", SearchOption.AllDirectories));


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "SimulationSystem",
                "MassEntity", 
                "MassSpawner",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}