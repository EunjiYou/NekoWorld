// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NStateMachineComponent.generated.h"

enum class ENState : uint8;
class UNStateBase;

// StateMachineViewer UI용 데이터
USTRUCT(BlueprintType)
struct FNStateDebugData
{
	GENERATED_BODY();

	ENState MyState;
	// ENState Parent;
	TArray<FNStateDebugData> Children;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEKOWORLD_API UNStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNStateMachineComponent();

	UFUNCTION(BlueprintCallable)
	void Init();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetState(ENState NewState);
	UNStateBase* GetState(ENState State);

	FNStateDebugData GenerateDebugDataRecursive(ENState state);
	UFUNCTION(BlueprintCallable)
	TArray<FNStateDebugData> GenerateDebugData();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
public:
	TMap<ENState, TSubclassOf<UNStateBase>> StateClassMap;
	
	UPROPERTY(Transient)
	TMap<ENState, UNStateBase*> StatePool;

	UPROPERTY(VisibleAnywhere)
	UNStateBase* CurState;
	
	UPROPERTY(VisibleAnywhere)
	UNStateBase* PrevState;
	
	UPROPERTY(VisibleAnywhere)
	UNStateBase* ReservedState;
};
