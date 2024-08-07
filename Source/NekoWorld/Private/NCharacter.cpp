// Fill out your copyright notice in the Description page of Project Settings.


#include "NCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "NInputSubsystem.h"
#include "NStateMachineComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


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

	// State Machine
	StateMachineComponent = CreateDefaultSubobject<UNStateMachineComponent>("StateMachineComp");

	// Movement Setting
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

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

	if(UNInputSubsystem* inputSubsystem = GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		if(APlayerController* pc = Cast<APlayerController>(Controller))
		{
			inputSubsystem->RegisterInputMappingContext(pc->GetLocalPlayer());
			inputSubsystem->EventInputLook.AddUObject(this, &ANCharacter::OnInputLook);
		}
	}
}

// Called to bind functionality to input
void ANCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UNInputSubsystem* inputSubsystem = GetGameInstance()->GetSubsystem<UNInputSubsystem>())
	{
		inputSubsystem->BindInputAction(PlayerInputComponent);
	}
}

void ANCharacter::MoveCharacter(const FVector2D MovementVector)
{
	FRotator controlRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	FVector moveDirX = UKismetMathLibrary::GetForwardVector(controlRotation) * MovementVector.Y;
	FVector moveDirY = UKismetMathLibrary::GetRightVector(controlRotation) * MovementVector.X;
	FVector resultMoveDir = moveDirX + moveDirY;
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (resultMoveDir * 100.0f), FColor::Red, false, 1.0f);
	AddMovementInput(resultMoveDir, 1.f);
}

void ANCharacter::OnInputLook(const FInputActionValue& Value)
{
	FVector2D lookVector = Value.Get<FVector2D>();
	AddControllerYawInput(lookVector.X);
	AddControllerPitchInput(lookVector.Y);
}

void ANCharacter::JumpCharacter()
{
	Jump();
}

void ANCharacter::OnInputJumpEnd(const FInputActionValue& Value)
{
	StopJumping();
}