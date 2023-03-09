// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OFYGame : ModuleRules
{
	public OFYGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[]
			{
				"OFYGame"
			});
	
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject", 
				"Engine",
				"GameplayTags",
                "GameplayTasks",
                "GameplayAbilities",
				"ModularGameplay",
				"ModularGameplayActors",
				"GameFeatures",
				"CommonLoadingScreen",
				"ControlFlows"
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"InputCore",
				"Slate",
				"SlateCore",
				"EnhancedInput",
				"UMG",
				"CommonUI",
				"CommonInput",
				"CommonGame",
				"CommonUser",
				"UIExtension",
				"GameSubtitles"
			});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
