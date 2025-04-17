#include "StateMachine/NStateOnAir.h"

#include "NCharacter.h"
#include "Common/CommonEnum.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StateMachine/NStateMachineComponent.h"
#include "SubSystem/NInputSubsystem.h"


ENState UNStateOnAir::CheckTransition()
{
	if(!Owner)
	{
		return ENState::None;
	}
	
	if(StateMachineComponent && StateMachineComponent->TransitionData.HitWall_OnMoveForward)
    {
    	return ENState::OnWall;
    }

	if(Owner->GetCharacterMovement())
	{
		if(Owner->GetCharacterMovement()->IsSwimming())
		{
			return ENState::OnWater;
		}
		else if(!Owner->GetCharacterMovement()->IsFalling())
		{
			return ENState::Idle;
		}
	}

	// Falling -> Gliding 전환 체크
	if(StateMachineComponent->GetCurState() == ENState::Falling)
	{
		if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
		{
			if(inputSubsystem->IsActionInputPressed(ENActionInputType::Jump))
			{
				// 둘 다 지형이 점프 높이보다 조오금 아래에 있는 경우만 Gliding으로 전환
				FVector startPos = Owner->GetActorLocation() - FVector(0.f, 0.f, Owner->GetDefaultHalfHeight());
				FVector endPos = startPos - Owner->GetActorUpVector() * 100.f;
				FHitResult hitResult;
				TArray<AActor*> actorsToIgnore;
				UKismetSystemLibrary::LineTraceSingle(GetWorld(),
					startPos, endPos,
					// Visibility
					TraceTypeQuery1, false, actorsToIgnore, EDrawDebugTrace::ForDuration,
					hitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f );
		
				if(!hitResult.bBlockingHit)
				{
					return ENState::Gliding;	
				}
			}
		}
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

	// 점프 상태 전환을 위한 정점 체크
	CharacterReachedJumpApex = false;
	Owner->GetCharacterMovement()->bNotifyApex = true;
	Owner->OnReachedJumpApex.AddDynamic(this, &UNStateJump::OnReachedJumpApex);
	
	Owner->Jump();
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

	// Jump -> Falling 전환
	if(CharacterReachedJumpApex)
	{
		return ENState::Falling;
	}

	// Todo : 점프 중 Gliding으로의 전환?
	
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

void UNStateFalling::OnEnter()
{
	Super::OnEnter();

	// 등반 등의 상태에서 돌아올 경우 Flying Movement 상태일 때가 있어서 전환
	if(Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}
}

void UNStateGliding::Init()
{
	Super::Init();

	SetParent(ENState::OnAir);
}

void UNStateGliding::OnEnter()
{
	Super::OnEnter();

	// MovementMode는 Falling 상태를 유지하고, 중력을 변경시켜 서서히 하강하도록 한다
	if(Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		Owner->GetCharacterMovement()->GravityScale = 0.01f;
		Owner->GetCharacterMovement()->RotationRate = FRotator(0.f, 100.f, 0.f);
	}
}

void UNStateGliding::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);
	
	if(Owner && Owner->GetCharacterMovement() && StateMachineComponent)
	{
		auto glidingMoveVector = Owner->MoveVector;

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
			glidingMoveVector += Owner->GetActorForwardVector() * 0.5f;		// 앞을 누르지 않았어도 약간 앞으로 나가는 듯한 힘을 추가
			// DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 100.0f), FColor::Orange, false, 1.0f);
		}
		else
		{
			IsNaturalTurning = false;
		}
	
		// Falling 상태에서는 MoveVector 입력이 없다고 Velocity 리셋이 되지 않으므로 현재 입력 값에 따른 Velocity 조절 필요 
		const float fallingSpeed = StateMachineComponent->StateData.FallingSpeed;
		Owner->GetCharacterMovement()->Velocity.X = glidingMoveVector.X * fallingSpeed;
		Owner->GetCharacterMovement()->Velocity.Y = glidingMoveVector.Y * fallingSpeed;
	}
}

void UNStateGliding::OnLeave()
{
	Super::OnLeave();

	if(Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->GravityScale = 1.f;
		// Owner->GetCharacterMovement()->AirControl = 0.05f;
		// Owner->GetCharacterMovement()->RotationRate = FRotator(0.f, StateMachineComponent? StateMachineComponent->StateData.GroundRotationRateYaw : 100.f, 0.f);
	}
}

ENState UNStateGliding::CheckTransition()
{
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(inputSubsystem->IsActionInputPressed(ENActionInputType::Jump))
		{
			return ENState::Falling;
		}
	}
	
	return Super::CheckTransition();
}

void UNStateDiving::Init()
{
	Super::Init();

	SetParent(ENState::OnAir);
}

void UNStateDiving::OnEnter()
{
	Super::OnEnter();
	
	FVector JumpVelocity = Owner->GetActorForwardVector() * 300.0f + FVector::UnitZ() * 400.0f;
	Owner->LaunchCharacter(JumpVelocity, true, true);
}