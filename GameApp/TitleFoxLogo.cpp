#include "PreCompile.h"
#include "TitleFoxLogo.h"
#include <GameEngine/GameEngineImageRenderer.h>

TitleFoxLogo::TitleFoxLogo() // default constructer ����Ʈ ������
	: renderer_(nullptr)
{

}

TitleFoxLogo::~TitleFoxLogo() // default destructer ����Ʈ �Ҹ���
{

}

TitleFoxLogo::TitleFoxLogo(TitleFoxLogo&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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

