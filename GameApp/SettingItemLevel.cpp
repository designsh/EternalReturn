#include "PreCompile.h"
#include "SettingItemLevel.h"
#include "ItemBoxWindow.h"
#include "ItemBoxManager.h"

#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngine/LightActor.h>
#include "UserGame.h"
#include <GameEngine/SKySphereActor.h>
#include "MousePointer.h"
#include "NavMesh.h"
#include "ItemBoxManager.h"
#include <GameEngine/GameEngineCollision.h>

SettingItemLevel::SettingItemLevel()
	: ItemBox_(nullptr)
	, Window_(nullptr)
{

}

SettingItemLevel::~SettingItemLevel()
{

}

void SettingItemLevel::LevelStart()
{
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);

	GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	GameEngineInput::GetInst().CreateKey("LBUTTON", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("Esc", VK_ESCAPE);

	if (false == GameEngineInput::GetInst().IsKey("FreeCam"))
	{
		GameEngineInput::GetInst().CreateKey("FreeCam", 'o');
	}

	GetMainCameraActor()->FreeCameraModeSwitch();
}

void SettingItemLevel::LevelUpdate(float _DeltaTime)
{
	static bool isLoaded = false;

	if (0 >= UserGame::LoadingFolder &&
		false == isLoaded)
	{
		CreateActorLevel();
		isLoaded = true;
	}
}

void SettingItemLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void SettingItemLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	static bool Check = false;

	if (Check == true)
	{
		return;
	}

	if (nullptr == GameEngineGUI::GetInst()->FindGUIWindow("ItemBoxWindow"))
	{
		Window_ = GameEngineGUI::GetInst()->CreateGUIWindow<ItemBoxWindow>("ItemBoxWindow");
	}

	GameEngineDirectory tempDir;

	tempDir.MoveParent("EternalReturn");
	tempDir.MoveChild("Resources");
	tempDir.MoveChild("FBX");
	tempDir.MoveChild("YSJ");

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	tempDir.MoveParent("FBX");
	tempDir.MoveChild("UserMesh");
	tempDir.MoveChild("Map");

	std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");

	for (size_t i = 0; i < vecFile.size(); i++)
	{
		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecFile[i].GetFullPath());
		}
	}

	tempDir.MoveParent("UserMesh");
	tempDir.MoveChild("ItemBox");

	vecFile = tempDir.GetAllFile(".UserMesh");

	for (size_t i = 0; i < vecFile.size(); i++)
	{
		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecFile[i].GetFullPath());
		}
	}

	tempDir.MoveParent("FBX");
	tempDir.MoveChild("UserMesh");
	tempDir.MoveChild("ItemBox");
	tempDir.MoveChild("ItemBoxInfo");

	ItemBox_ = CreateActor<ItemBoxManager>();
	ItemBox_->UserAllLoad(tempDir);

	if (nullptr != Window_)
	{
		Window_->ItemBoxManager_ = ItemBox_;
	}

	Check = true;
}

void SettingItemLevel::CreateActorLevel()
{
	LightActor* Light = CreateActor<LightActor>();
	Light->GetLight()->SetAmbientPower(10.f);
	Light->GetLight()->SetSpacularLightPow(10.f);
	SKySphereActor* SkyActor = CreateActor<SKySphereActor>();

	mouse_ = CreateActor<MousePointer>();

	//NaviMesh* Navi = CreateActor<NaviMesh>();

	//Navi->CreateNaviMesh(LumiaMap->GetFBXNaviRenderer());
}
