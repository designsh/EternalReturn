#include "PreCompile.h"
#include "MeshLoadLevel.h"
#include "UI_Skill.h"
#include <GameEngine/PostFade.h>
#include <GameEngine/PostBlur.h>
#include <GameEngine/SmallPostBlur.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineTransform.h>
#include <GameEngine/CameraActor.h>
#include <GameEngine/MouseActor.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/GameEngineFBXWindow.h>
#include "UserGame.h"
#include "TestPlayer.h"

MeshLoadLevel::MeshLoadLevel()
{
}

MeshLoadLevel::~MeshLoadLevel()
{
}

void MeshLoadLevel::LevelStart()
{
	FBXWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineFBXWindow>("FBXWindow");

	FBXWindow->FBXFolder.MoveParent("EternalReturn");
	FBXWindow->FBXFolder.MoveChild("EngineResources");
	FBXWindow->FBXFolder.MoveChild("FBX");

	FBXWindow->UserMeshSaveFolder.MoveParent("EternalReturn");
	FBXWindow->UserMeshSaveFolder.MoveChild("Resources");
	FBXWindow->UserMeshSaveFolder.MoveChild("FBX");
	FBXWindow->UserMeshSaveFolder.MoveChild("UserMesh");


	FBXWindow->FBXFolder.MoveParent("EternalReturn");
	FBXWindow->FBXFolder.MoveChild("EngineResources");
	FBXWindow->FBXFolder.MoveChild("FBX");

	FBXWindow->UserAnimationSaveFolder.MoveParent("EternalReturn");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Resources");
	FBXWindow->UserAnimationSaveFolder.MoveChild("FBX");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Character");
	//FBXWindow->UserAnimationSaveFolder.MoveChild("Yuki");
	//GameEngineGUIWindow::inst

	//GameEngineDirectory Dir;
	//Dir.MoveParent("AR38");
	//Dir.MoveChild("EngineResources");
	//Dir.MoveChild("FBX");

	//std::vector<GameEngineFile> Files = Dir.GetAllFile("FBX");

	//for (auto& File : Files)
	//{
	//	GameEngineFBXMeshManager::GetInst().Load(File.GetFullPath());
	//}


	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));


	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("MOn", 'p');
		GameEngineInput::GetInst().CreateKey("MOff", 'o');
		GameEngineInput::GetInst().CreateKey("LevelControl", 'i');
	}

	GameEngineDirectory tempDir;

	tempDir.MoveParent("EternalReturn");
	tempDir.MoveChild("Resources");
	tempDir.MoveChild("Texture");
	tempDir.MoveChild("Map");

	std::vector<GameEngineFile> vecFile = tempDir.GetAllFile("FBX");

	for (size_t i = 0; i < vecFile.size(); i++)
	{
		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(vecFile[i].GetFullPath());
		}
	}

	for (size_t i = 0; i < vecFile.size(); i++)
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFileName());
		std::string SaveName = GameEnginePath::GetFileNameWithOutExtension(vecFile[i].GetFileName());
		SaveName += ".UserMesh";
		if (nullptr != Mesh)
		{
			Mesh->UserSave(FBXWindow->UserMeshSaveFolder.PathToPlusFileName(SaveName));
		}
	}

	GameEngineDebug::MsgBoxError("UserMesh Save 완료");

	int a = 0;
}

void MeshLoadLevel::LevelUpdate(float _DeltaTime)
{
	static bool CreateActorCheck = false;

	if (0 >= UserGame::LoadingFolder
		&& false == CreateActorCheck)
	{
		CreateActorLevel();
		CreateActorCheck = true;
	}

	static bool Check = false;

	if (false == Check && nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredLightTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredTarget(), Size * 3);
		Check = true;
	}

	if (true == GameEngineInput::GetInst().Down("MOn"))
	{
		GameEngineGUIWindow* Window = GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow");

		Window->OnOffChange();
	}

	if (true == GameEngineInput::GetInst().Down("MOff"))
	{
		//Window->Off();
		//MActor->WindowCursorOff();TestPlayer

	}
}

// 지금 내가 보스방이야.
// 지금 내가 보스방이야.
void MeshLoadLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	

	//if (std::string::npos != _NextLevel->GetName().find("World")
	//	&& std::string::npos != _NextLevel->GetName().find("Boss"))
	//{

	//	Player::MainPlayer->GetLevel()->GetLevelActorMove(_NextLevel, Player::MainPlayer);

	//}

	// MoveLevelActor("TitleLevel", "BossLevel");

}
void MeshLoadLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	FBXWindow->TestInit();
}

void MeshLoadLevel::CreateActorLevel()
{



	static bool Check = false;

	if (true == Check)
	{
		return;
	}

	{
		MActor = CreateActor<MouseActor>();
		MActor->GetUIRenderer()->SetRenderGroup(1000);
	}

	{
		LightActor* Actor;

		
		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.3f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.3f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);
		Actor->GetTransform()->SetLocalRotationDegree({45.0f, 0.0f, 0.0f});

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.3f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);
		Actor->GetTransform()->SetLocalRotationDegree({ 0.0f, 90.0f, 0.0f });
	}


	{
		TestPlayer* Actor = CreateActor<TestPlayer>();
	}

	{
		SKySphereActor* Actor = CreateActor<SKySphereActor>();
		// Actor->SetRadius(5000);
	}
	

	{
		UI_Skill* Actor = CreateActor<UI_Skill>();
		Actor->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));
	}

	Check = true;
}