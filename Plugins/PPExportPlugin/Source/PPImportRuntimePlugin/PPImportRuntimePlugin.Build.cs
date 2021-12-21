// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PPImportRuntimePlugin : ModuleRules
{
	public PPImportRuntimePlugin( ReadOnlyTargetRules Target ) : base( Target )
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = false;
		
		PrivateIncludePaths.AddRange(
		    new string[] {
				"PPImportRuntimePlugin/Private"
			});
			
		PrivateDependencyModuleNames.AddRange(
		    new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Json",
				"JsonUtilities"
			});
	}
}