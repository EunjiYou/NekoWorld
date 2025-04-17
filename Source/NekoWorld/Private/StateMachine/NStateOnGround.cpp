#include "StateMachine/NStateOnGround.h"

#include "NCharacter.h"
#include "StateMachine/NStateMachineComponent.h"
#include "Common/CommonEnum.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SubSystem/NInputSubsystem.h"


void UNStateOnGround::OnEnter()
{
	Super::OnEnter();
	
	// Todo : OnGround 자식 상태 진입 시 호출해줘야함! (State 전반적으로 Parent OnEnter/OnLeave 호출 여부 개발 필요)

	// OnWall 상태같은 곳에서 복귀했을 때를 위해 MovementMode 세팅
	Owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Owner->GetCharacterMovement()->RotationRate = FRotator(0.f, StateMachineComponent? StateMachineComponent->StateData.GroundRotationRateYaw : 100.f, 0.f);
}

ENState UNStateOnGround::CheckTransition()
{
	if(StateMachineComponent && StateMachineComponent->TransitionData.HitWall_OnMoveForward)
	{
		return ENState::OnWall;
	}
	
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(inputSubsystem->IsActionInputPressed(ENActionInputType::Jump))
		{
			// 지면에서 점프 키 입력 시 다이빙 가능 여부 체크. 아닌 경우는 Jump 상태로 넘김
			FVector startPos = Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 100.f);
			FVector endPos = startPos + (Owner->GetActorUpVector() * -500.f);
			TArray<FHitResult> hitResults;
			TArray<AActor*> actorsToIgnore;
			UKismetSystemLibrary::LineTraceMulti(GetWorld(),
				startPos, endPos,
				// Visibility
				TraceTypeQuery1, false, actorsToIgnore, EDrawDebugTrace::ForDuration,
				hitResults, true, FLinearColor::Red, FLinearColor::Green, 1.f);

			auto hitResultNum = hitResults.Num();
			// 아무것도 안 걸렸으면 점프로 전환
			if(hitResultNum == 0)
			{
				return ENState::Jump;
			}

			// 0번째(가장 먼저 닿은 Actor)가 물이여야 한다는 전제로 체크
			auto waterVolume = Cast<APhysicsVolume>(hitResults[0].GetActor());
			if(!waterVolume || !waterVolume->bWaterVolume)
			{
				return ENState::Jump;
			}

			// 물 외에 닿은 게 없으면 다이빙 가능한 수면으로 생각하고 다이빙으로 넘김
			if(hitResultNum == 1)
			{
				return ENState::Diving;
			}

			// 물 - 지면 차가 다이빙 가능 높이만큼은 되면 다이빙으로 전환
			float diveableHeight = 100.f;
			float waterHeight = hitResults[0].GetActor()->GetActorLocation().Z - hitResults[1].GetActor()->GetActorLocation().Z;
			if(diveableHeight < waterHeight)
			{
				return ENState::Diving;
			}
			else
			{
				return ENState::Jump;
			}
		}
	}

	if(Owner->GetCharacterMovement())
	{
		if(Owner->GetCharacterMovement()->IsFalling())
		{
			return ENState::Falling;
		}
		else if(Owner->GetCharacterMovement()->IsSwimming())
		{
			return ENState::OnWater;
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
		if(inputSubsystem->IsActionInputPressed(ENActionInputType::Dash))
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
		if(inputSubsystem->IsActionInputPressed(ENActionInputType::Dash))
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
