// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NStateBase.generated.h"

enum class ENActionInputType : uint32;

class ANCharacter;
class UNStateMachineComponent;

UENUM(BlueprintType)
enum class ENState : uint8
{
	None,
	OnGround,
	Idle,
	WalkRun,
	Walk,
	Run,
	Dash,
	DashStart,
	DashEnd,
	Sprint,
	Sliding,
	OnWall,
	ClimbStart,
	ClimbMove,
	ClimbJump,
	ClimbSprint,
	ClimbEnd,
	OnAir,
	Jump,
	Falling,
	Gliding,
	Diving,
	OnWater,
	Swimming,
	SwimmingSprint,
	DivingEnd,
};

UCLASS()
class UNStateBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init();
	
	virtual void OnEnter();
	virtual void OnUpdate(float DeltaTime) { Duration += DeltaTime; }
	virtual void OnLeave() {}

	virtual ENState CheckTransition() { return ENState::None; }

	void SetParent(ENState ParentState);

	void SetInputCancelAction(UPARAM(Meta=(Bitmask, BitmaskEnum = "ENActionInputType")) uint32 actionInputType)
	{
		//HasCancelActionInput = true;
		CancelActionInputs |= actionInputType;
	}

public:
	ENState Parent;
	ENState MyState;
	float Duration;
	
	// 이 State를 Cancel할 수 있는 입력 타입. CancelActionNotify로부터 세팅됨
	//bool HasCancelActionInput;
	UPROPERTY(Meta = (Bitmask, BitmaskEnum = "ENActionInputType"))
	uint32 CancelActionInputs;

	// CancelActionInputs 중 이 State가 받은 입력 타입. 한 Tick에 입력이 여러개가 들어왔을 경우 맨 마지막 입력으로 세팅한다. 
	ENActionInputType ReceivedCancelAction;

#if WITH_EDITORONLY_DATA
	// Debug용으로 사용하는 데이터
	TArray<ENState> Children;
#endif
	
protected:
	ANCharacter* Owner;
	UNStateMachineComponent* StateMachineComponent;
};