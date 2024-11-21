// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NAnimNotifyState.h"
#include "NAnimNotify_CancelAction.generated.h"

/**
 * 
 */
UCLASS()
class UNAnimNotify_CancelAction : public UNAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
public:
	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "ENActionInputType"))
	uint32 ActionInputType;
};
