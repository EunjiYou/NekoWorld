// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NCharacterAnimInstance.generated.h"

enum class ENState : uint8;
class ANCharacter;
/**
 * 
 */
UCLASS()
class NEKOWORLD_API UNCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	float MoveSpeed;
	
	UPROPERTY(BlueprintReadOnly)
	ENState CurCharacterState;

	UPROPERTY(BlueprintReadOnly)
	ANCharacter* OwnerCharacter;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};