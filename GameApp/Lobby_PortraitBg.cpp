#include "Precompile.h"
#include "Lobby_PortraitBg.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "LobbyUIController.h"

Lobby_PortraitBg::Lobby_PortraitBg()
	: Char(JobType::MAX)
{
}

Lobby_PortraitBg::~Lobby_PortraitBg()
{
}

void Lobby_PortraitBg::Start()
{

	{
		BasicImageRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
		BasicImageRenderer->SetImage("Lobby_PortraitBG_Basic.png", "PointSmp");
		BasicImageRenderer->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
		BasicImageRenderer->GetTransform()->SetLocalScaling(BasicImageRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		CharPortraitRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
		CharPortraitRenderer->SetImage("Lobby_Portrait_Aya.png", "PointSmp");
		CharPortraitRenderer->GetTransform()->SetLocalPosition((GetTransform()->GetLocalPosition() + float4{ 0.0f, 9.0f}));
		CharPortraitRenderer->GetTransform()->SetLocalScaling(CharPortraitRenderer->GetCurrentTexture()->GetTextureSize());
		CharPortraitRenderer->On();
	}

	{
		SelectImageRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
		SelectImageRenderer->SetImage("Lobby_PortraitBG_Select.png", "PointSmp");
		SelectImageRenderer->GetTransform()->SetLocalPosition((GetTransform()->GetLocalPosition()));
		SelectImageRenderer->GetTransform()->SetLocalScaling(SelectImageRenderer->GetCurrentTexture()->GetTextureSize());
		SelectImageRenderer->Off();
	}


	{
		MouseCollision = CreateTransformComponent<GameEngineCollision>(GetTransform(), static_cast<int>(UIRenderOrder::UICOL));
		MouseCollision->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
		MouseCollision->GetTransform()->SetLocalScaling(BasicImageRenderer->GetCurrentTexture()->GetTextureSize());
		MouseCollision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}

}

void Lobby_PortraitBg::SetChar(JobType _Character)
{
	Char = _Character;

	switch (static_cast<JobType>(Char))
	{
	case JobType::NONE:
		CharPortraitRenderer->SetImage("Lobby_Portrait_Anon.png", "PointSmp");
		break;
	case JobType::YUKI:
		CharPortraitRenderer->SetImage("Lobby_Portrait_Yuki.png", "PointSmp");
		break;
	//ase JobType::FIORA:
	//	CharPortraitRenderer->SetImage("Lobby_Portrait_Anon.png", "PointSmp");
	//	break;
	//ase JobType::ZAHIR:
	//	CharPortraitRenderer->SetImage("Lobby_Portrait_Anon.png", "PointSmp");
	//	break;
	//ase JobType::NADINE:
	//	CharPortraitRenderer->SetImage("Lobby_Portrait_Nadine.png", "PointSmp");
	//	break;
	case JobType::HYUNWOO:
		CharPortraitRenderer->SetImage("Lobby_Portrait_Hyunwoo.png", "PointSmp");
		break;
	case JobType::JACKIE:
		CharPortraitRenderer->SetImage("Lobby_Portrait_Jackie.png", "PointSmp");
		break;
	case JobType::RIO:
		CharPortraitRenderer->SetImage("Lobby_Portrait_Rio.png", "PointSmp");
		break;
	case JobType::AYA:
		CharPortraitRenderer->SetImage("Lobby_Portrait_Aya.png", "PointSmp");
		break;
	case JobType::MAX:
		CharPortraitRenderer->Off();
		break;
	default:
		break;
	}

	return;
}

void Lobby_PortraitBg::SelectOn()
{
	SelectImageRenderer->On();
	LobbyUIController::SelectedChar = Char;
}

void Lobby_PortraitBg::SelectOff()
{
	SelectImageRenderer->Off();
}

void Lobby_PortraitBg::Update(float _DeltaTime)
{
	//if (MouseCollision->Collision(eCollisionGroup::MousePointer))
	//{
	//	if (GameEngineInput::GetInst().Down("LBUTTON"))
	//	{
	//		SelectImageRenderer->On();
	//	}
	//}
	//else
	//{
	//	if (GameEngineInput::GetInst().Down("LBUTTON"))
	//	{
	//		SelectImageRenderer->Off();
	//	}
	//}
}
