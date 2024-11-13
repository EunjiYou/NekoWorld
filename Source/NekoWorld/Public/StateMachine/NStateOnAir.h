#pragma once

#include "NStateBase.h"
#include "NStateOnAir.generated.h"

UCLASS()
class UNStateOnAir : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateJump : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual void OnLeave() override;
	virtual ENState CheckTransition() override;

	UFUNCTION()
	void OnReachedJumpApex();

public:
	bool CharacterReachedJumpApex;
};

UCLASS()
class UNStateFalling : public UNStateBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
};

UCLASS()
class UNStateGliding : public UNStateBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnLeave() override;

	virtual ENState CheckTransition() override;

private:
	bool IsDirectionalTurning;
};