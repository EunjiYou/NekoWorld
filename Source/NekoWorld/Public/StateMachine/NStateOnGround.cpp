#include "NStateOnGround.h"

#include "NCharacter.h"
#include "NStateMachineComponent.h"
#include "Common/CommonEnum.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SubSystem/NInputSubsystem.h"


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
		if(inputSubsystem->DashKeyPressed)
		{
			return ENState::Dash;
		}
		
		if(!inputSubsystem->MovementVector.IsNearlyZero())
		{
			return ENState::WalkRun;
		}
	}
	
	return ENState::None;
}

void UNStateWalkRun::Init()
{
	Super::Init();

	SetParent(ENState::OnGround);
}

ENState UNStateWalkRun::CheckTransition()
{
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(inputSubsystem->DashKeyPressed)
		{
			return ENState::Dash;
		}
		
		if(inputSubsystem->MovementVector.IsNearlyZero())
		{
			return ENState::Idle;
		}
		else
		{
			if(inputSubsystem->IsRun)
			{
				return ENState::Run;
			}
			else
			{
				return ENState::Walk;
			}
		}
	}
	
	return ENState::None;
}

void UNStateWalk::Init()
{
	Super::Init();

	SetParent(ENState::WalkRun);
}

void UNStateWalk::OnEnter()
{
	Super::OnEnter();
	
	if(auto movementComp = Owner->GetCharacterMovement())
	{
		movementComp->MaxWalkSpeed = 100.0f;
	}
}

void UNStateRun::Init()
{
	Super::Init();

	SetParent(ENState::WalkRun);
}

void UNStateRun::OnEnter()
{
	Super::OnEnter();

	if(auto movementComp = Owner->GetCharacterMovement())
	{
		movementComp->MaxWalkSpeed = 600.0f;
	}
}

void UNStateDash::Init()
{
	Super::Init();

	SetParent(ENState::OnGround);
}

void UNStateDash::OnEnter()
{
	Super::OnEnter();

	Duration = 0.f;
	if(auto movementComp = Owner->GetCharacterMovement())
	{
		movementComp->MaxWalkSpeed = 1200.0f;
	}

	if(StateMachineComponent)
	{
		if(StateMachineComponent->PrevState == MyState)
		{
			StateMachineComponent->TransitionData.DashCount++;
		}
		else
		{
			StateMachineComponent->TransitionData.DashCount = 1;	
		}
	}
}

void UNStateDash::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

	Duration += DeltaTime;
	Owner->AddMovementInput(Owner->GetActorForwardVector(), 1.f);
}

ENState UNStateDash::CheckTransition()
{
	if(ReceivedCancelAction == ENActionInputType::Dash)
	{
		if(StateMachineComponent && StateMachineComponent->TransitionData.DashCount < 2)
		{
			return ENState::Dash;	
		}
	}
	
	if( Duration > 1.2f )
	{
		if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
		{
			if(!inputSubsystem->MovementVector.IsNearlyZero())
			{
				return ENState::Sprint;
			}
			else
			{
				return ENState::Idle;
			}
		}
	}
	
	return ENState::None;
}

void UNStateSprint::Init()
{
	Super::Init();

	SetParent(ENState::OnGround);
}

void UNStateSprint::OnEnter()
{
	Super::OnEnter();
	if(auto movementComp = Owner->GetCharacterMovement())
	{
		movementComp->MaxWalkSpeed = 1200.0f;
	}
}

ENState UNStateSprint::CheckTransition()
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
