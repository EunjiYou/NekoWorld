// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NStateBase.h"
#include "Components/ActorComponent.h"
#include "NStateMachineComponent.generated.h"

enum class ENActionInputType : uint8;
enum class ENState : uint8;
class UNStateBase;

// StateMachineViewer UI용 데이터
USTRUCT(BlueprintType)
struct FNStateDebugData
{
	GENERATED_BODY()

	ENState MyState;
	// ENState Parent;
	TArray<FNStateDebugData> Children;
};

USTRUCT()
struct FNStateTransitionData
{
	GENERATED_BODY()

	FNStateTransitionData()
		: DashCount(0)
		, HitWall_OnMoveForward(false)
	{
	}

	UPROPERTY(VisibleAnywhere)
	int DashCount;
	
	UPROPERTY(VisibleAnywhere)
	bool HitWall_OnMoveForward;
	
	float GetClimbStartAngle() { return 80.0f; }
	float GetClimbCancelAngle() { return 110.0f; }
};

USTRUCT()
struct FNStateData
{
	GENERATED_BODY()

	float GetClimbJumpSpeed() { return 500.0f; }
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStateChangeDel, ENState, ENState); // CurState, NextState

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEKOWORLD_API UNStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNStateMachineComponent();

	void Init();
	void RegisterState();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UNStateBase* GetState(ENState State);

	void OnInputAction(const ENActionInputType actionInputType);

	ENState GetCurState() { return CurStateObj? CurStateObj->MyState : ENState::None; }
	ENState GetPrevState() { return PrevStateObj? PrevStateObj->MyState : ENState::None; }

	// 특정 state의 자식인 지를 판단
	// recursiveCheck : parent가 none이 될 때 까지 검색할 지 여부
	bool IsChildStateOf(ENState targetState, ENState parentState, bool recursiveCheck = false);

	// 특정 state에 대한 Parent를 전부 얻어옴
	TArray<ENState> GetAllParentState(ENState targetState);
	
#if WITH_EDITORONLY_DATA
	FNStateDebugData GenerateDebugDataRecursive(ENState state);
	TArray<FNStateDebugData> GenerateDebugData();
#endif

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void SetState(ENState NewState);
	
public:
	TMap<ENState, TSubclassOf<UNStateBase>> StateClassMap;
	
	UPROPERTY(Transient)
	TMap<ENState, UNStateBase*> StatePool;
	
	UPROPERTY()
	UNStateBase* CurStateObj;

	UPROPERTY()
	UNStateBase* PrevStateObj;
	
	UPROPERTY(VisibleAnywhere)
	UNStateBase* ReservedState;

	UPROPERTY(VisibleAnywhere)
	FNStateTransitionData TransitionData;

	UPROPERTY(VisibleAnywhere)
	FNStateData StateData;

	FOnStateChangeDel OnStateChange;
};
