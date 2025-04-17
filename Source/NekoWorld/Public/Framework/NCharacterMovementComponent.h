// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NCharacterMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// 원신같은 휙 도는 회전 구현을 위한 Rotation 계산식 수정
	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const override;
	
	virtual void PhysicsVolumeChanged(class APhysicsVolume* NewVolume) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	// CharacterMovementComponent에서의 IsInWater 기준을 원신 방식으로 수정
	virtual bool IsInWater() const override;
	
protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

public:
	UPROPERTY()
	FVector DesiredMoveDir;

private:
	bool bIsInWaterVolume;
};
