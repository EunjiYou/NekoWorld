// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/NCharacterMovementComponent.h"


FRotator UNCharacterMovementComponent::ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime,
	FRotator& DeltaRotation) const
{
	// Acceleration 여부와 관계없이 마지막 Input으로 들어온 Direction 방향으로 회전하도록 한다.
	// Lerp는 내가 하지 않아도 MovementComponent에서 알아서 RotationRate에 따른 이동량을 계산해줌
	return DesiredMoveDir.GetSafeNormal().Rotation();
}
