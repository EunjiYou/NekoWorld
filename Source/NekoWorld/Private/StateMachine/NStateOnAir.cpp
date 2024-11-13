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

	if(Owner->GetCharacterMovement()
	&& Owner->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		return ENState::Idle;
	}
	
	if(StateMachineComponent->GetCurState() == ENState::Falling)
	{
		if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
		{
			if(inputSubsystem->ActionInputButton[(uint8)ENActionInputType::Jump])
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
		if(inputSubsystem->ActionInputButton[(uint8)ENActionInputType::Jump])
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

void UNStateGliding::Init()
{
	Super::Init();

	SetParent(ENState::OnAir);
}

void UNStateGliding::OnEnter()
{
	Super::OnEnter();

	if(Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		Owner->GetCharacterMovement()->GravityScale = 0.01f;
		// Owner->GetCharacterMovement()->AirControl = 0.7f;
		Owner->GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
	}
}

void UNStateGliding::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

	if(Owner && Owner->GetCharacterMovement())
	{
		//Owner->AddMovementInput(Owner->GetActorForwardVector());
		float dotProduct = FVector::DotProduct(Owner->MoveVector, Owner->GetActorForwardVector());
		UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(dotProduct));
		auto glidingMoveVector = Owner->MoveVector;
		if(dotProduct < -0.7f)
		{
			IsDirectionalTurning = true;
			glidingMoveVector += Owner->GetActorForwardVector() * 0.5f;
			DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 100.0f), FColor::Orange, false, 1.0f);
		}
		else if(dotProduct < 0.f && IsDirectionalTurning)
		{
			glidingMoveVector += Owner->GetActorForwardVector() * 0.5f;
			DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 100.0f), FColor::Orange, false, 1.0f);
		}
		else
		{
			IsDirectionalTurning = false;
		}
		
		const float FallingSpeed = 100.f;
		Owner->GetCharacterMovement()->Velocity.X = glidingMoveVector.X * FallingSpeed;
		Owner->GetCharacterMovement()->Velocity.Y = glidingMoveVector.Y * FallingSpeed;
	}
}

void UNStateGliding::OnLeave()
{
	Super::OnLeave();

	if(Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->GravityScale = 1.f;
		Owner->GetCharacterMovement()->AirControl = 0.05f;
		Owner->GetCharacterMovement()->RotationRate = FRotator(0.f, 1500.f, 0.f);
	}
}

ENState UNStateGliding::CheckTransition()
{
	if(UNInputSubsystem* inputSubsystem = Owner->GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(inputSubsystem->ActionInputButton[(uint8)ENActionInputType::Jump])
		{
			return ENState::Falling;
		}
	}
	
	return Super::CheckTransition();
}
