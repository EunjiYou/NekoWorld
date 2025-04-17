// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/NInputSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Common/CommonEnum.h"


void UNInputSubsystem::RegisterInputMappingContext(const ULocalPlayer* LocalPlayer)
{
	if(!LocalPlayer)
	{
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if(!subsystem)
	{
		return;
	}
	
	FString iMCPath = TEXT("/Game/Blueprint/Input/IMC_Move.IMC_Move");
	UInputMappingContext* imc_move = Cast<UInputMappingContext>(StaticLoadObject(UInputMappingContext::StaticClass(), nullptr, *iMCPath));
	subsystem->AddMappingContext(imc_move, 0);
}

void UNInputSubsystem::BindInputAction(UInputComponent* PlayerInputComponent)
{
	if(!PlayerInputComponent)
	{
		return;
	}
	
	UEnhancedInputComponent* eiComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(!eiComp)
	{
		return;
	}
	
	if(UInputAction* ia_Move = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move"))))
	{
		eiComp->BindAction(ia_Move, ETriggerEvent::Triggered, this, &UNInputSubsystem::OnInputMove);
		eiComp->BindAction(ia_Move, ETriggerEvent::Completed, this, &UNInputSubsystem::OnInputMove);
	}

	if(UInputAction* ia_Look = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look"))))
	{
		eiComp->BindAction(ia_Look, ETriggerEvent::Triggered, this, &UNInputSubsystem::OnInputLook);
	}
	
	if(UInputAction* ia_Jump = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump"))))
	{
		eiComp->BindAction(ia_Jump, ETriggerEvent::Triggered, this, &UNInputSubsystem::OnInputJump);
		eiComp->BindAction(ia_Jump, ETriggerEvent::Completed, this, &UNInputSubsystem::OnInputJump);
	}
		
	if(UInputAction* ia_WalkRunToggle = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Blueprint/Input/Actions/IA_RunWalkToggle.IA_RunWalkToggle"))))
	{
		eiComp->BindAction(ia_WalkRunToggle, ETriggerEvent::Triggered, this, &UNInputSubsystem::OnInputWalkRunToggle);
	}

	// Dash Trigger : Down
	// 대시 키 입력, 해제 순간을 받기 위한 Trigger 세팅 
	if(UInputAction* ia_Dash = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Blueprint/Input/Actions/IA_Dash.IA_Dash"))))
	{
		eiComp->BindAction(ia_Dash, ETriggerEvent::Started, this, &UNInputSubsystem::OnInputDash);
		eiComp->BindAction(ia_Dash, ETriggerEvent::Completed, this, &UNInputSubsystem::OnInputDash);
	}

	if(UInputAction* ia_ClimbCancel = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Blueprint/Input/Actions/IA_ClimbCancel.IA_ClimbCancel"))))
	{
		eiComp->BindAction(ia_ClimbCancel, ETriggerEvent::Triggered, this, &UNInputSubsystem::OnInputClimbCancel);
		eiComp->BindAction(ia_ClimbCancel, ETriggerEvent::Completed, this, &UNInputSubsystem::OnInputClimbCancel);
	}
}

bool UNInputSubsystem::IsActionInputPressed(ENActionInputType action)
{
	return ActionInputButton & static_cast<uint32>(action);
}

void UNInputSubsystem::OnInputMove(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();
	EventInputMove.Broadcast(Value);
}

void UNInputSubsystem::OnInputLook(const FInputActionValue& Value)
{
	EventInputLook.Broadcast(Value);
}

void UNInputSubsystem::OnInputJump(const FInputActionValue& Value)
{
	SetActionInputButton(ENActionInputType::Jump, Value.Get<bool>());
}

void UNInputSubsystem::OnInputWalkRunToggle(const FInputActionValue& Value)
{
	IsRun = !IsRun;
}

void UNInputSubsystem::OnInputDash(const FInputActionValue& Value)
{
	SetActionInputButton(ENActionInputType::Dash, Value.Get<bool>());
}

void UNInputSubsystem::OnInputClimbCancel(const FInputActionValue& Value)
{
	bool isClimbCancel = Value.Get<bool>();

	SetActionInputButton(ENActionInputType::ClimbCancel, isClimbCancel);
}

void UNInputSubsystem::SetActionInputButton(ENActionInputType action, bool isPressed)
{
	if(isPressed)
	{
		ActionInputButton |= static_cast<uint32>(action);	
	}
	else
	{
		ActionInputButton &= ~(static_cast<uint32>(action));
	}
	EventInputAction.Broadcast(action);
}
