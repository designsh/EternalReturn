#include "PreCompile.h"
#include "TitleStartButton.h"
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>

TitleStartButton::TitleStartButton() // default constructer 디폴트 생성자
	: renderer_(nullptr), collision_(nullptr)
{

}

TitleStartButton::~TitleStartButton() // default destructer 디폴트 소멸자
{

}


bool TitleStartButton::MouseCollisionCheck()
{
    return collision_->Collision(static_cast<int>(eCollisionGroup::MousePointer));
}

void TitleStartButton::SetImageByIndex(int _index)
{
	switch (_index)
	{
	case 0:
		renderer_->SetImage("Title_Start0.png", "PointSmp");
		break;
	case 1:
		renderer_->SetImage("Title_Start1.png", "PointSmp");
		break;
	case 2:
		renderer_->SetImage("Title_Start2.png", "PointSmp");
		break;
	default:
		break;
	}

}

void TitleStartButton::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	renderer_->SetImage("Title_Start0.png", "PointSmp");
	renderer_->GetTransform()->SetLocalPosition({ 0.0f, -150.0f, 0.0f });
	renderer_->GetTransform()->SetLocalScaling(renderer_->GetCurrentTexture()->GetTextureSize());

	collision_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_->GetTransform()->SetLocalPosition(renderer_->GetTransform()->GetLocalPosition());
	collision_->GetTransform()->SetLocalScaling(renderer_->GetCurrentTexture()->GetTextureSize());
	collision_->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

}

void TitleStartButton::Update(float _DeltaTime)
{
}

