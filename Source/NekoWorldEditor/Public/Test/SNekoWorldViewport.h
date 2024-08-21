// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SEditorViewport.h"
#include "UObject/GCObject.h"


class FNekoWorldAssetEditor;
class UNekoWorldAsset;
class FNekoWorldViewportClient;

class SNekoWorldViewport : public SEditorViewport, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SNekoWorldViewport) {}
		SLATE_ARGUMENT(TWeakPtr<FNekoWorldAssetEditor>, ParentNekoWorldAssetEditor)
		SLATE_ARGUMENT(UNekoWorldAsset*, ObjectToEdit)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	SNekoWorldViewport();
	~SNekoWorldViewport() override;


	// FGCObject 인터페이스. UObject가 아니어도 GC 회수 가능
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

protected:
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

private:
	TWeakPtr<FNekoWorldAssetEditor> NekoWorldAssetEditorPtr;
	TSharedPtr<class FAdvancedPreviewScene> PreviewScene;
	TSharedPtr<class FNekoWorldViewportClient> NekoWorldViewportClient;
	UNekoWorldAsset* NekoWorldAsset;
};
