// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NCharacterMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEKOWORLD_API UNCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// 원신같은 휙 도는 회전 구현을 위한 Rotation 계산식 수정
	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const override;

public:
	UPROPERTY()
	FVector DesiredMoveDir;
};
