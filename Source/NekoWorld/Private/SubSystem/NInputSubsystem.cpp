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
	
	FString iMCPath = TEXT("/Game/ThirdPerson/Input/IMC_Default.IMC_Default");
	UInputMappingContext* imc_Default = Cast<UInputMappingContext>(StaticLoadObject(UInputMappingContext::StaticClass(), nullptr, *iMCPath));
	subsystem->AddMappingContext(imc_Default, 0);
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
