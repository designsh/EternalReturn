#include "PreCompile.h"
#include "JackieQEffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>

JackieQEffect::JackieQEffect() // default constructer 디폴트 생성자:
	: slashFlag_(false)
{

}

JackieQEffect::~JackieQEffect() // default destructer 디폴트 소멸자
{

}

JackieQEffect::JackieQEffect(JackieQEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void JackieQEffect::PlayAwake()
{
	renderState_ << "Awake";
}

void JackieQEffect::Start()
{

	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_SELine_10.png");
	hitBase->Cut(3,3);

	zoneRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	zoneRenderer_->SetImage("FX_BI_Jan_Skill02_Range_BG.png", "PointSmp");
	zoneRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 30.0f, 230.0f });
	zoneRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,90.f,0.f });
	zoneRenderer_->GetTransform()->SetLocalScaling(zoneRenderer_->GetCurrentTexture()->GetTextureSize()/2);
	zoneRenderer_->SetAlpha(0.7f);
	zoneRenderer_->Off();

	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("jackie_groundbomb.png", "PointSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 230.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize());
	impactRenderer_->CreateAnimation("FX_BI_SELine_10.png", "FX_BI_SELine_10", 0, 8, 0.02f, false);
	impactRenderer_->SetChangeAnimation("FX_BI_SELine_10", true);
	impactRenderer_->SetAlpha(0.7f);
	impactRenderer_->Off();

	renderState_.CreateState(MakeState(JackieQEffect, Sleep));
	renderState_.CreateState(MakeState(JackieQEffect, Awake));
	renderState_ << "Sleep";

}

void JackieQEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void JackieQEffect::startSleep()
{
	zoneRenderer_->Off();
	impactRenderer_->Off();
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	slashFlag_ = false;
	timer_ = 0.0f;
	crackFadeTime_ = 0.8f;
}

void JackieQEffect::updateSleep(float _deltaTime)
{
	return;
}

void JackieQEffect::startAwake()
{
	zoneRenderer_->On();
	impactRenderer_->On();
	crackFadeTime_ = 0.8f;
	impactRenderer_->SetChangeAnimation("FX_BI_SELine_10", true);
}

void JackieQEffect::updateAwake(float _deltaTime)
{
	crackFadeTime_ -= _deltaTime;
	timer_ += _deltaTime;

	if (0.3f <= timer_ && slashFlag_ == false)
	{
		impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,90.f,0.f });
		impactRenderer_->SetChangeAnimation("FX_BI_SELine_10", true);
		slashFlag_ = true;
	}

	if (0.0f >= crackFadeTime_)
	{
		impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
		slashFlag_ = false;
		timer_ = 0.0f;
		crackFadeTime_ = 0.8f;
		renderState_ << "Sleep";

		return;
	}
}
