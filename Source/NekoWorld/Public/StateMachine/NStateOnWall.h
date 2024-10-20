// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NStateBase.h"
#include "NStateOnWall.generated.h"

// Todo : IK를 통한 등반 모션 구현 (캐릭터를 벽에 달라붙여야 한다.)
// Todo : 옆 벽면 등반 테스트

UCLASS()
class UNStateOnWall : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void OnEnter() override;
	virtual void OnLeave() override;
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateClimbingStart : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual void OnLeave() override;
	virtual ENState CheckTransition() override;
};


UCLASS()
class UNStateClimbing : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateClimbingJump : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual void OnLeave() override;
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateClimbingEnd : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual ENState CheckTransition() override;
};