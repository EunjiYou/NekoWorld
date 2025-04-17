// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NStateBase.h"
#include "NStateOnWater.generated.h"

/**
 * 
 */
UCLASS()
class UNStateOnWater : public UNStateBase
{
	GENERATED_BODY()

private:
	virtual void OnEnter() override;
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateSwimming : public UNStateBase
{
	GENERATED_BODY()
	
private:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual ENState CheckTransition() override;

private:
	bool IsNaturalTurning;
	bool ApplyBrakingDeceleration;
};

UCLASS()
class UNStateSwimmingSprint : public UNStateBase
{
	GENERATED_BODY()

private:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateDivingEnd : public UNStateBase
{
	GENERATED_BODY()

private:
	virtual void Init() override;
	virtual ENState CheckTransition() override;
};
