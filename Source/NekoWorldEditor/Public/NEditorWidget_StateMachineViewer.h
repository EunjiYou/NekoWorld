// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "StateMachine/NStateMachineComponent.h"
#include "NEditorWidget_StateMachineViewer.generated.h"

class UVerticalBox;
class UTextBlock;


UCLASS(BlueprintType)
class UNStateDebugWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextBlock;
};

UCLASS(BlueprintType)
class UNEditorWidget_StateMachineViewer : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void GenerateWidgetRecursive(FNStateDebugData data, float marginX);

	virtual void NativeDestruct() override;
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UNStateDebugWidget> StatebaseWidget;

	UPROPERTY(meta=(BindWidget))
	UVerticalBox* VerticalBox;
	
	AActor* StateMachineOwner;
	TArray<FNStateDebugData> StateDataTree;
};
