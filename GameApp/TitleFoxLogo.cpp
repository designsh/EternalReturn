#include "PreCompile.h"
#include "TitleFoxLogo.h"
#include <GameEngine/GameEngineImageRenderer.h>

TitleFoxLogo::TitleFoxLogo() // default constructer 디폴트 생성자
	: renderer_(nullptr)
{

}

TitleFoxLogo::~TitleFoxLogo() // default destructer 디폴트 소멸자
{

}

TitleFoxLogo::TitleFoxLogo(TitleFoxLogo&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void TitleFoxLogo::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	renderer_->SetImage("Foxteam.png", "PointSmp");
	renderer_->GetTransform()->SetLocalPosition({ 430.0f, -300.0f, -10.0f });
	renderer_->GetTransform()->SetLocalScaling(renderer_->GetCurrentTexture()->GetTextureSize());
}

void TitleFoxLogo::Update(float _DeltaTime)
{
}

