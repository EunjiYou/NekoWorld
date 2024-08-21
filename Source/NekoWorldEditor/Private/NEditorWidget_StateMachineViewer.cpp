// Fill out your copyright notice in the Description page of Project Settings.


#include "NEditorWidget_StateMachineViewer.h"
#include "EditorLevelLibrary.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"


void UNEditorWidget_StateMachineViewer::NativeConstruct()
{
	Super::NativeConstruct();

	StateMachineOwner = UEditorLevelLibrary::SpawnActorFromClass(AActor::StaticClass(), FVector::Zero());
	if(StateMachineOwner)
	{
		if(UNStateMachineComponent* stateMachineComponent = Cast<UNStateMachineComponent>(StateMachineOwner->AddComponentByClass(UNStateMachineComponent::StaticClass(), true, FTransform::Identity, false)))
		{
			stateMachineComponent->Init();
			StateDataTree = stateMachineComponent->GenerateDebugData();
		}
	}

	for(FNStateDebugData stateData : StateDataTree)
	{
		GenerateWidgetRecursive(stateData, 0.f);	
	}
}

void UNEditorWidget_StateMachineViewer::GenerateWidgetRecursive(FNStateDebugData data, float marginX)
{
	UNStateDebugWidget* uiStateBase = CreateWidget<UNStateDebugWidget>(GetWorld(), StatebaseWidget);
	uiStateBase->SetPadding(FMargin(marginX, 0.f));

	// Set Text By Enum
	FString EnumValueName;
	UEnum::GetValueAsString(data.MyState).Split(TEXT("::"), nullptr, &EnumValueName);
	uiStateBase->TextBlock->SetText(FText::FromString(EnumValueName));
	
	UVerticalBoxSlot* slot = VerticalBox->AddChildToVerticalBox(uiStateBase);
	slot->SetHorizontalAlignment(HAlign_Left);
	slot->SetVerticalAlignment(VAlign_Top);

	for(FNStateDebugData child : data.Children)
	{
		GenerateWidgetRecursive(child, marginX + 90.0f);
	}
}

void UNEditorWidget_StateMachineViewer::NativeDestruct()
{
	Super::NativeDestruct();

	if(StateMachineOwner)
	{
		StateMachineOwner->Destroy();
	}
}
