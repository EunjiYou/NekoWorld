#pragma once

#include "CoreMinimal.h"
#include "NStateBase.h"
#include "NStateOnGround.generated.h"


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
class UNStateWalkRun : public UNStateBase
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

public:
	virtual void Init() override;
	virtual void OnEnter() override;
};

UCLASS()
class UNStateRun : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnEnter() override;
};

UCLASS()
class UNStateDash : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};

UCLASS()
class UNStateDashStart : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual ENState CheckTransition() override;
};

UCLASS()
class UNStateDashEnd : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual ENState CheckTransition() override;

public:
	float Duration;
};

UCLASS()
class UNStateSprint : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual ENState CheckTransition() override;
};
