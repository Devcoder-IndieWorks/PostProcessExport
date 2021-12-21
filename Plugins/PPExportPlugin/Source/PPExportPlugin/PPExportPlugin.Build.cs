// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PPExportPlugin : ModuleRules
{
	public PPExportPlugin( ReadOnlyTargetRules Target ) : base( Target )
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = false;
		
		PrivateIncludePaths.AddRange(
		    new string[] {
				"PPExportPlugin/Private"
			});
			
		PrivateDependencyModuleNames.AddRange(
		    new string[] {
				"Core",
				"CoreUObject",
				"Engine",
                "Json",
                "JsonUtilities",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "Blutility",
                "UMG"
			});
	}
}