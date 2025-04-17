#pragma once

#include "CoreMinimal.h"
#include "NStateBase.h"
#include "NStateOnGround.generated.h"


UCLASS()
class UNStateOnGround : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void OnEnter() override;
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

// DashCount에 따른 상태 변화를 위해 Dash 상태를 분리 (StateMachineComponent->TransitionData.DashCount 참고)
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
	virtual void OnUpdate(float DeltaTime) override;
	virtual ENState CheckTransition() override;
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

//Unreal 자체에서 해주는 Floor 처리가 Sliding과 유사함. 자체적으로는 캐릭터 회전 Block만 할 것
UCLASS()
class UNStateSliding : public UNStateBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnEnter() override;
	virtual void OnLeave() override;
	virtual ENState CheckTransition() override;
};
