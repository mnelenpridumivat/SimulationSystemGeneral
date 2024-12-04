// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class SimulationSystem : ModuleRules
{
	public SimulationSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
			
		PublicIncludePaths.Add(ModuleDirectory);
		PublicIncludePaths.AddRange(Directory.GetDirectories(Path.Combine(ModuleDirectory, "Public"), "*", SearchOption.AllDirectories));
		PrivateIncludePaths.AddRange(Directory.GetDirectories(Path.Combine(ModuleDirectory, "Private"), "*", SearchOption.AllDirectories));
		
		/*PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				"SimulationSystem",
				"SimulationSystem/Public",
				"SimulationSystem/Public/Graph"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				"SimulationSystem/Private",
				//"Public",
				"SimulationSystem/Private/Graph",
				//"Public/Graph"
			}
			);*/
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Engine",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"NavigationSystem",
				"Settings",
				"GameplayAbilities"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
