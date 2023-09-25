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
	// �����Ͱ� ����� ���� �� �̸�
	static const FName NekoWorldEditorAppIdentifier;
};
