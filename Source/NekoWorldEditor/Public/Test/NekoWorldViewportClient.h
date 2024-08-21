// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class UNekoWorldAsset;

class FNekoWorldViewportClient : public FEditorViewportClient, public TSharedFromThis<FNekoWorldViewportClient>
{
public:
	FNekoWorldViewportClient(
		TWeakPtr<class FNekoWorldAssetEditor> ParentEditor,
		const TSharedRef<class FAdvancedPreviewScene>& AdvPreviewScene,
		const TSharedRef<class SNekoWorldViewport>& NekoWorldViewport,
		UNekoWorldAsset* ObjectToEdit);
	~FNekoWorldViewportClient();

private:
	TWeakPtr<class FNekoWorldAssetEditor> NekoWorldAssetEditorPtr;
	TWeakPtr<class SNekoWorldViewport> NekoWorldViewportPtr;
	UNekoWorldAsset* NekoWorldAsset;
	class FAdvancedPreviewScene* AdvancedPreviewScene;
};
