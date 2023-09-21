// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NekoWorldSlateStyle.h"

/**
 *  Slate �޴�, ���ٿ��� �������� ���(Command)
 */
class FNekoWorldCommands : public TCommands<FNekoWorldCommands>
{
public:
	// ���⼭ �� �ٿ� ������ �ؽ�Ʈ�� ��� �� �ִ°�?
	FNekoWorldCommands()
		: TCommands<FNekoWorldCommands>(
			TEXT("NekoWorld"), 
			NSLOCTEXT("NekoWorld", "NekoWorld", "NekoWorld"), 
			NAME_None, 
			FNekoWorldSlateStyle::GetStyleSetName())
	{
		
	}

	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> Command1;
	TSharedPtr<FUICommandInfo> Command2;
	TSharedPtr<FUICommandInfo> Command3;
	TSharedPtr<FUICommandInfo> Command4;
};


/**
 *  Command ���� �� ������ ����
 */
class FNekoWorldActions
{
public:
	static void Action1();
	static void Action2();
	static void Action3();
};
