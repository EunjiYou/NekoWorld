// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Toolkits/AssetEditorToolkit.h"

/**
 * 
 */
class FNekoWorldAssetEditor : public FAssetEditorToolkit
{
public:
	// Destructor
	virtual ~FNekoWorldAssetEditor() override;

	//// Initializer
	void InitNekoWorldAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, class UNekoWorldAsset* InAsset);


	// Tab Spawn Function
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Detail(const FSpawnTabArgs& Args);

	//// IToolkit override Functions
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetDocumentationLink() const override
	{
		return TEXT("NotAvailable");
	}

private:
	// 에디터가 사용할 고유 앱 명칭
	static const FName NekoWorldEditorAppIdentifier;

	// 에디터 내 Tab들의 고유 명칭
	static const FName ViewportTabId;
	static const FName DetailTabId;

	// Detail View
	TSharedPtr<IDetailsView> DetailsView;

	// 편집할 Object
	UNekoWorldAsset* NekoWorldAsset;
};
