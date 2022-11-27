#include "PreCompile.h"
#include "JackieEEffect.h"

#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>

JackieEEffect::JackieEEffect() // default constructer 디폴트 생성자:
	: slashFlag_(false)
{

}

JackieEEffect::~JackieEEffect() // default destructer 디폴트 소멸자
{

}

JackieEEffect::JackieEEffect(JackieEEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void JackieEEffect::PlayAwake()
{
	renderState_ << "Awake";
}

void JackieEEffect::Start()
{

	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_Dust_03b.png");
	hitBase->Cut(6, 6);

	zoneRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	zoneRenderer_->SetImage("FX_BI_Estelle_Skill04_FloorDecal.png", "PointSmp");
	zoneRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 40.0f, 0.0f });
	zoneRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,90.f,0.f });
	zoneRenderer_->GetTransform()->SetLocalScaling(zoneRenderer_->GetCurrentTexture()->GetTextureSize() );
	zoneRenderer_->SetColor({ 1.0f, 0.0f, 0.0f });
	zoneRenderer_->SetAlpha(1.0f);
	zoneRenderer_->Off();

	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("FX_BI_Dust_03b.png", "PointSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize() * 2);
	impactRenderer_->CreateAnimation("FX_BI_Dust_03b.png", "FX_BI_Dust_03b", 0, 35, 0.02f, false);
	impactRenderer_->SetChangeAnimation("FX_BI_Dust_03b", true);
	impactRenderer_->Off();

	renderState_.CreateState(MakeState(JackieEEffect, Sleep));
	renderState_.CreateState(MakeState(JackieEEffect, Awake));
	renderState_ << "Sleep";

}

void JackieEEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void JackieEEffect::startSleep()
{
	zoneRenderer_->Off();
	zoneRenderer_->SetAlpha(1.0f);
	impactRenderer_->Off();
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	timer_ = 0.0f;
	crackFadeTime_ = 0.8f;
}

void JackieEEffect::updateSleep(float _deltaTime)
{
	return;
}

void JackieEEffect::startAwake()
{
	zoneRenderer_->On();
	impactRenderer_->On();
	crackFadeTime_ = 1.0f;
	impactRenderer_->SetChangeAnimation("FX_BI_Dust_03b", true);
}

void JackieEEffect::updateAwake(float _deltaTime)
{
	crackFadeTime_ -= _deltaTime;
	timer_ += _deltaTime;
	zoneRenderer_->SetColor({ crackFadeTime_, 0.0f, 0.0f });
	zoneRenderer_->SetAlpha(crackFadeTime_);

	if (0.0f >= crackFadeTime_)
	{
		timer_ = 0.0f;
		crackFadeTime_ = 1.0f;
		zoneRenderer_->Off();
		zoneRenderer_->SetAlpha(1.0f);
		renderState_ << "Sleep";

		return;
	}
}
