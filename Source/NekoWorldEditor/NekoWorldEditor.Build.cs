// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class NekoWorldEditor : ModuleRules
{
	public NekoWorldEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		// EditorUtilityWidget(StateMachineViewer) �ڵ忡 �ʿ��� ���
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UMGEditor"
		});
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
			"NekoWorld",
			// For Editor Utility Widget
			"EditorScriptingUtilities",
			"UMG"
        });
	}
}
