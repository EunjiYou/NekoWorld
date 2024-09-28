// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEKOWORLD_API UNStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNStateMachineComponent();

	void Init();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UNStateBase* GetState(ENState State);

	FNStateDebugData GenerateDebugDataRecursive(ENState state);
	UFUNCTION(BlueprintCallable)
	TArray<FNStateDebugData> GenerateDebugData();

	void OnInputAction(const ENActionInputType actionInputType);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void SetState(ENState NewState);
	
public:
	TMap<ENState, TSubclassOf<UNStateBase>> StateClassMap;
	
	UPROPERTY(Transient)
	TMap<ENState, UNStateBase*> StatePool;


	UPROPERTY(VisibleAnywhere)
	ENState CurState;
	UPROPERTY()
	UNStateBase* CurStateObj;

	UPROPERTY(VisibleAnywhere)
	ENState PrevState;
	UPROPERTY()
	UNStateBase* PrevStateObj;
	
	UPROPERTY(VisibleAnywhere)
	UNStateBase* ReservedState;

	UPROPERTY(VisibleAnywhere)
	FNStateTransitionData TransitionData;
};
