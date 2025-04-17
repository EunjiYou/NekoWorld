// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/NStateOnWater.h"

#include "NCharacter.h"
#include "Common/CommonEnum.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StateMachine/NStateMachineComponent.h"
#include "SubSystem/NInputSubsystem.h"

class UNInputSubsystem;

void UNStateOnWater::OnEnter()
{
	Super::OnEnter();

	if(Owner && Owner->GetCharacterMovement() && StateMachineComponent)
	{
		Owner->GetCharacterMovement()->Buoyancy = 1.3f;
		Owner->GetCharacterMovement()->RotationRate = FRotator(0.f, 100.f, 0.f);
		Owner->GetCharacterMovement()->Velocity.X = 0.f;
		Owner->GetCharacterMovement()->Velocity.Y = 0.f;

		// Velocity Z 값은 설정값에 따라 물에 잠기는 정도가 달라서 더 조건을 두고 세팅
		// 잠기는 정도의 차이를 두진 않을거라 Min,Max 둘 중 하나로만 설정되게 한다
		// 너무 빠른 속도로 들어가면 바로 물 아래로 가라앉으므로 Min까지로만 적용
		if(Owner->GetCharacterMovement()->Velocity.Z < StateMachineComponent->StateData.MinVelocityZSwimming)
		{
			Owner->GetCharacterMovement()->Velocity.Z = StateMachineComponent->StateData.MinVelocityZSwimming;
		}
		// 너무 빠르지만 않으면 전부 Max 속도로 세팅
		else
		{
			Owner->GetCharacterMovement()->Velocity.Z = StateMachineComponent->StateData.MaxVelocityZSwimming;
		}
		
		// 감속은 Z속도 적용을 위해 일단 0으로 세팅. 각자 상황에 따라 나중에 세팅하게 됨
		Owner->GetCharacterMovement()->BrakingDecelerationSwimming = 0.f;
	}
}

ENState UNStateOnWater::CheckTransition()
{
	if(StateMachineComponent->GetCurState() == MyState)
	{
		// 이전이 다이빙이었다면 DivingEnd로 유도. 그 외에는 Swimming 상태로 전환
		if(StateMachineComponent->GetPrevState() == ENState::Diving)
		{
			return ENState::DivingEnd;
		}
		else
		{
			return ENState::Swimming;			
		}
	}

	// 등반 전환
	if(StateMachineComponent && StateMachineComponent->TransitionData.HitWall_OnMoveForward)
	{
		return ENState::OnWall;
	}

	// 물 밖을 벗어난 경우는 OnGround 상태로 전환
	if(Owner && Owner->GetCharacterMovement() && !Owner->GetCharacterMovement()->IsSwimming())
	{
		return ENState::OnGround;
	}
	
	return ENState::None;
}

void UNStateSwimming::Init()
{
	Super::Init();

	SetParent(ENState::OnWater);
}

void UNStateSwimming::OnEnter()
{
	Super::OnEnter();

	ApplyBrakingDeceleration = false;
	if(Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->MaxSwimSpeed = StateMachineComponent->StateData.SpeedOnSwimming;
	}
}

void UNStateSwimming::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

	// Todo : SwimmingSprint 시도 똑같은 코드 처리 필요.. 어떻게?? (OnWater::Update 또는 각자 구현..)
	if(Owner && Owner->GetCharacterMovement() && StateMachineComponent)
	{
		if(!ApplyBrakingDeceleration)
		{
			// 캐릭터 현재 수심에 따른 감속 세팅. 캐릭터가 가라앉았을 때는 감속이 없어야 금방 뜨므로, 어느정도 떴을 때 감속을 적용해준다. (감속 설정 안 해주면 이동키 입력이 없어도 이동해버림)
			if(Owner->GetCharacterMovement()->ImmersionDepth() <= 0.8f)
			{
				Owner->GetCharacterMovement()->BrakingDecelerationSwimming = StateMachineComponent->StateData.BrakingDecelerationSwimming;
				ApplyBrakingDeceleration = true;
			}
		}
		
		auto swimmingMoveVector = FVector::ZeroVector;

		// 극단적인 방향 회전에 대해 원신처럼 약간의 회전 보정을 진행 (naturalTurning)
		float dotProduct = FVector::DotProduct(Owner->MoveVector, Owner->GetActorForwardVector());
		float naturalTurningStartAngle = FMath::Cos(FMath::DegreesToRadians(120.f));
		float naturalTurningEndAngle = FMath::Cos(FMath::DegreesToRadians(60.f));
		if(Owner->MoveVector.IsNearlyZero())
		{
			IsNaturalTurning = false;	
		}
		else if(dotProduct < naturalTurningStartAngle	// 회전 각도가 StartAngle로부터 시작해서 EndAngle 구간에 있는 경우만
			|| (dotProduct < naturalTurningEndAngle && IsNaturalTurning))
		{
			IsNaturalTurning = true;
			swimmingMoveVector += Owner->GetActorForwardVector() * 0.5f;		// 앞을 누르지 않았어도 약간 앞으로 나가는 듯한 힘을 추가
			// DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 100.0f), FColor::Orange, false, 1.0f);
		}
		else
		{
			IsNaturalTurning = false;
		}
	
		Owner->AddMovementInput(swimmingMoveVector);
	}
}

ENState UNStateSwimming::CheckTransition()
{
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		// 방향키/대시키를 전부 입력해둔 상태에서만 수영 스프린트 상태로 전환
		if(inputSubsystem->IsActionInputPressed(ENActionInputType::Dash)
			&& !inputSubsystem->MovementVector.IsNearlyZero())
		{
			return ENState::SwimmingSprint;
		}
	}
	
	return ENState::None;
}

void UNStateSwimmingSprint::Init()
{
	Super::Init();

	SetParent(ENState::OnWater);
}

void UNStateSwimmingSprint::OnEnter()
{
	Super::OnEnter();

	if(Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->MaxSwimSpeed = StateMachineComponent->StateData.SpeedOnSwimmingSprint;
	}
}

ENState UNStateSwimmingSprint::CheckTransition()
{
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		// 방향키/대시 키를 안 누르고 있으면 수영으로 전환
		if(!inputSubsystem->IsActionInputPressed(ENActionInputType::Dash)
			|| inputSubsystem->MovementVector.IsNearlyZero())
		{
			return ENState::Swimming;
		}
	}
	
	return ENState::None;
}

void UNStateDivingEnd::Init()
{
	Super::Init();

	SetParent(ENState::OnWater);
}

ENState UNStateDivingEnd::CheckTransition()
{
	if(Duration > 1.0f)
	{
		return ENState::Swimming;
	}
	
	return ENState::None;
}
