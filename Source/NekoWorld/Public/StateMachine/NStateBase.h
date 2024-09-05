// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NStateBase.generated.h"

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
	
	virtual void OnEnter() {}
	virtual void OnUpdate(float DeltaTime) {}
	virtual void OnLeave() {}

	virtual ENState CheckTransition() { return ENState::None; }

	void SetParent(ENState ParentState);
	
public:
	ENState Parent;
	ENState MyState;
	
	// Debug용으로 사용하는 데이터
	TArray<ENState> Children;
	
	
	
protected:
	ANCharacter* Owner;
	UNStateMachineComponent* StateMachineComponent;
};