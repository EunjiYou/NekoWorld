// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NekoWorldSlateStyle.h"

/**
 *  Slate 메뉴, 툴바에게 지정해줄 명령(Command)
 */
class FNekoWorldCommands : public TCommands<FNekoWorldCommands>
{
public:
	// 여기서 툴 바에 나오는 텍스트를 띄울 수 있는가?
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
 *  Command 실행 시 연결할 로직
 */
class FNekoWorldActions
{
public:
	static void Action1();
	static void Action2();
	static void Action3();
};
