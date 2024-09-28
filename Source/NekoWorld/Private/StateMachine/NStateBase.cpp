// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/NStateBase.h"

#include "StateMachine/NStateMachineComponent.h"
#include "NCharacter.h"
#include "Common/CommonEnum.h"


void UNStateBase::Init()
{
	if(UObject* outer = GetOuter())
	{
		StateMachineComponent = Cast<UNStateMachineComponent>(outer);
		if(StateMachineComponent)
		{
			Owner = Cast<ANCharacter>(StateMachineComponent->GetOwner());
		}
	}
}

void UNStateBase::OnEnter()
{
	HasCancelActionInput = false;
	CancelActionInputs.Empty();
	ReceivedCancelAction = ENActionInputType::None;
}

void UNStateBase::SetParent(ENState ParentState)
{
	if(StateMachineComponent)
	{
		if(UNStateBase* parentStateObj = StateMachineComponent->GetState(ParentState))
		{
			parentStateObj->Children.Add(MyState);
			Parent = ParentState;
		}
	}
}
