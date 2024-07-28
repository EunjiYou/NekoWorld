// Fill out your copyright notice in the Description page of Project Settings.


#include "NCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ANCharacter::ANCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> characterMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"));
	if(characterMesh.Succeeded() && GetMesh())
	{
		GetMesh()->SetSkeletalMeshAsset(characterMesh.Object.Get());
		static ConstructorHelpers::FClassFinder<UAnimInstance> animInst(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
		if(animInst.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(animInst.Class);
		}

		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}

	// 카메라 설정
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SocketOffset = FVector(0.f, 0.f, 100.f);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComponent->SetupAttachment(SpringArmComponent);

	// 입력 설정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> imcDefault(TEXT("/Game/ThirdPerson/Input/IMC_Default.IMC_Default"));
	if(imcDefault.Succeeded())
	{
		IMC_Default = imcDefault.Object.Get();
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> iaMove(TEXT("/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move"));
	if(iaMove.Succeeded())
	{
		IA_Move = iaMove.Object.Get();
	}
			
	static ConstructorHelpers::FObjectFinder<UInputAction> iaLook(TEXT("/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look"));
	if(iaLook.Succeeded())
	{
		IA_Look = iaLook.Object.Get();
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> iaJump(TEXT("/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump"));
	if(iaJump.Succeeded())
	{
		IA_Jump = iaJump.Object.Get();
	}
}

// Called when the game starts or when spawned
void ANCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* pc = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(IMC_Default, 0);
		}
	}
}

// Called to bind functionality to input
void ANCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(UEnhancedInputComponent* eiComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		eiComp->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ANCharacter::OnInputMove);
		eiComp->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ANCharacter::OnInputJump);
		eiComp->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ANCharacter::OnInputJumpEnd);
		eiComp->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ANCharacter::OnInputLook);
	}
}

void ANCharacter::OnInputMove(const FInputActionValue& Value)
{
	FVector2D movementVector = Value.Get<FVector2D>();
	float movementScale = FMath::Max(1.0f, movementVector.SquaredLength());
	FVector moveDir = GetActorForwardVector() * movementVector.Y + GetActorRightVector() * movementVector.X;
	// FVector moveDir = FVector(movementVector.Y, movementVector.X, 0.f);
	AddMovementInput(moveDir, movementScale);
}

void ANCharacter::OnInputLook(const FInputActionValue& Value)
{
	FVector2D lookVector = Value.Get<FVector2D>();
	AddControllerYawInput(lookVector.X);
	AddControllerPitchInput(lookVector.Y);
}

void ANCharacter::OnInputJump(const FInputActionValue& Value)
{
	Jump();
}

void ANCharacter::OnInputJumpEnd(const FInputActionValue& Value)
{
	StopJumping();
}