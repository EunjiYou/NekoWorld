// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NInputSubsystem.generated.h"

struct FInputActionValue;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputDel, const FInputActionValue&);

UCLASS()
class NEKOWORLD_API UNInputSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void RegisterInputMappingContext(const ULocalPlayer* LocalPlayer);
	void BindInputAction(UInputComponent* PlayerInputComponent);

private:
	void OnInputMove(const FInputActionValue& Value);
	void OnInputLook(const FInputActionValue& Value);
	void OnInputJump(const FInputActionValue& Value);
	void OnInputWalkRunToggle(const FInputActionValue& Value);
	void OnInputDash(const FInputActionValue& Value);

public:
	FOnInputDel EventInputMove;
	FOnInputDel EventInputLook;
	
	FVector2D MovementVector;
	bool JumpKeyPressed;
	bool DashKeyPressed;
	
	bool IsRun = true;
};
