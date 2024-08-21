// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class NekoWorldEditor : ModuleRules
{
	public NekoWorldEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		// EditorUtilityWidget(StateMachineViewer) 코드에 필요한 모듈
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

            // ** IToolkit override에 필요한 모듈들
            "UnrealEd",		// 언리얼 엔진에 통합되어 있는 통합 개발 툴셋
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
