// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncNodeTest.generated.h"

// 1.Async ������ ��� ����� ��� �׽�Ʈ
	// UBlueprintAsyncActionBase/UGameplayTask, Latent
// 2.Multiple Input/Output Excution Pin�� ���� �� �ִ� ��� �׽�Ʈ
	// Multiple Input = Latent
	// Multiple Output = UBlueprintAsyncActionBase, UGameplayTask
	// Multiple Input + Output Pin�� ���� ���� ������.
		// K2Node�� ���� �� �ִ°�?

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



// Multiple InputPin�� ���� �� �ִ� ���
// �ٽ� Ű����� Latent�� ExpandEnumAsExecs
// Multiple OutputPin���� ������ �����ϰ�, OutputPin�� ���� �� �ִ� ����� ����.

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
