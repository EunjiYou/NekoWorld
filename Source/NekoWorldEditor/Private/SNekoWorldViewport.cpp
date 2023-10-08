// Fill out your copyright notice in the Description page of Project Settings.


#include "SNekoWorldViewport.h"

#include "NekoWorldViewportClient.h"
#include "AdvancedPreviewScene.h"

SNekoWorldViewport::SNekoWorldViewport()
	: PreviewScene(MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues())))
{
}

SNekoWorldViewport::~SNekoWorldViewport()
{
	if(NekoWorldViewportClient.IsValid())
	{
		NekoWorldViewportClient->Viewport = NULL;
	}
}

void SNekoWorldViewport::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(NekoWorldAsset);
}

TSharedRef<FEditorViewportClient> SNekoWorldViewport::MakeEditorViewportClient()
{
	NekoWorldViewportClient = MakeShareable(new FNekoWorldViewportClient(NekoWorldAssetEditorPtr, PreviewScene.ToSharedRef(), SharedThis(this), NekoWorldAsset));

	return NekoWorldViewportClient.ToSharedRef();
}


void SNekoWorldViewport::Construct(const FArguments& InArgs)
{
	NekoWorldAssetEditorPtr = InArgs._ParentNekoWorldAssetEditor;
	NekoWorldAsset = InArgs._ObjectToEdit;

	SEditorViewport::Construct(SEditorViewport::FArguments());
}
