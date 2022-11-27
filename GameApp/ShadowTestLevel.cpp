#include "Precompile.h"
#include "ShadowTestLevel.h"

#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineRenderWindow.h>

#include "Enums.h"
#include "UserGame.h"
#include "MousePointer.h"
#include "ShadowTestMap.h"
#include "ShadowTestActor.h"

bool ShadowTestLevel::ThreadLoadingEnd = false;

void ShadowTestLevel::LoadBasicActorResrouce()
{
	//{
	//	GameEngineDirectory NaviMeshDir;
	//	NaviMeshDir.MoveParent("EternalReturn");
	//	NaviMeshDir / "Resources" / "FBX" / "Map";
	//	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(NaviMeshDir.PathToPlusFileName("DefferedTest_Alley.fbx")))
	//	{
	//		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(NaviMeshDir.PathToPlusFileName("DefferedTest_Alley.fbx"));
	//		Mesh->CreateRenderingBuffer();
	//	}
	//}

	ShadowTestActor::TestResourceLoad();
}

void ShadowTestLevel::CreateBasicActor()
{
	// 마우스 생성
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// 테스트 그림자액터
	TestShadowActor_ = CreateActor<ShadowTestActor>();

	// 테스트 라이트액터
	TestLightActor_ = CreateActor<LightActor>();
	TestLightActor_->GetLight()->SetDiffusePower(1.f);
	TestLightActor_->GetLight()->SetAmbientPower(10.f);
	TestLightActor_->GetLight()->SetSpacularLightPow(50.f);
	TestLightActor_->GetLight()->SetLightShadowClipingRange({ 300.f, 300.f });
	//TestLightActor_->GetTransform()->SetWorldPosition(float4(0.0f, 500.0f, 0.0f));
	TestLightActor_->GetTransform()->SetLocalRotationDegree({ 90.0f, 0.0f, 0.0f });
	TestLightActor_->GetTransform()->SetWorldPosition(TestLightActor_->GetTransform()->GetWorldBackVector() * 500.0f);

	// 테스트 맵
	TestShadowMap_ = CreateActor<ShadowTestMap>();
}

void ShadowTestLevel::LevelStart()
{
}

void ShadowTestLevel::LevelUpdate(float _DeltaTime)
{
	// 리소스 로딩 완료 후 액터생성
	if (0 >= UserGame::LoadingFolder &&
		false == ThreadLoadingEnd)
	{
		LoadBasicActorResrouce();
		CreateBasicActor();
		ThreadLoadingEnd = true;
	}

	// Switching FreeCamMode
	if (true == GameEngineInput::GetInst().Down("O"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}
}

void ShadowTestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void ShadowTestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	// 카메라위치 조정
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 500.f, -500.f });
	GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({ 70.f, 0.0f, 0.0f });

	// 렌더타겟 윈도우 on
	GameEngineRenderWindow* RenderWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
	if (nullptr != RenderWindow)
	{
		RenderWindow->On();
	}
}

ShadowTestLevel::ShadowTestLevel()
	: TestLightActor_(nullptr)
	, TestShadowMap_(nullptr)
	, TestShadowActor_(nullptr)
{
}

ShadowTestLevel::~ShadowTestLevel()
{
}
