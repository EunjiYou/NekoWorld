// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "StateMachine/NStateMachineComponent.h"
#include "NEditorWidget_StateMachineViewer.generated.h"

class UEditorUtilityButton;
class UEditorUtilityScrollBox;
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
	void TryMakeStateWidget();
	void GenerateWidgetRecursive(FNStateDebugData data, float marginX);
	UFUNCTION()
	void OnClickStateLocClearBtn();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeDestruct() override;

	void OnStateChanged(ENState curState, ENState nextState);
	
public:
	UPROPERTY(meta=(BindWidget))
	UEditorUtilityScrollBox* StateScrollBox;

	UPROPERTY(meta=(BindWidget))
	UEditorUtilityScrollBox* StateLogScrollBox;
	
	UPROPERTY(meta=(BindWidget))
	UEditorUtilityButton* StateLogClearBtn;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UNStateDebugWidget> StatebaseWidget;
	
	TArray<FNStateDebugData> StateDataTree;

	ENState CurState;
	TMap<ENState, UNStateDebugWidget*> StateWidgetMap;

	// StateChange Delegate 바운딩 여부 체크
	bool IsBoundingStateChanged = false;
};
