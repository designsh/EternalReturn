#include "PreCompile.h"
#include "TitleBG.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>

TitleBG::TitleBG() // default constructer ����Ʈ ������
{

}

TitleBG::~TitleBG() // default destructer ����Ʈ �Ҹ���
{

}

TitleBG::TitleBG(TitleBG&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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

