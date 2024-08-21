// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FNekoWorldEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void OnTabManagerChanged();

public:
	FDelegateHandle DeferredStartDelegateHandle;
	FName StateMachineViewerTabId;
};