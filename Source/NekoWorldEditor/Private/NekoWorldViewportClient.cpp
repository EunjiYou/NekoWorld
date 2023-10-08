// Fill out your copyright notice in the Description page of Project Settings.


#include "NekoWorldViewportClient.h"

#include "AdvancedPreviewScene.h"
#include "SNekoWorldViewport.h"

FNekoWorldViewportClient::FNekoWorldViewportClient(TWeakPtr<FNekoWorldAssetEditor> ParentEditor, const TSharedRef<FAdvancedPreviewScene>& AdvPreviewScene, const TSharedRef<SNekoWorldViewport>& NekoWorldViewport, UNekoWorldAsset* ObjectToEdit)
		: FEditorViewportClient(nullptr, &AdvPreviewScene.Get(), StaticCastSharedRef<SEditorViewport>(NekoWorldViewport))
		, NekoWorldAssetEditorPtr(ParentEditor)
		, NekoWorldViewportPtr(NekoWorldViewport)
		, NekoWorldAsset(ObjectToEdit)
{
	SetViewMode(VMI_Lit);

	AdvancedPreviewScene = static_cast<FAdvancedPreviewScene*>(PreviewScene);
}

FNekoWorldViewportClient::~FNekoWorldViewportClient()
{
	
}
