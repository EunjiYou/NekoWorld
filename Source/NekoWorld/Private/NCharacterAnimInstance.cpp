// Fill out your copyright notice in the Description page of Project Settings.


#include "NCharacterAnimInstance.h"

#include "NCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/NStateBase.h"
#include "StateMachine/NStateMachineComponent.h"


void UNCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if(!IsValid(OwnerCharacter))
	{
		OwnerCharacter = Cast<ANCharacter>(TryGetPawnOwner()); 
	}

	if(!IsValid(OwnerCharacter))
	{
		return;
	}

	if(auto stateMachineComp = OwnerCharacter->GetComponentByClass<UNStateMachineComponent>())
	{
		CurCharacterState = stateMachineComp->CurStateObj? stateMachineComp->CurStateObj->MyState : ENState::None;
	}
	MoveSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
}
