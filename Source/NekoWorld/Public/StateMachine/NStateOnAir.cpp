#include "NStateOnAir.h"

#include "NCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SubSystem/NInputSubsystem.h"

ENState UNStateOnAir::CheckTransition()
{
	if(!Owner)
	{
		return ENState::None;
	}

	if(Owner->GetCharacterMovement()
	&& Owner->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		return ENState::Idle;
	}

	return ENState::None;
}

void UNStateJump::Init()
{
	Super::Init();

	SetParent(ENState::OnAir);
}

void UNStateJump::OnEnter()
{
	Super::OnEnter();

	CharacterReachedJumpApex = false;
	Owner->GetCharacterMovement()->bNotifyApex = true;
	Owner->OnReachedJumpApex.AddDynamic(this, &UNStateJump::OnReachedJumpApex);
	
	Owner->JumpCharacter();
}

void UNStateJump::OnLeave()
{
	Super::OnLeave();
	
	Owner->GetCharacterMovement()->bNotifyApex = false;
	Owner->OnReachedJumpApex.Remove(this, TEXT("OnReachedJumpApex"));
}

ENState UNStateJump::CheckTransition()
{
	if(!Owner)
	{
		return ENState::None;
	}

	if(CharacterReachedJumpApex)
	{
		return ENState::Falling;
	}
	
	if(Owner->GetCharacterMovement()
	&& Owner->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		return ENState::None;
	}
	
	
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{	
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

void UNStateJump::OnReachedJumpApex()
{
	CharacterReachedJumpApex = true;
}

void UNStateFalling::Init()
{
	Super::Init();

	SetParent(ENState::OnAir);
}
