// Fill out your copyright notice in the Description page of Project Settings.


#include "NCharacter.h"
#include "InputAction.h"
#include "NCharacterAnimInstance.h"
#include "SubSystem/NInputSubsystem.h"
#include "StateMachine/NStateMachineComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		static ConstructorHelpers::FClassFinder<UAnimInstance> animInst(TEXT("/Game/Blueprint/NCharacterAnimBP.NCharacterAnimBP_C"));
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
	// 자연스러운 회전을 위해 이 옵션 대신 회전 보간 적용
	// GetCharacterMovement()->bOrientRotationToMovement = true;
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
			inputSubsystem->EventInputMove.AddUObject(this, &ANCharacter::OnInputMove);
			inputSubsystem->EventInputLook.AddUObject(this, &ANCharacter::OnInputLook);
		}
	}

	NAnimInstance = Cast<UNCharacterAnimInstance>(GetMesh()->GetAnimInstance());
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
	// Controller 회전 방향 기준으로 InputVector만큼 이동
	FVector moveVector = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).TransformVector(FVector(MovementVector.Y, MovementVector.X, 0.f));
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (moveVector * 100.0f), FColor::Red, false, 1.0f);
	NewRotation = FRotationMatrix::MakeFromX(moveVector).Rotator();
	
	AddMovementInput(moveVector, 1.f);
}

void ANCharacter::OnInputMove(const FInputActionValue& Value)
{
	FVector2D movementVector = Value.Get<FVector2D>();
	if(movementVector.IsNearlyZero())
	{
		return;
	}

	MoveCharacter(movementVector);
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

void ANCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(FMath::Abs(GetActorRotation().Yaw - NewRotation.Yaw) > 1.f)
	{
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaSeconds, 20.0f);
		// 새로운 회전 적용
		SetActorRotation(InterpRotation);		
	}
}
