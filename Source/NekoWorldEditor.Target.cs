// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

// [23/09/18] ���� �߰��� NekoWorldEditorModule���� ������
public class NekoWorldEditorTarget : TargetRules
{
	public NekoWorldEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "NekoWorld", "NekoWorldEditor" } );
	}
}
