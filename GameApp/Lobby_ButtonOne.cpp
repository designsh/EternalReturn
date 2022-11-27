#include "Precompile.h"
#include "Lobby_ButtonOne.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

Lobby_ButtonOne::Lobby_ButtonOne()
{
}

Lobby_ButtonOne::~Lobby_ButtonOne()
{
}

bool Lobby_ButtonOne::MouseCollisionCheck()
{
	return ButtonOneCollision->Collision(static_cast<int>(eCollisionGroup::MousePointer));
}

void Lobby_ButtonOne::SetText(const std::string& _text)
{
	ButtonOneRenderer->TextSetting("±¼¸²", _text, 16.0f, FW1_CENTER, float4::BLACK, { 0.0f, 12.0f, 0.0f });
}

void Lobby_ButtonOne::SetImageByIndex(int _index)
{
	switch (_index)
	{
	case 0:
		// idle
		ButtonOneRenderer->SetImage("Btn_Slide_Basic.png", "PointSmp");
		break;
	case 1:
		// rollover
		ButtonOneRenderer->SetImage("Btn_Slide_Rollover.png", "PointSmp");
		break;
	case 2:
		// click
		ButtonOneRenderer->SetImage("Btn_Slide_Choice.png", "PointSmp");
		break;
	case 3:
		// confirmed
		ButtonOneRenderer->SetImage("Btn_Start_Quick_Rollover.png", "PointSmp");
		ButtonOneRenderer->GetTransform()->SetLocalPosition({ 554.0f, 330.0f });
		break;
	default:
		break;
	}
}

void Lobby_ButtonOne::Start()
{
	{
		ButtonOneRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UIPANEL0));
		ButtonOneRenderer->SetImage("Btn_Slide_Basic.png", "PointSmp");
		ButtonOneRenderer->GetTransform()->SetLocalPosition({ 410.0f, 330.0f });
		ButtonOneRenderer->GetTransform()->SetLocalScaling(ButtonOneRenderer->GetCurrentTexture()->GetTextureSize());
		ButtonOneRenderer->TextSetting("±¼¸²", "È£½ºÆ® »ý¼º", 16.0f, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f, 0.0f });

		ButtonOneCollision = CreateTransformComponent<GameEngineCollision>(GetTransform(), static_cast<int>(UIRenderOrder::UICOL));
		ButtonOneCollision->GetTransform()->SetLocalPosition({ 410.0f, 330.0f});
		ButtonOneCollision->GetTransform()->SetLocalScaling(ButtonOneRenderer->GetTransform()->GetLocalScaling());
		ButtonOneCollision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}

}

void Lobby_ButtonOne::Update(float _DeltaTime)
{


}
