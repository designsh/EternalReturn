#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/GameEngineRenderWindow.h>

#include "MeshLoadLevel.h"
#include "SettingItemLevel.h"
#include "ServerTestLevel.h"
#include "LobbyLevel.h"
#include "KeyboardClass.h"	
#include "LumiaLevel.h"
#include "TitleLevel.h"
#include "LoadingLevel.h"
#include "TestLevel.h"
#include "EndingLevel.h"
#include "OutLineTestLevel.h"
#include "ShadowTestLevel.h"

std::atomic<int> UserGame::LoadingFolder = 0;
bool UserGame::IsServer_ = false;

UserGame::UserGame()
{

}

UserGame::~UserGame()
{
	KeyboardClass::GetInst().Destroy();
}

UserGame::UserGame(UserGame&& _other) noexcept
{

}


void UserGame::Initialize()
{
	GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineLevelControlWindow>("LevelControlWindow");
	GameEngineRenderWindow* RenderWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineRenderWindow>("RenderWindow");
	RenderWindow->Off();
	

	//LevelCreate<MeshLoadLevel>("MeshLoadLevel");
	//LevelCreate<OutLineTestLevel>("OutLineTestLevel");
	//LevelCreate<ShadowTestLevel>("ShadowTestLevel");
	LevelCreate<TestLevel>("TestLevel");
	LevelCreate<TitleLevel>("TitleLevel");
	LevelCreate<LobbyLevel>("LobbyLevel");
	LevelCreate<LoadingLevel>("LoadingLevel");
	LevelCreate<LumiaLevel>("LumiaLevel");
	LevelCreate<EndingLevel>("EndingLevel");
	LevelChange("TitleLevel");
	return;
}

void UserGame::Release()
{
}