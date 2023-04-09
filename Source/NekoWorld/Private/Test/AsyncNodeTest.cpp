// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/AsyncNodeTest.h"
#include "Kismet/GameplayStatics.h"


UMultipleOutput* UMultipleOutput::MultipleOutputNode(UObject* WorldContextObj)
{
	UMultipleOutput* Node = NewObject<UMultipleOutput>(WorldContextObj);
	Node->BeginPlay();

	return Node;
}

void UMultipleOutput::BeginPlay()
{
	FTimerHandle WaitHandle;
	float WaitTime = 3.0f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([this, &WaitHandle]()
	{
		OnComplete.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(WaitHandle);
	}), WaitTime, false);
}


void UMultipleInput::MultipleInputNodeWithLatent(UObject* WorldContextObj, TEnumAsByte<MultipleInputPinType> InputPins, FLatentActionInfo LatentInfo)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObj, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& pLatentActionManager = World->GetLatentActionManager();
		// ���� Action�� �ִ����� �˻�
		FInputAction* pInputAction = pLatentActionManager.FindExistingAction<FInputAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

		// Not Exist Action
		if (!pInputAction)
		{
			if (InputPins == MultipleInputPinType::Type1)
			{
				//
				pInputAction = new FInputAction(LatentInfo);
				pLatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, pInputAction);
			}
			if (InputPins == MultipleInputPinType::Type2)
			{
				//

			}
		}
	}
}

void UMultipleInput::MultipleInputNode(UObject* WorldContextObj, TEnumAsByte<MultipleInputPinType> InputPins, FLatentActionInfo LatentInfo)
{
	if (InputPins == MultipleInputPinType::Type1)
	{

	}
	if (InputPins == MultipleInputPinType::Type2)
	{

	}
}
