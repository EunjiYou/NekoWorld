// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/NCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/KismetSystemLibrary.h"

FRotator UNCharacterMovementComponent::ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime,
                                                                       FRotator& DeltaRotation) const
{
	// 활강, 수영 상태에서는 기존의 회전 움직임을 유지
	if(IsFalling() || IsSwimming())
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

void UNCharacterMovementComponent::PhysicsVolumeChanged(APhysicsVolume* NewVolume)
{
	if (!HasValidData())
	{
		return;
	}

	bIsInWaterVolume = NewVolume->bWaterVolume;
	if ( NewVolume && NewVolume->bWaterVolume )
	{
		if ( !IsSwimming() )
		{
			// 원신 방식대로 가슴 부근에서 수영을 하기 위해 SwimmingMode로 바꾸지 않고 Tick으로 체크
			return;
		}
	}
	
	Super::PhysicsVolumeChanged(NewVolume);
}

void UNCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// WaterVolume 내에서 특정 수위 이상에서만 수영하기 위해 체크
	if(bIsInWaterVolume && !IsSwimming() && IsInWater())
	{
		SetMovementMode(MOVE_Swimming);
		bIsInWaterVolume = false;
	}
}

bool UNCharacterMovementComponent::IsInWater() const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return false;
	}

	// 원래 IsInWater 기준과 다르게 여기서는 캐릭터 Chest 정도의 위치가 물에 닿았는 지를 체크한다
	if(const APhysicsVolume* physVolume = GetPhysicsVolume())
	{
		FVector chestPosition = OwnerCharacter->GetActorLocation() + FVector(0.f, 0.f, 40.f);
		return physVolume->bWaterVolume && physVolume->EncompassesPoint(chestPosition); 
	}

	return false;
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