// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NInputSubsystem.generated.h"

enum class ENActionInputType : uint8;
struct FInputActionValue;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputDel, const FInputActionValue&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputActionDel, const ENActionInputType);

UCLASS()
class NEKOWORLD_API UNInputSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void RegisterInputMappingContext(const ULocalPlayer* LocalPlayer);
	void BindInputAction(UInputComponent* PlayerInputComponent);

private:
	void OnInputMove(const FInputActionValue& Value);
	void OnInputLook(const FInputActionValue& Value);
	void OnInputJump(const FInputActionValue& Value);
	void OnInputWalkRunToggle(const FInputActionValue& Value);
	void OnInputDash(const FInputActionValue& Value);
	void OnInputClimbCancel(const FInputActionValue& Value);

	void SetActionInputButton(ENActionInputType action, bool isPressed);

public:
	TBitArray<> ActionInputButton;
	// InputActionButton에 대한 변경을 알려줄 Delegate 필요
	FOnInputActionDel EventInputAction;
	
	FOnInputDel EventInputMove;
	FOnInputDel EventInputLook;
	
	FVector2D MovementVector;
	bool JumpKeyPressed;
	bool DashKeyPressed;
	
	bool IsRun = true;
};
