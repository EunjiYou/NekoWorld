// Fill out your copyright notice in the Description page of Project Settings.

#include "NekoWorldEditorModule.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "LevelEditor.h"

IMPLEMENT_MODULE(FNekoWorldEditorModule, NekoWorldEditor);

#define LOCTEXT_NAMESPACE "NekoWorldMenu"

void FNekoWorldEditorModule::StartupModule()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	DeferredStartDelegateHandle = LevelEditorModule.OnTabManagerChanged().AddRaw(this, &FNekoWorldEditorModule::OnTabManagerChanged);
}

void FNekoWorldEditorModule::ShutdownModule()
{
	// EditorSubsystem lifetime�� ������ ũ���� �߻��ϹǷ� Unregister ����. ���� ���� ��� �ٽ� �õ�
	// if(UEditorUtilitySubsystem* editorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
	// {
	// 	editorUtilitySubsystem->UnregisterTabByID(StateMachineViewerTabId);
	// }
}

void FNekoWorldEditorModule::OnTabManagerChanged()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	LevelEditorModule.OnTabManagerChanged().Remove(DeferredStartDelegateHandle);

	if(UEditorUtilitySubsystem* editorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
	{
		UEditorUtilityWidgetBlueprint* widget = LoadObject<UEditorUtilityWidgetBlueprint>(NULL, TEXT("/Game/Blueprint/UI_StateMachineViewer.UI_StateMachineViewer"), NULL, LOAD_None, NULL);
		editorUtilitySubsystem->RegisterTabAndGetID(widget, StateMachineViewerTabId);
	}
}

#undef LOCTEXT_NAMESPACE
