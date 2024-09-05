// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/NInputSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

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
	
	if(UInputAction* ia_Dash = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/Game/Blueprint/Input/Actions/IA_Dash.IA_Dash"))))
	{
		eiComp->BindAction(ia_Dash, ETriggerEvent::Triggered, this, &UNInputSubsystem::OnInputDash);
		eiComp->BindAction(ia_Dash, ETriggerEvent::Completed, this, &UNInputSubsystem::OnInputDash);
	}
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
	JumpKeyPressed = Value.Get<bool>();
}

void UNInputSubsystem::OnInputWalkRunToggle(const FInputActionValue& Value)
{
	IsRun = !IsRun;
}

void UNInputSubsystem::OnInputDash(const FInputActionValue& Value)
{
	DashKeyPressed = Value.Get<bool>();
}
