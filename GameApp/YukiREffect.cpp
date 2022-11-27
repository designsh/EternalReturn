#include "PreCompile.h"
#include "YukiREffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>

YukiREffect::YukiREffect() // default constructer 디폴트 생성자
{

}

YukiREffect::~YukiREffect() // default destructer 디폴트 소멸자
{

}

YukiREffect::YukiREffect(YukiREffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void YukiREffect::PlayAwake()
{
	renderState_ << "Awake";
}


void YukiREffect::PlaySlash()
{
	timer_ = 0.0f;
	groundOutTime_ = 0.0f;
	renderState_ << "Slash";
}

void YukiREffect::Start()
{
	zoneRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	zoneRenderer_->SetImage("FX_BI_Yuki_02.png", "PointSmp");
	zoneRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 300.0f });
	zoneRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	zoneRenderer_->GetTransform()->SetLocalScaling(zoneRenderer_->GetCurrentTexture()->GetTextureSize() * 2.5f);
	zoneRenderer_->SetColor({0.8f, 0.2f, 0.2f});
	zoneRenderer_->SetAlpha(0.0f);
	zoneRenderer_->Off();

	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("FX_BI_Sword_02AB.png", "PointSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 150.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,180.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize() * 5.5f);
	impactRenderer_->SetAlpha(0.8f); 
	impactRenderer_->Off();

	renderState_.CreateState(MakeState(YukiREffect, Sleep));
	renderState_.CreateState(MakeState(YukiREffect, Awake));
	renderState_.CreateState(MakeState(YukiREffect, Intermission));
	renderState_.CreateState(MakeState(YukiREffect, Slash));
	renderState_ << "Sleep";
}

void YukiREffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void YukiREffect::startAwake()
{
	zoneRenderer_->On();
	impactRenderer_->Off();
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 0.0f });
	slashFlag_ = false;
	timer_ = 0.0f;
	groundFadeTime_ = 0.6f;
	slashFadeTime_ = 0.8f;
}

void YukiREffect::updateAwake(float _deltaTime)
{
	if (0.8f <= timer_)
	{
		timer_ = 0.0f;
		groundFadeTime_ = 0.6f;
		renderState_ << "Intermission";
		return;
	}

	if (0.9f <= groundFadeTime_)
	{
		groundFadeTime_ = 0.9f;
	}

	groundFadeTime_ += _deltaTime;
	zoneRenderer_->SetAlpha(groundFadeTime_);

	

	timer_ += _deltaTime;
}


void YukiREffect::startIntermission()
{
	groundOutTime_ = 0.3f;
}

void YukiREffect::updateIntermission(float _deltaTime)
{

	if (0.0f >= groundOutTime_)
	{
		groundOutTime_ = 0.0f;
	}

	zoneRenderer_->SetAlpha(groundOutTime_ /0.3f);

	groundOutTime_ -= _deltaTime;
}

void YukiREffect::startSlash()
{
	zoneRenderer_->Off();
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 0.0f });
	impactRenderer_->On();
	slashFadeTime_ = 0.8f;
}

void YukiREffect::updateSlash(float _deltaTime)
{
	if (0.0f >= slashFadeTime_)
	{
		slashFadeTime_ = 0.0f;
		impactRenderer_->Off();
		impactRenderer_->SetAlpha(0.8f);
		renderState_ << "Sleep";
	}

	impactRenderer_->GetTransform()->SetLocalMove({ 0.0f,0.0f,50.0f * _deltaTime });
	impactRenderer_->SetAlpha(slashFadeTime_);

	slashFadeTime_ -= _deltaTime;

}

void YukiREffect::startSleep()
{
	zoneRenderer_->Off();
	impactRenderer_->Off();
	slashFadeTime_ = 0.8f;
	groundOutTime_ = 0.3f;
	timer_ = 0.0f;
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 0.0f });
}

void YukiREffect::updateSleep(float _deltaTime)
{
}
