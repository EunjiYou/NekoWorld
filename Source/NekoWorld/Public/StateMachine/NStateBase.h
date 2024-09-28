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
	
	// Debug������ ����ϴ� ������
	TArray<ENState> Children;
	
	// �� State�� Cancel�� �� �ִ� �Է� Ÿ��. CancelActionNotify�κ��� ���õ�
	bool HasCancelActionInput;
	TArray<ENActionInputType> CancelActionInputs;

	// CancelActionInputs �� �� State�� ���� �Է� Ÿ��. �� Tick�� �Է��� �������� ������ ��� �� ������ �Է����� �����Ѵ�. 
	ENActionInputType ReceivedCancelAction;

protected:
	ANCharacter* Owner;
	UNStateMachineComponent* StateMachineComponent;
};