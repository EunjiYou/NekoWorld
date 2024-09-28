// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/NAnimNotify_CancelAction.h"

#include "NCharacter.h"
#include "StateMachine/NStateBase.h"
#include "StateMachine/NStateMachineComponent.h"

void UNAnimNotify_CancelAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(!OwnerCharacter.IsValid())
	{
		return;
	}

	if(auto stateComp = OwnerCharacter.Get()->GetComponentByClass<UNStateMachineComponent>())
	{
		if(auto stateObj = stateComp->CurStateObj)
		{
			stateObj->SetInputCancelAction(ActionInputType);
		}
	}
}

void UNAnimNotify_CancelAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
