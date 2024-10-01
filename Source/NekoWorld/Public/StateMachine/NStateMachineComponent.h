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
	{}

	UPROPERTY(VisibleAnywhere)
	int DashCount;
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

	FOnStateChangeDel OnStateChange;
};
