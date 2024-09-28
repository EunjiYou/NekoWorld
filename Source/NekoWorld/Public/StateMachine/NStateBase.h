// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NStateBase.generated.h"

enum class ENActionInputType : uint8;
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
	Sprint,
	OnAir,
	Jump,
	Falling,
};

UCLASS()
class UNStateBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init();
	
	virtual void OnEnter();
	virtual void OnUpdate(float DeltaTime) {}
	virtual void OnLeave() {}

	virtual ENState CheckTransition() { return ENState::None; }

	void SetParent(ENState ParentState);

	void SetInputCancelAction(ENActionInputType actionInputType)
	{
		HasCancelActionInput = true;
		CancelActionInputs.AddUnique(actionInputType);
	}
	
public:
	ENState Parent;
	ENState MyState;
	
	// Debug용으로 사용하는 데이터
	TArray<ENState> Children;
	
	// 이 State를 Cancel할 수 있는 입력 타입. CancelActionNotify로부터 세팅됨
	bool HasCancelActionInput;
	TArray<ENActionInputType> CancelActionInputs;

	// CancelActionInputs 중 이 State가 받은 입력 타입. 한 Tick에 입력이 여러개가 들어왔을 경우 맨 마지막 입력으로 세팅한다. 
	ENActionInputType ReceivedCancelAction;

protected:
	ANCharacter* Owner;
	UNStateMachineComponent* StateMachineComponent;
};