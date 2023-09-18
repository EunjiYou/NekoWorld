// Fill out your copyright notice in the Description page of Project Settings.


#include "NekoWorldCommands.h"

#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "NekoCommand"

void FNekoWorldCommands::RegisterCommands()
{
	UI_COMMAND(Command1, "NekoCommand1", "Neko Command1 Tooltip", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Command2, "NekoCommand2", "Neko Command2 Tooltip", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Command3, "NekoCommand3", "Neko Command3 Tooltip", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Command4, "NekoCommand4", "Neko Command4 Tooltip", EUserInterfaceActionType::Button, FInputChord());
}

void FNekoWorldActions::Action1()
{
	FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NekoCommand", "Neko Editor Extension Action 1 Message"));
}

void FNekoWorldActions::Action2()
{
	FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NekoCommand", "Neko Editor Extension Action 2 Message"));
}

void FNekoWorldActions::Action3()
{
	FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NekoCommand", "Neko Editor Extension Action 3 Message"));
}

#undef LOCTEXT_NAMESPACE