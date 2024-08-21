// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/NStateBase.h"

#include "StateMachine/NStateMachineComponent.h"
#include "NCharacter.h"
#include "NCharacterAnimInstance.h"
#include "SubSystem/NInputSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	if(Owner && Owner->NAnimInstance)
	{
		Owner->NAnimInstance->CurCharacterState = MyState;
	}
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

ENState UNStateOnGround::CheckTransition()
{
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(inputSubsystem->JumpKeyPressed)
		{
			return ENState::Jump;
		}
	}
	
	return ENState::None;
}

void UNStateIdle::Init()
{
	Super::Init();

	SetParent(ENState::OnGround);
}

ENState UNStateIdle::CheckTransition()
{
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(!inputSubsystem->MovementVector.IsNearlyZero())
		{
			return ENState::Run;
		}
	}
	
	return ENState::None;
}

void UNStateRun::Init()
{
	Super::Init();

	SetParent(ENState::OnGround);
}

void UNStateRun::OnUpdate()
{
	UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>();
	if(inputSubsystem)
	{
		Owner->MoveCharacter(inputSubsystem->MovementVector);
	}
}

ENState UNStateRun::CheckTransition()
{
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(inputSubsystem->MovementVector.IsNearlyZero())
		{
			return ENState::Idle;
		}
	}
	
	return ENState::None;
}

void UNStateOnAir::Init()
{
	Super::Init();
}

void UNStateJump::Init()
{
	Super::Init();

	SetParent(ENState::OnAir);
}

void UNStateJump::OnUpdate()
{
	if(Owner && Owner->GetCharacterMovement()
		&& Owner->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
		{
			if(inputSubsystem->JumpKeyPressed)
			{
				Owner->JumpCharacter();		
			}
		}
		
	}
}

ENState UNStateJump::CheckTransition()
{
	if(!Owner)
	{
		return ENState::None;
	}
	
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(Owner->GetCharacterMovement()
			&& Owner->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
		{
			return ENState::None;
		}
		
		if(inputSubsystem->JumpKeyPressed)
		{
			return ENState::None;
		}

		if(inputSubsystem->MovementVector.IsNearlyZero())
		{
			return ENState::Idle;
		}
		else
		{
			return ENState::Run;
		}
	}

	return ENState::None;
}
