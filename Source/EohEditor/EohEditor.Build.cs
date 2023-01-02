// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EohEditor : ModuleRules
{
	public EohEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"BasketballCards",
			"Core",
			"CoreUObject",
			"Engine"
		});
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"InputCore",
			"Slate",
			"SlateCore"
		});
	}
}
