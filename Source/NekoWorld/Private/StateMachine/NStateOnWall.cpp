// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/NStateOnWall.h"

#include "NCharacter.h"
#include "Common/CommonEnum.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StateMachine/NStateMachineComponent.h"
#include "SubSystem/NInputSubsystem.h"


void UNStateOnWall::OnEnter()
{
	Super::OnEnter();

	if(Owner)
	{
		Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		Owner->GetCharacterMovement()->MaxFlySpeed = 200.f;
		Owner->GetCharacterMovement()->BrakingDecelerationFlying = 500.0f;
		Owner->GetCharacterMovement()->bOrientRotationToMovement = false;

		// 발 밑 지형을 판단하기 위해 기준을 낮게 잡아둠. 계단 걷기 때에 영향을 줄 값이라 원상태로 복구 필요
		Owner->GetCharacterMovement()->MaxStepHeight = 10.0f;
	}
}

void UNStateOnWall::OnLeave()
{
	Super::OnLeave();

	if(Owner)
	{
		Owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
		
		Owner->GetCharacterMovement()->MaxStepHeight = 45.0f;
	}
}

ENState UNStateOnWall::CheckTransition()
{
	// ClimbEnd Check
	{
		float sphereRadius = 15.f;
		auto traceStart = Owner->GetActorLocation() + FVector::UnitZ() * (Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + sphereRadius);
		auto traceEnd = traceStart + Owner->GetActorForwardVector() * 50.0f;
		// Trace를 쏘았을 때 벽이 있는 지를 체크한 결과로 StateMachine Transition Data 업데이트
		TArray<AActor*> actorsToIgnore;
		FHitResult hitResult;
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
			traceStart, traceEnd, sphereRadius,
			// TraceChannel::ClimbCheck
			ETraceTypeQuery::TraceTypeQuery3, false, actorsToIgnore, EDrawDebugTrace::ForDuration,
			hitResult, true, FLinearColor::Green, FLinearColor::Red, 1.f );
		if(!hitResult.bBlockingHit)
		{
			return ENState::ClimbingEnd;
		}
	}

	if(StateMachineComponent->GetCurState() == MyState)
	{
		// 공중에서 벽에 붙을 때는 점프를 하지 않으므로 바로 Climbing 상태로 전환
		if(StateMachineComponent->IsChildStateOf(StateMachineComponent->GetPrevState(), ENState::OnAir))
		{
			return ENState::Climbing;			
		}
		// 공중 외에서는 벽에 붙을 시 점프와 함께 등반 시작
		else
		{
			return ENState::ClimbingStart;	
		}
	}
	
	return Super::CheckTransition();
}

void UNStateClimbingStart::Init()
{
	Super::Init();

	SetParent(ENState::OnWall);
}

void UNStateClimbingStart::OnEnter()
{
	Super::OnEnter();

	if(Owner)
	{
		Owner->ClimbingJump(true);
	}
}

void UNStateClimbingStart::OnLeave()
{
	Super::OnLeave();

	Owner->GetCharacterMovement()->StopMovementImmediately();
	Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

ENState UNStateClimbingStart::CheckTransition()
{
	if(Duration > 0.3f)
	{
		return ENState::Climbing;
	}
	
	return Super::CheckTransition();
}

void UNStateClimbing::Init()
{
	Super::Init();

	SetParent(ENState::OnWall);
}

ENState UNStateClimbing::CheckTransition()
{
	if(!IsValid(Owner))
	{
		return ENState::None;
	}

	// 등반 취소 (등반 점프, 등반 완료에서는 캔슬이 되지 않음)
	// X 키를 누르거나 방향키 밑 + 점프 누를 시 취소
	if(auto inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(inputSubsystem->ActionInputButton[(uint8)ENActionInputType::ClimbCancel])
		{
			return ENState::Falling;
		}

		if(inputSubsystem->ActionInputButton[(uint8)ENActionInputType::Jump])
		{
			if(Owner->GetCharacterMovement()->GetPendingInputVector().Z < 0.f)
			{
				return ENState::Falling;	
			}
			else
			{
				return ENState::ClimbingJump;
			}
			
		}
	}

	FFindFloorResult floorResult;
	Owner->GetCharacterMovement()->FindFloor(Owner->GetActorLocation(), floorResult, false);
	if(floorResult.bWalkableFloor)
	{
		return ENState::OnGround;
	}
	
	return Super::CheckTransition();
}

void UNStateClimbingJump::Init()
{
	Super::Init();

	SetParent(ENState::OnWall);
}

void UNStateClimbingJump::OnEnter()
{
	Super::OnEnter();

	if(Owner)
	{
		Owner->ClimbingJump(false);

		// 측면 점프 시 하강하지 않기 위한 조치
		Owner->GetCharacterMovement()->GravityScale = 0.f;
	}
}

void UNStateClimbingJump::OnLeave()
{
	Super::OnLeave();

	if(Owner)
	{
		Owner->GetCharacterMovement()->StopMovementImmediately();
		Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		
		Owner->GetCharacterMovement()->GravityScale = 1.f;
	}
}

ENState UNStateClimbingJump::CheckTransition()
{
	if(Duration > 0.3f)
	{
		return ENState::Climbing;
	}
	
	return Super::CheckTransition();
}

void UNStateClimbingEnd::Init()
{
	Super::Init();

	SetParent(ENState::OnWall);
}

void UNStateClimbingEnd::OnEnter()
{
	Super::OnEnter();

	if(Owner)
	{
		// Todo : 모션 제대로 붙이고서 제거 필요
		Owner->ClimbingJump(true);
	}
}

ENState UNStateClimbingEnd::CheckTransition()
{
	if(Duration > 0.5f)
	{
		return ENState::OnGround;
	}
	
	return Super::CheckTransition();
}
