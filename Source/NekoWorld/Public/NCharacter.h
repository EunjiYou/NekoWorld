// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class NEKOWORLD_API ANCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void OnInputMove(const FInputActionValue& Value);
	void OnInputLook(const FInputActionValue& Value);
	void OnInputJump(const FInputActionValue& Value);
	void OnInputJumpEnd(const FInputActionValue& Value);
	
private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;
	
	UInputMappingContext* IMC_Default;
	
	UInputAction* IA_Look;
	UInputAction* IA_Move;
	UInputAction* IA_Jump;
};
