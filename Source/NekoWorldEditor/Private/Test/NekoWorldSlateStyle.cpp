// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NekoWorldSlateStyle.h"

#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyle.h"
#include "Misc/Paths.h"

TSharedPtr<FSlateStyleSet> FNekoWorldSlateStyle::StyleInstance = nullptr;

void FNekoWorldSlateStyle::Initialize()
{
	if(!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FNekoWorldSlateStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FNekoWorldSlateStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("NekoWorldSlateStyle"));
	return StyleSetName;
}


#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef<class FSlateStyleSet> FNekoWorldSlateStyle::Create()
{
	// FSlateStyleSet()에 들어갈 Name은 GetStyleSetName와 통일 시켜줘야함
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("NekoWorldSlateStyle"));
	Style->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));

	// Menu Icon
	Style->Set("NekoWorld.Command1", new IMAGE_BRUSH(TEXT("Icons/icon_file_switch_16px"), Icon16x16));
	Style->Set("NekoWorld.Command2", new IMAGE_BRUSH(TEXT("Icons/icon_file_savelevels_16px"), Icon16x16));
	Style->Set("NekoWorld.Command3", new IMAGE_BRUSH(TEXT("Icons/icon_file_ProjectOpen_16x"), Icon16x16));
	Style->Set("NekoWorld.Command4", new IMAGE_BRUSH(TEXT("Icons/icon_file_ProjectsRecent_16px"), Icon16x16));

	// Toolbar Icon
	Style->Set("NekoWorldToolbarIcon.Command1", new IMAGE_BRUSH(TEXT("Icons/icon_Landscape_Tool_Erosion_40x"), Icon40x40));
	Style->Set("NekoWorldToolbarIcon.Command1.Small", new IMAGE_BRUSH(TEXT("Icons/icon_Landscape_Tool_Erosion_20x"), Icon20x20));
	Style->Set("NekoWorldToolbarIcon.Command2", new IMAGE_BRUSH(TEXT("Icons/icon_Landscape_Tool_Flatten_40x"), Icon40x40));
	Style->Set("NekoWorldToolbarIcon.Command3", new IMAGE_BRUSH(TEXT("Icons/icon_Landscape_Tool_Noise_40x"), Icon40x40));
	Style->Set("NekoWorldToolbarIcon.Command4", new IMAGE_BRUSH(TEXT("Icons/icon_Landscape_Tool_Smooth_40x"), Icon40x40));

	return Style;
}

#undef IMAGE_BRUSH

void FNekoWorldSlateStyle::ReloadTextures()
{
	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FNekoWorldSlateStyle::Get()
{
	return *StyleInstance;
}
