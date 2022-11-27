#include "PreCompile.h"
#include "TitleBG.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>

TitleBG::TitleBG() // default constructer 디폴트 생성자
{

}

TitleBG::~TitleBG() // default destructer 디폴트 소멸자
{

}

TitleBG::TitleBG(TitleBG&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void TitleBG::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	renderer_->SetImage("BG_Blur.png", "PointSmp");
	renderer_->GetTransform()->SetLocalPosition({ 0.0f,0.0f, 0.0f });
	renderer_->GetTransform()->SetLocalScaling(renderer_->GetCurrentTexture()->GetTextureSize());
}

void TitleBG::Update(float _DeltaTime)
{

}

