// Fill out your copyright notice in the Description page of Project Settings.

#include "NekoWorldEditorModule.h"

#include "NekoWorldCommands.h"
#include "LevelEditor.h"

IMPLEMENT_MODULE(FNekoWorldEditorModule, NekoWorldEditor);

#define LOCTEXT_NAMESPACE "NekoWorldMenu"

void FNekoWorldEditorModule::StartupModule()
{
	// Register Command
	FNekoWorldCommands::Register();

	// Mapping Command & Action
	CommandList = MakeShareable(new FUICommandList());
	CommandList->MapAction(FNekoWorldCommands::Get().Command1, FExecuteAction::CreateStatic(&FNekoWorldActions::Action1));
	CommandList->MapAction(FNekoWorldCommands::Get().Command2, FExecuteAction::CreateStatic(&FNekoWorldActions::Action2));
	CommandList->MapAction(FNekoWorldCommands::Get().Command3, FExecuteAction::CreateStatic(&FNekoWorldActions::Action3));
	CommandList->MapAction(FNekoWorldCommands::Get().Command4, FExecuteAction::CreateStatic(&FNekoWorldActions::Action3));

	// 툴바 생성을 위한 델리게이트 함수 선언
	struct NekoWorldToolbar
	{
		static void CreateNekoWorldToolbar(FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.BeginSection("NekoToolbar");
			{
				ToolbarBuilder.AddToolBarButton(FNekoWorldCommands::Get().Command1);
				ToolbarBuilder.AddToolBarButton(FNekoWorldCommands::Get().Command2);
				ToolbarBuilder.AddToolBarButton(FNekoWorldCommands::Get().Command3);
				ToolbarBuilder.AddToolBarButton(FNekoWorldCommands::Get().Command4);
			}
			ToolbarBuilder.EndSection();
		}
	};

	// 레벨 에디터 얻어오기
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedPtr<FExtender> NekoToolbarExtender = MakeShareable(new FExtender());
	NekoToolbarExtender->AddToolBarExtension("Content", EExtensionHook::After, CommandList,
		FToolBarExtensionDelegate::CreateStatic(&NekoWorldToolbar::CreateNekoWorldToolbar));
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(NekoToolbarExtender);


	// 메뉴 생성을 위한 델리게이트 함수
	struct FNekoWorldMenu
	{
		static void CreateNekoWorldMenu(FMenuBuilder& MenuBuilder)
		{
			// Section1
			MenuBuilder.BeginSection("Section1", LOCTEXT("NekoWorldMenu", "Neko World Menu Section1"));
			
				MenuBuilder.AddMenuEntry(FNekoWorldCommands::Get().Command1);
				MenuBuilder.AddMenuEntry(FNekoWorldCommands::Get().Command2);
			
			MenuBuilder.EndSection();

			// Section2 (SubMenu)
			MenuBuilder.BeginSection("Section2", LOCTEXT("NekoWorldMenu", "Neko World Menu Section2"));
			{
				struct NekoWorldSubMenu
				{
					static void CreateNekoWorldSubMenu(FMenuBuilder& SubMenuBuilder)
					{
						SubMenuBuilder.AddMenuEntry(FNekoWorldCommands::Get().Command3);
						SubMenuBuilder.AddMenuEntry(FNekoWorldCommands::Get().Command4);
					}
				};

				MenuBuilder.AddSubMenu(
					LOCTEXT("NekoWorldMenu", "NekoWorld SubMenu"),
					LOCTEXT("NekoWorldMenu", "NekoWorld SubMenu Tooltip"),
					FNewMenuDelegate::CreateStatic(&NekoWorldSubMenu::CreateNekoWorldSubMenu), false, FSlateIcon()
				);
			}
			MenuBuilder.EndSection();
		}
	};

	TSharedPtr<FExtender> NekoWorldMenuExtender = MakeShareable(new FExtender());
	NekoWorldMenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, CommandList,
		FMenuExtensionDelegate::CreateStatic(&FNekoWorldMenu::CreateNekoWorldMenu));
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(NekoWorldMenuExtender);
}

void FNekoWorldEditorModule::ShutdownModule()
{
	FNekoWorldCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE