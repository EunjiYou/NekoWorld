// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NCharacter.generated.h"

class UNCharacterAnimInstance;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UNStateMachineComponent;

UCLASS()
class ANCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveCharacter(const FVector2D MovementVector);
	void JumpCharacter();

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void OnInputMove(const FInputActionValue& Value);
	void OnInputLook(const FInputActionValue& Value);

public:
	UPROPERTY()
	UNCharacterAnimInstance* NAnimInstance;
	
private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnyWhere)
	UNStateMachineComponent* StateMachineComponent;

	FRotator NewRotation;
};
