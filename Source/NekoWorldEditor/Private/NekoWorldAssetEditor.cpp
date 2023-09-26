// Fill out your copyright notice in the Description page of Project Settings.


#include "NekoWorldAssetEditor.h"

#include "NekoWorldAsset.h"
#include "NekoWorldSlateStyle.h"


const FName FNekoWorldAssetEditor::NekoWorldEditorAppIdentifier = FName(TEXT("NekoWorldEditorApp"));
const FName FNekoWorldAssetEditor::ViewportTabId = FName(TEXT("NekoWorldAsset Viewport"));
const FName FNekoWorldAssetEditor::DetailTabId = FName(TEXT("NekoWorldAsset Detail"));

#define LOCTEXT_NAMESPACE "NekoWorldEditor"


void FNekoWorldAssetEditor::InitNekoWorldAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, class UNekoWorldAsset* InAsset)
{
	// 툴바가 들어갈 기본 레이아웃 설계
	const TSharedRef<FTabManager::FLayout> EditorDefaultLayout = FTabManager::NewLayout("Neko_Layout_v2")
		->AddArea
		(
			// Menu?
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
			)
			->Split
			(
				// Viewport Tab
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.6)
					->AddTab(ViewportTabId, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				// Detail Tab
				->Split
				(
					FTabManager::NewStack()
					->AddTab(DetailTabId, ETabState::OpenedTab)
				)
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

TSharedRef<SDockTab> FNekoWorldAssetEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == ViewportTabId);
	return SNew(SDockTab);
}

TSharedRef<SDockTab> FNekoWorldAssetEditor::SpawnTab_Detail(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == DetailTabId);
	return SNew(SDockTab);
}

void FNekoWorldAssetEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_NekoWorldAssetEditor", "NekoWorld Asset Editor"));
	TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(ViewportTabId, FOnSpawnTab::CreateSP(this, &FNekoWorldAssetEditor::SpawnTab_Viewport))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FNekoWorldSlateStyle::GetStyleSetName(), "NekoWorld.Command1"));

	InTabManager->RegisterTabSpawner(DetailTabId, FOnSpawnTab::CreateSP(this, &FNekoWorldAssetEditor::SpawnTab_Detail))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FNekoWorldSlateStyle::GetStyleSetName(), "NekoWorld.Command2"));
}

void FNekoWorldAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(ViewportTabId);
	InTabManager->UnregisterTabSpawner(DetailTabId);
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