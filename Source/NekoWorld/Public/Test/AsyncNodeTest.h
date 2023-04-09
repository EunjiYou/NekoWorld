// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncNodeTest.generated.h"

// 1.Async 형태의 노드 만드는 방법 테스트
	// UBlueprintAsyncActionBase/UGameplayTask, Latent
// 2.Multiple Input/Output Excution Pin을 넣을 수 있는 방법 테스트
	// Multiple Input = Latent
	// Multiple Output = UBlueprintAsyncActionBase, UGameplayTask
	// Multiple Input + Output Pin은 만들 수가 없었다.
		// K2Node는 만들 수 있는가?

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutputPin);

UCLASS()
class NEKOWORLD_API UMultipleOutput : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObj", DefaultToSelf = "WorldContextObj"))
		static UMultipleOutput* MultipleOutputNode(UObject* WorldContextObj);

private:
	UFUNCTION()
		void BeginPlay();

public:
	UPROPERTY(BlueprintAssignable)
		FOutputPin OnComplete;
};



// Multiple InputPin을 만들 수 있는 방법
// 핵심 키워드는 Latent와 ExpandEnumAsExecs
// Multiple OutputPin보다 과정이 복잡하고, OutputPin을 만들 수 있는 방법이 없다.

UENUM(BlueprintType)
enum MultipleInputPinType
{
	Type1,
	Type2
};

class FInputAction : public FPendingLatentAction
{
public:
	FInputAction(const FLatentActionInfo LatentInfo)
		: ExecutionFunction(LatentInfo.ExecutionFunction)
		, Outputlink(LatentInfo.Linkage)
		, CallbackTarget(LatentInfo.CallbackTarget)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, Outputlink, CallbackTarget);
	}

private:
	FName ExecutionFunction;
	int Outputlink;
	FWeakObjectPtr CallbackTarget;
};


UCLASS()
class UMultipleInput : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObj", DefaultToSelf = "WorldContextObj", ExpandEnumAsExecs = "InputPins"))
		static void MultipleInputNodeWithLatent(UObject* WorldContextObj, TEnumAsByte<MultipleInputPinType> InputPins, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObj", DefaultToSelf = "WorldContextObj", ExpandEnumAsExecs = "InputPins"))
		static void MultipleInputNode(UObject* WorldContextObj, TEnumAsByte<MultipleInputPinType> InputPins, FLatentActionInfo LatentInfo);
};
