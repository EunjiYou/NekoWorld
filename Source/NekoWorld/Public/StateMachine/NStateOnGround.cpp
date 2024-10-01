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
			return ENState::DashStart;
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
			return ENState::DashStart;
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

void UNStateDashStart::Init()
{
	Super::Init();

	SetParent(ENState::Dash);
}

void UNStateDashStart::OnEnter()
{
	Super::OnEnter();
	
	if(auto movementComp = Owner->GetCharacterMovement())
	{
		movementComp->MaxWalkSpeed = 1200.0f;
	}

	if(StateMachineComponent)
	{
		if(StateMachineComponent->GetPrevState() == ENState::DashEnd)
		{
			StateMachineComponent->TransitionData.DashCount++;
		}
		else
		{
			StateMachineComponent->TransitionData.DashCount = 1;	
		}
	}
}

ENState UNStateDashStart::CheckTransition()
{
	// Dash를 두 단계로 분리하기 위한 State인 지라 바로 DashEndState로 넘김
	return ENState::DashEnd;
}

void UNStateDashEnd::Init()
{
	Super::Init();

	SetParent(ENState::Dash);
}

void UNStateDashEnd::OnEnter()
{
	Super::OnEnter();

	Duration = 0.f;
}

void UNStateDashEnd::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

	Duration += DeltaTime;
	Owner->AddMovementInput(Owner->GetActorForwardVector(), 1.f);
}

ENState UNStateDashEnd::CheckTransition()
{
	if(ReceivedCancelAction == ENActionInputType::Dash)
	{
		if(StateMachineComponent && StateMachineComponent->TransitionData.DashCount < 2)
		{
			return ENState::DashStart;	
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
