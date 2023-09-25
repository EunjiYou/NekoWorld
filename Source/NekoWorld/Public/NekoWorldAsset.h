// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NekoWorldAsset.generated.h"

/**
 * 
 */
UCLASS()
class NEKOWORLD_API UNekoWorldAsset : public UObject
{
	GENERATED_BODY()

	UNekoWorldAsset();

public:
	UPROPERTY(EditAnywhere, Category = "Session")
	FString SpeakerName;

	UPROPERTY(VisibleAnywhere, Category = "Session")
	FDateTime SessionStart;

	UPROPERTY(VisibleAnywhere, Category = "Session")
	int SessionDuration;
};
