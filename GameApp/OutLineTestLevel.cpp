#include "PreCompile.h"
#include "OutLineTestLevel.h"

#include <GameEngine/LightActor.h>

#include "Enums.h"
#include "UserGame.h"
#include "OutLineTestWindow.h"
#include "MousePointer.h"
#include "OutLineTestMap.h"
#include "OutLineTestActor.h"

bool OutLineTestLevel::ThreadLoadingEnd = false;

void OutLineTestLevel::LoadBasicActorResrouce()
{
	OutLineTestActor::TestResourceLoad();
}

void OutLineTestLevel::CreateBasicActor()
{
	// ���콺 ����
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// �׽�Ʈ ����Ʈ����
	TestLightActor_ = CreateActor<LightActor>();
	TestLightActor_->GetLight()->SetDiffusePower(1.0f);
	TestLightActor_->GetLight()->SetAmbientPower(10.0f);
	TestLightActor_->GetLight()->SetSpacularLightPow(10.0f);

	// �׽�Ʈ��
	TestMap_ = CreateActor<OutLineTestMap>();

	// �׽�Ʈ����
	TestActor_ = CreateActor<OutLineTestActor>();
}

void OutLineTestLevel::LevelStart()
{
}

void OutLineTestLevel::LevelUpdate(float _DeltaTime)
{
	// ���ҽ� �ε� �Ϸ� �� ���ͻ���
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

void OutLineTestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	// IMGUI Window
	OutLineWindow_ = GameEngineGUI::GetInst()->FindGUIWindowConvert<OutLineTestWindow>("OutLineTestWindow");
	if (nullptr == OutLineWindow_)
	{
		OutLineWindow_ = GameEngineGUI::GetInst()->CreateGUIWindow<OutLineTestWindow>("OutLineTestWindow");
		OutLineWindow_->Off();
	}
	OutLineWindow_->Off();
}

void OutLineTestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	// IMGUI Window
	OutLineWindow_ = GameEngineGUI::GetInst()->FindGUIWindowConvert<OutLineTestWindow>("OutLineTestWindow");
	if (nullptr == OutLineWindow_)
	{
		OutLineWindow_ = GameEngineGUI::GetInst()->CreateGUIWindow<OutLineTestWindow>("OutLineTestWindow");
		OutLineWindow_->On();
	}
	OutLineWindow_->On();

	// ī�޶���ġ ����
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 500.f, -500.f });
	GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({70.f, 0.0f, 0.0f});
}

OutLineTestLevel::OutLineTestLevel()
	: OutLineWindow_(nullptr)
	, TestLightActor_(nullptr)
	, TestMap_(nullptr)
	, TestActor_(nullptr)
{
}

OutLineTestLevel::~OutLineTestLevel()
{
}
