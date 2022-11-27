#include "PreCompile.h"
#include "TitlePrologueBG.h"
#include <GameEngine/GameEngineImageRenderer.h>

TitlePrologueBG::TitlePrologueBG() // default constructer 디폴트 생성자
	:renderer_(nullptr)
{

}

TitlePrologueBG::~TitlePrologueBG() // default destructer 디폴트 소멸자
{

}

TitlePrologueBG::TitlePrologueBG(TitlePrologueBG&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void TitlePrologueBG::SetAlpha(float _Alpha)
{
	if (0.0f > _Alpha)
	{
		return;
	}

	renderer_->SetAlpha(_Alpha);

}

void TitlePrologueBG::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	renderer_->SetImage("FoxteamBig.png", "PointSmp");
	renderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, -20.0f });
	renderer_->GetTransform()->SetLocalScaling(renderer_->GetCurrentTexture()->GetTextureSize());
}

void TitlePrologueBG::Update(float _DeltaTime)
{
	

}

