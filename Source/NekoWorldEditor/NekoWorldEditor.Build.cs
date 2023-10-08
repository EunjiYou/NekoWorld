// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class NekoWorldEditor : ModuleRules
{
	public NekoWorldEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore",
			
			// For Slate UI
			"PropertyEditor", "Slate", "SlateCore",

            // ** IToolkit override�� �ʿ��� ����
            "UnrealEd",		// �𸮾� ������ ���յǾ� �ִ� ���� ���� ����
            "LevelEditor",
			// **

			// For Custom Viewport
			"AdvancedPreviewScene",
			//

			// My Runtime Module
			"NekoWorld"
        });
	}
}
