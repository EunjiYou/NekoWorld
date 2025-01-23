// Fill out your copyright notice in the Description page of Project Settings.


#include "NCharacter.h"
#include "InputAction.h"
#include "KismetTraceUtils.h"
#include "NCharacterAnimInstance.h"
#include "SubSystem/NInputSubsystem.h"
#include "StateMachine/NStateMachineComponent.h"
#include "Camera/CameraComponent.h"
#include "Framework/NCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ANCharacter::ANCharacter(const FObjectInitializer& ObjectInitializer)
// CharacterMovement를 NCharacterMovementComponent로 교체
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
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
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1500.f, 0.f);
	NCharacterMovementComponent = Cast<UNCharacterMovementComponent>(GetCharacterMovement());
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
void ANCharacter::OnInputMove(const FInputActionValue& Value)
{
	FVector2D inputVector = Value.Get<FVector2D>();
	if(inputVector.IsNearlyZero())
	{
		MoveVector = FVector::ZeroVector;
		StateMachineComponent->TransitionData.HitWall_OnMoveForward = false;
		return;
	}

	// 캐릭터가 등반 상태가 아닐 경우 
	if(!StateMachineComponent->IsChildStateOf(StateMachineComponent->GetCurState(), ENState::OnWall, true))
	{
		// Controller 회전 방향 기준으로 InputVector만큼 이동
		FVector moveVector = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).TransformVector(FVector(inputVector.Y, inputVector.X, 0.f));
		MoveVector = moveVector;
		
		// 보간 회전 처리
		if(NCharacterMovementComponent)
		{
			NCharacterMovementComponent->DesiredMoveDir = MoveVector;
		}
		
		// 등반 가능 상태 체크 (보간 회전 중에 Trace에 걸리면 등반이 이상하게 진행되므로, 보간 회전 완료 한 상태에서만 체크)
		if(FVector::DotProduct(moveVector, GetActorForwardVector()) > 0.98f)
		{
			// 캐릭터 중앙 기준으로 앞에 벽이 있는 지 Trace 진행
			TArray<AActor*> actorsToIgnore;
			FHitResult hitResult;
			UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
				GetActorLocation(), GetActorLocation() + (moveVector * 30.f), 15.f,
				// TraceChannel::ClimbCheck
				ETraceTypeQuery::TraceTypeQuery3, false, actorsToIgnore, EDrawDebugTrace::ForDuration,
				hitResult, true, FLinearColor::Green, FLinearColor::Red, 1.f );

			// Trace 결과로 등반 가능한 지를 체크하여 State Transition Data로 세팅
			StateMachineComponent->TransitionData.HitWall_OnMoveForward = IsClimbable(hitResult);
			StateMachineComponent->TransitionData.HitWall_Normal = hitResult.ImpactNormal;
		}
	}
	else
	{
		// 등반 중에는 Y,Z 축 이동
		MoveVector = FVector(0.f, inputVector.X, inputVector.Y);		
	}

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (MoveVector * 100.0f), FColor::Red, false, 1.0f);
}

void ANCharacter::OnInputLook(const FInputActionValue& Value)
{
	FVector2D lookVector = Value.Get<FVector2D>();
	AddControllerYawInput(lookVector.X);
	AddControllerPitchInput(lookVector.Y);
}

bool ANCharacter::IsClimbable(const FHitResult& Hit)
{
	// Hit가 되지 않았으면 false
	if (!Hit.IsValidBlockingHit())
	{
		return false;
	}

	// Hit된 벽면 법선벡터로 경사를 구하여 등반 각도 체크
	float dot = FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector);
	float resultAngle = FMath::RadiansToDegrees(FMath::Acos(dot));
	if (resultAngle > StateMachineComponent->TransitionData.GetClimbStartAngle()
		&& resultAngle < StateMachineComponent->TransitionData.GetClimbCancelAngle())
	{
		return true;
	}

	return false;
}

void ANCharacter::ClimbingJump(bool forceJumpToUp)
{
	// 입력값이 없을 때는 위 점프 고정, 옆 이동 중일땐 이동값을 활용
	FVector jumpVector;
	if(forceJumpToUp)
	{
		jumpVector = GetActorUpVector();
	}
	else
	{
		jumpVector = MoveVector.IsNearlyZero()? GetActorUpVector() : MoveVector;
	}
	
	LaunchCharacter(jumpVector * StateMachineComponent->StateData.GetClimbJumpSpeed(), true, true);
}

void ANCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(!MoveVector.IsNearlyZero())
	{
		AddMovementInput(MoveVector, 1.f);
	}
}
