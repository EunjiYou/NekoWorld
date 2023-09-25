// Fill out your copyright notice in the Description page of Project Settings.


#include "NekoWorldAssetEditor.h"
#include "NekoWorldAsset.h"


const FName FNekoWorldAssetEditor::NekoWorldEditorAppIdentifier = FName(TEXT("NekoWorldEditorApp"));

#define LOCTEXT_NAMESPACE "NekoWorldEditor"


void FNekoWorldAssetEditor::InitNekoWorldAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, class UNekoWorldAsset* InAsset)
{
	// 툴바가 들어갈 기본 레이아웃 설계
	const TSharedRef<FTabManager::FLayout> EditorDefaultLayout = FTabManager::NewLayout("Neko_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
			)
		);

	// 에디터 초기화
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, NekoWorldEditorAppIdentifier, EditorDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, InAsset);
}

FNekoWorldAssetEditor::~FNekoWorldAssetEditor()
{
}

void FNekoWorldAssetEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FNekoWorldAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}


FName FNekoWorldAssetEditor::GetToolkitFName() const
{
	return FName("NekoWorld Editor");
}

FText FNekoWorldAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "NekoWorldEditor");
}

FString FNekoWorldAssetEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "Neko").ToString();
}

FLinearColor FNekoWorldAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.0f, 0.0f, 0.2f, 0.5f);
}

#undef LOCTEXT_NAMESPACE