// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ENActionInputType : uint8
{
	None		= 0 UMETA(Hidden),
	Dash		= 1,
	Jump		= 2,
	ClimbCancel = 3,
	Max			= 4 UMETA(Hidden)	// BitArray 크기를 위한 Max 지정
};