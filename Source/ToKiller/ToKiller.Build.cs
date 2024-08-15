// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ToKiller : ModuleRules
{
	public ToKiller(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
