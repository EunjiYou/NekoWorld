// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/NStateOnWall.h"

#include "NCharacter.h"
#include "Common/CommonEnum.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
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
		Owner->GetCharacterMovement()->BrakingDecelerationFlying = 1500.0f;
		Owner->GetCharacterMovement()->bOrientRotationToMovement = false;

		// 발 밑 지형을 판단하기 위해 기준을 낮게 잡아둠. 계단 걷기 때에 영향을 줄 값이라 원상태로 복구 필요
		Owner->GetCharacterMovement()->MaxStepHeight = 10.0f;

		// 등반 시 캐릭터 각도를 벽 맞은 편으로 보정. 추후 등반 중 계속 벽면을 체크하여 보정하게 될 수 있음
		if(StateMachineComponent)
		{
			auto desireRotation = UKismetMathLibrary::MakeRotFromXZ(-StateMachineComponent->TransitionData.HitWall_Normal, FVector::UpVector);
			Owner->SetActorRotation(desireRotation);
		}
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
	if(!StateMachineComponent)
	{
		return ENState::None;
	}
	
	// ClimbEnd Check
	if(StateMachineComponent->GetCurState() != ENState::ClimbEnd
		&& StateMachineComponent->GetCurState() != ENState::ClimbSprint)
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
			return ENState::ClimbEnd;
		}
	}

	if(StateMachineComponent->GetCurState() == MyState)
	{
		// 공중에서 벽에 붙을 때는 점프를 하지 않으므로 바로 Climbing 상태로 전환
		if(StateMachineComponent->IsChildStateOf(StateMachineComponent->GetPrevState(), ENState::OnAir))
		{
			return ENState::ClimbMove;			
		}
		// 공중 외에서는 벽에 붙을 시 점프와 함께 등반 시작
		else
		{
			return ENState::ClimbStart;	
		}
	}
	
	return Super::CheckTransition();
}

void UNStateClimbStart::Init()
{
	Super::Init();

	SetParent(ENState::OnWall);
}

void UNStateClimbStart::OnEnter()
{
	Super::OnEnter();

	if(Owner)
	{
		Owner->ClimbingJump(true);
	}
}

void UNStateClimbStart::OnLeave()
{
	Super::OnLeave();

	Owner->GetCharacterMovement()->StopMovementImmediately();
	Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

ENState UNStateClimbStart::CheckTransition()
{
	if(Duration > 0.3f)
	{
		return ENState::ClimbMove;
	}
	
	return Super::CheckTransition();
}

void UNStateClimbMove::Init()
{
	Super::Init();

	SetParent(ENState::OnWall);
}

ENState UNStateClimbMove::CheckTransition()
{
	if(!IsValid(Owner))
	{
		return ENState::None;
	}


	if(auto inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		// 등반 취소 (등반 점프, 등반 완료에서는 캔슬이 되지 않음)
		// X 키를 누르거나 방향키 밑 + 점프 누를 시 취소
		if(inputSubsystem->IsActionInputPressed(ENActionInputType::ClimbCancel))
		{
			return ENState::Falling;
		}

		// 등반 점프 / 낙하 체크
		if(inputSubsystem->IsActionInputPressed(ENActionInputType::Jump))
		{
			// 방향키 누름 없이 점프 누르면 낙하
			if(Owner->GetCharacterMovement()->GetPendingInputVector().Z < 0.f)
			{
				return ENState::Falling;	
			}
			else
			{
				return ENState::ClimbJump;
			}
		}

		if(inputSubsystem->IsActionInputPressed(ENActionInputType::Dash))
		{
			return ENState::ClimbSprint;
		}
	}

	// 캐릭터가 바닥에 닿으면 OnGround 상태로 변경
	FFindFloorResult floorResult;
	Owner->GetCharacterMovement()->FindFloor(Owner->GetActorLocation(), floorResult, false);
	if(floorResult.bWalkableFloor)
	{
		return ENState::OnGround;
	}
	
	return Super::CheckTransition();
}

void UNStateClimbJump::Init()
{
	Super::Init();

	SetParent(ENState::OnWall);
}

void UNStateClimbJump::OnEnter()
{
	Super::OnEnter();

	if(Owner)
	{
		Owner->ClimbingJump(false);

		// 측면 점프 시 하강하지 않기 위한 조치
		Owner->GetCharacterMovement()->GravityScale = 0.f;
	}
}

void UNStateClimbJump::OnLeave()
{
	Super::OnLeave();

	if(Owner)
	{
		Owner->GetCharacterMovement()->StopMovementImmediately();
		Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		
		Owner->GetCharacterMovement()->GravityScale = 1.f;
	}
}

ENState UNStateClimbJump::CheckTransition()
{
	if(Duration > 0.3f)
	{
		return ENState::ClimbMove;
	}
	
	return Super::CheckTransition();
}

void UNStateClimbSprint::Init()
{
	Super::Init();

	SetParent(ENState::OnWall);
}

void UNStateClimbSprint::OnEnter()
{
	Super::OnEnter();

	if(Owner && StateMachineComponent)
	{
		// 등반 질주를 위한 Pitch 각도 조정. 명조를 보았을 때 실제 각도는 90도보다 적거나, 모션으로 대체될 수도 있음..
		FQuat NewRotation = FQuat::MakeFromEuler(FVector(0, 90, 0));
		Owner->SetActorRotation(NewRotation);
		
		Owner->GetCharacterMovement()->MaxFlySpeed = 700.f;
	}
}

void UNStateClimbSprint::OnLeave()
{
	Super::OnLeave();

	if(Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->MaxFlySpeed = 200.f;

		// 등반 질주 시작 시의 pitch 조정을 reset
		FQuat NewRotation = FQuat::MakeFromEuler(FVector::ZeroVector);
		Owner->SetActorRotation(NewRotation);
	}
}

ENState UNStateClimbSprint::CheckTransition()
{
	if(!Owner)
	{
		return ENState::None;
	}

	if(auto inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		// 방향키를 떼면 다시 등반 상태로 변경
		if(inputSubsystem->MovementVector.IsNearlyZero())
		{
			return ENState::ClimbMove;
		}
	}

	// 캐릭터 발밑(조금 더 앞에서)에 등반 지형을 체크
	FVector startLoc = Owner->GetActorLocation() - Owner->GetActorUpVector() * Owner->GetDefaultHalfHeight() + Owner->GetActorForwardVector() * Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector endLoc = startLoc + -Owner->GetActorUpVector() * 20.0f;
	TArray<AActor*> actorsToIgnore;
	FHitResult hitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		startLoc, endLoc, 15.f,
		// TraceChannel::ClimbCheck
		ETraceTypeQuery::TraceTypeQuery3, false, actorsToIgnore, EDrawDebugTrace::ForDuration,
		hitResult, true, FLinearColor::Green, FLinearColor::Red, 1.f );
	if(!hitResult.bBlockingHit)
	{
		return ENState::OnGround;
	}
	
	return Super::CheckTransition();
}

void UNStateClimbEnd::Init()
{
	Super::Init();

	SetParent(ENState::OnWall);
}

ENState UNStateClimbEnd::CheckTransition()
{
	if(Duration > 1.13f)
	{
		return ENState::OnGround;
	}
	
	return Super::CheckTransition();
}
