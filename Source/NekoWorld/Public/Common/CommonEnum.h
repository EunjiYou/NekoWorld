// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ENActionInputType : uint32
{
	None		= 1<<0 UMETA(Hidden),
	Dash		= 1<<1,
	Jump		= 1<<2,
	ClimbCancel = 1<<3,
	Max			= 4 UMETA(Hidden)	// BitArray 크기를 위한 Max 지정
};