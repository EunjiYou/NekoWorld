#include "StateMachine/NStateOnGround.h"

#include "NCharacter.h"
#include "StateMachine/NStateMachineComponent.h"
#include "Common/CommonEnum.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SubSystem/NInputSubsystem.h"


ENState UNStateOnGround::CheckTransition()
{
	if(StateMachineComponent && StateMachineComponent->TransitionData.HitWall_OnMoveForward)
	{
		return ENState::OnWall;
	}
	
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(inputSubsystem->JumpKeyPressed)
		{
			return ENState::Jump;
		}
	}
	
	if(Owner && Owner->GetCharacterMovement())
	{
		const auto characterMovement = Owner->GetCharacterMovement();
		// 항시 찾을 수 있는 바닥 정보
		FFindFloorResult FloorResult;
		characterMovement->FindFloor(Owner->GetActorLocation(), FloorResult, false);
					
		if(FloorResult.bBlockingHit && !characterMovement->IsWalkable(FloorResult.HitResult))
		{
			return ENState::Sliding;
		}
	}

	// OnGround 외 상태에서 전환된 경우 Idle로 넘김
	if(StateMachineComponent->GetCurState() == MyState)
	{
		return ENState::Idle;
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

void UNStateDashEnd::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

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

void UNStateSliding::Init()
{
	Super::Init();

	SetParent(ENState::OnGround);
}

void UNStateSliding::OnEnter()
{
	Super::OnEnter();

	// 슬라이딩 중에는 회전하지 않도록 Block 처리
	if(Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void UNStateSliding::OnLeave()
{
	Super::OnLeave();

	// 슬라이딩 중 Block 시켜둔 회전 기능 리셋
	if(Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

ENState UNStateSliding::CheckTransition()
{
	if(Owner && Owner->GetCharacterMovement())
	{
		const auto characterMovement = Owner->GetCharacterMovement();
		FFindFloorResult FloorResult;
		characterMovement->FindFloor(Owner->GetActorLocation(), FloorResult, false);
					
		if(!FloorResult.bBlockingHit || characterMovement->IsWalkable(FloorResult.HitResult))
		{
			return ENState::Idle;
		}
	}
	
	return Super::CheckTransition();
}
