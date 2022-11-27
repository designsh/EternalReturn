#include "Precompile.h"
#include "Lobby_ButtonTwo.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

Lobby_ButtonTwo::Lobby_ButtonTwo()
{
}

Lobby_ButtonTwo::~Lobby_ButtonTwo()
{
}

bool Lobby_ButtonTwo::MouseCollisionCheck()
{
	return ButtonTwoCollision->Collision(static_cast<int>(eCollisionGroup::MousePointer));
}

void Lobby_ButtonTwo::SetText(const std::string& _text)
{
	ButtonTwoRenderer->TextSetting("굴림", _text, 16.0f, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f, 0.0f });
}

void Lobby_ButtonTwo::SetImageByIndex(int _index)
{
	switch (_index)
	{
	case 0:
		// idle
		ButtonTwoRenderer->SetImage("Btn_Slide_Basic.png", "PointSmp");
		break;
	case 1:
		// rollover
		ButtonTwoRenderer->SetImage("Btn_Slide_Rollover.png", "PointSmp");
		break;
	case 2:
		// click
		ButtonTwoRenderer->SetImage("Btn_Slide_Choice.png", "PointSmp");
		break;
	case 3:
		// confirmed
		ButtonTwoRenderer->SetImage("Btn_Start_Rollover.png", "PointSmp");
		break;
	default:
		break;
	}
}

void Lobby_ButtonTwo::Start()
{
	{
		ButtonTwoRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UIPANEL0));
		ButtonTwoRenderer->SetImage("Btn_Slide_Basic.png", "PointSmp");
		//이미지 크기는 126, 두 이미지의 거리는 8
		ButtonTwoRenderer->GetTransform()->SetLocalPosition({ 554.0f, 330.0f });
		ButtonTwoRenderer->GetTransform()->SetLocalScaling(ButtonTwoRenderer->GetCurrentTexture()->GetTextureSize());
		ButtonTwoRenderer->TextSetting("굴림", "클라이언트 참여", 16.0f, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f, 0.0f });
	}
	
	{
		ButtonTwoCollision = CreateTransformComponent<GameEngineCollision>(GetTransform(), static_cast<int>(UIRenderOrder::UICOL));
		ButtonTwoCollision->GetTransform()->SetLocalPosition({ 554.0f,330.0f});
		ButtonTwoCollision->GetTransform()->SetLocalScaling(ButtonTwoRenderer->GetTransform()->GetLocalScaling());
		ButtonTwoCollision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}

}

void Lobby_ButtonTwo::Update(float _DeltaTime)
{
	GetLevel()->PushDebugRender(ButtonTwoRenderer->GetTransform(), CollisionType::Rect);
}
