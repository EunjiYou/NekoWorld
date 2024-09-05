// Fill out your copyright notice in the Description page of Project Settings.


#include "NEditorWidget_StateMachineViewer.h"
#include "EditorLevelLibrary.h"
#include "EditorUtilityWidgetComponents.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "StateMachine/NStateBase.h"


void UNEditorWidget_StateMachineViewer::NativeConstruct()
{
	Super::NativeConstruct();

	TryMakeStateWidget();
}

void UNEditorWidget_StateMachineViewer::TryMakeStateWidget()
{
	StateScrollBox->ClearChildren();
	
	auto myCharacter = GEditor->GetPIEWorldContext()? UGameplayStatics::GetPlayerCharacter(GEditor->GetPIEWorldContext()->World(), 0) : nullptr;
	if(myCharacter)
	{
		auto stateMachineComp = myCharacter->GetComponentByClass<UNStateMachineComponent>();
		if(stateMachineComp)
		{
			StateDataTree = stateMachineComp->GenerateDebugData();
		}
	}
	else
	{
		// Play 이전에 State UI 생성을 위한 별도 처리
		auto stateMachineOwner = UEditorLevelLibrary::SpawnActorFromClass(AActor::StaticClass(), FVector::Zero());
		if(stateMachineOwner)
		{
			if(UNStateMachineComponent* stateMachineComponent = Cast<UNStateMachineComponent>(stateMachineOwner->AddComponentByClass(UNStateMachineComponent::StaticClass(), true, FTransform::Identity, false)))
			{
				stateMachineComponent->Init();
				StateDataTree = stateMachineComponent->GenerateDebugData();
			}
		}
		
		// UI 생성을 다했으면 Actor는 꼭 지우자!
		stateMachineOwner->Destroy();
	}
	
	for(FNStateDebugData stateData : StateDataTree)
	{
		GenerateWidgetRecursive(stateData, 0.f);	
	}
}

void UNEditorWidget_StateMachineViewer::GenerateWidgetRecursive(FNStateDebugData data, float marginX)
{
	UNStateDebugWidget* uiStateBase = CreateWidget<UNStateDebugWidget>(GetWorld(), StatebaseWidget);
	StateWidgetMap.Add(data.MyState, uiStateBase);
	
	// Set Text By Enum
	FString EnumValueName;
	UEnum::GetValueAsString(data.MyState).Split(TEXT("::"), nullptr, &EnumValueName);
	uiStateBase->TextBlock->SetText(FText::FromString(EnumValueName));
	
	auto slot = Cast<UScrollBoxSlot>(StateScrollBox->AddChild(uiStateBase));
	if (slot)
	{
		// Set the alignment of the slot
		slot->SetHorizontalAlignment(HAlign_Left);
		slot->SetVerticalAlignment(VAlign_Top);
		slot->SetPadding(FMargin(marginX, 0.f));
	}

	for(FNStateDebugData child : data.Children)
	{
		GenerateWidgetRecursive(child, marginX + 90.0f);
	}
}

void UNEditorWidget_StateMachineViewer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// 현재 Character State 표시
	auto myCharacter = GEditor->GetPIEWorldContext()? UGameplayStatics::GetPlayerCharacter(GEditor->GetPIEWorldContext()->World(), 0) : nullptr;
	if(myCharacter)
	{
		auto stateMachineComp = myCharacter->GetComponentByClass<UNStateMachineComponent>();
		if(stateMachineComp)
		{
			ENState curState = stateMachineComp->CurStateObj? stateMachineComp->CurStateObj->MyState : ENState::None;
			ENState prevState = stateMachineComp->PrevStateObj? stateMachineComp->PrevStateObj->MyState : ENState::None;
			if(CurState == curState)
			{
				return;
			}

			CurState = curState;
			if(auto curStateBtn = StateWidgetMap.FindRef(CurState))
			{
				curStateBtn->SetColorAndOpacity(FLinearColor::Red);
			}
			if(auto prevStateBtn = StateWidgetMap.FindRef(prevState))
			{
				prevStateBtn->SetColorAndOpacity(FLinearColor::Gray);
			}
		}
	}
	else
	{
		for(auto stateBtn : StateScrollBox->GetAllChildren())
		{
			if(auto stateDebugBtn = Cast<UNStateDebugWidget>(stateBtn))
			{
				stateDebugBtn->SetColorAndOpacity(FLinearColor::Gray);				
			}
		}
	}
}

void UNEditorWidget_StateMachineViewer::NativeDestruct()
{
	Super::NativeDestruct();
	
	StateWidgetMap.Empty();
}
