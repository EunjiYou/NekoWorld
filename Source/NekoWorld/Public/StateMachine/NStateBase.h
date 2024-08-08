// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NStateBase.generated.h"

class ANCharacter;
class UNStateMachineComponent;

UENUM()
enum class ENState
{
	None,
	OnGround,
	OnAir,
	Idle,
	Walk,
	Run,
	Jump
};

UCLASS()
class UNStateBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init();
	
	virtual void OnEnter() {}
	virtual void OnUpdate() {}
	virtual void OnLeave() {}

	virtual ENState CheckTransition() { return ENState::None; }

	void SetParent(ENState ParentState);
	
public:
	ENState Parent;
	TArray<ENState> Children;
	ENState MyState;
	
protected:
	ANCharacter* Owner;
	UNStateMachineComponent* StateMachineComponent;
};

UCLASS()
class UNStateOnGround : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateIdle : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateWalk : public UNStateBase
{
	GENERATED_BODY()
};

UCLASS()
class UNStateRun : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnUpdate() override;
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateOnAir : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};

UCLASS()
class UNStateJump : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnUpdate() override;
	virtual ENState CheckTransition() override;
};