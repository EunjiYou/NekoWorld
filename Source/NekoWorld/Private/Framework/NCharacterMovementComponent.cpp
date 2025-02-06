// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/NCharacterMovementComponent.h"
#include "GameFramework/Character.h"

FRotator UNCharacterMovementComponent::ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime,
                                                                       FRotator& DeltaRotation) const
{
	// 활강 같은 상태에서는 기존의 회전 움직임을 유지
	if(MovementMode == MOVE_Falling)
	{
		return Super::ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRotation);	
	}
	else
	{
		// Acceleration 여부와 관계없이 마지막 Input으로 들어온 Direction 방향으로 회전하도록 한다.
		// Lerp는 내가 하지 않아도 MovementComponent에서 알아서 RotationRate에 따른 이동량을 계산해줌
		return DesiredMoveDir.GetSafeNormal().Rotation();		
	}
}

void UNCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	// 수영, 공중 등의 상황에서는 DesiredMoveDir을 사용하지 않으므로, 다시 전환되었을 때 꼭 초기화 해주어야 함
	if(MovementMode == MOVE_Walking)
	{
		DesiredMoveDir = GetCharacterOwner()->GetActorForwardVector();
	}
}
