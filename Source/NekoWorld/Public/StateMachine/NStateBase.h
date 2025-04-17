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
	
	// �� State�� Cancel�� �� �ִ� �Է� Ÿ��. CancelActionNotify�κ��� ���õ�
	//bool HasCancelActionInput;
	UPROPERTY(Meta = (Bitmask, BitmaskEnum = "ENActionInputType"))
	uint32 CancelActionInputs;

	// CancelActionInputs �� �� State�� ���� �Է� Ÿ��. �� Tick�� �Է��� �������� ������ ��� �� ������ �Է����� �����Ѵ�. 
	ENActionInputType ReceivedCancelAction;

#if WITH_EDITORONLY_DATA
	// Debug������ ����ϴ� ������
	TArray<ENState> Children;
#endif
	
protected:
	ANCharacter* Owner;
	UNStateMachineComponent* StateMachineComponent;
};