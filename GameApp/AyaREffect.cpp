#include "PreCompile.h"
#include "AyaREffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>

AyaREffect::AyaREffect() // default constructer 디폴트 생성자
	: rangeRenderer_(nullptr), gatherRenderer_(nullptr), shotRenderer1_(nullptr), shotRenderer2_(nullptr), shotRenderer3_(nullptr)
{

}

AyaREffect::~AyaREffect() // default destructer 디폴트 소멸자
{

}

AyaREffect::AyaREffect(AyaREffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void AyaREffect::PlayAwake()
{
	renderState_ << "Awake";
}

void AyaREffect::Start()
{
	rangeRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	rangeRenderer_->SetImage("rRange.png", "PointSmp");
	rangeRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 30.0f, 0.0f });
	rangeRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	rangeRenderer_->GetTransform()->SetLocalScaling(rangeRenderer_->GetCurrentTexture()->GetTextureSize() * 4.5f);
	rangeRenderer_->SetAlpha(0.0f);
	rangeRenderer_->Off();

	rangeScale_ = rangeRenderer_->GetTransform()->GetLocalScaling();

	gatherRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	gatherRenderer_->SetImage("rGather.png", "PointSmp");
	gatherRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 35.0f, 0.0f });
	gatherRenderer_->GetTransform()->SetLocalRotationDegree({90.f,0.f,0.f });
	gatherRenderer_->GetTransform()->SetLocalScaling(gatherRenderer_->GetCurrentTexture()->GetTextureSize() * 2.5f);
	gatherRenderer_->SetAlpha(0.0f);
	gatherRenderer_->Off();

	gatherScale_ = gatherRenderer_->GetTransform()->GetLocalScaling();

	shotRenderer1_ = CreateTransformComponent<GameEngineEffectRenderer>();
	shotRenderer1_->SetImage("rShot.png", "PointSmp");
	shotRenderer1_->GetTransform()->SetLocalPosition({ 0.0f, 400.0f, 0.0f });
	shotRenderer1_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	shotRenderer1_->GetTransform()->SetLocalScaling(shotRenderer1_->GetCurrentTexture()->GetTextureSize());
	shotRenderer1_->SetAlpha(0.0f);
	shotRenderer1_->Off();

	shotRenderer2_ = CreateTransformComponent<GameEngineEffectRenderer>();
	shotRenderer2_->SetImage("rShot.png", "PointSmp");
	shotRenderer2_->GetTransform()->SetLocalPosition({ 0.0f, 450.0f, 0.0f });
	shotRenderer2_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	shotRenderer2_->GetTransform()->SetLocalScaling(shotRenderer2_->GetCurrentTexture()->GetTextureSize() * 1.2f);
	shotRenderer2_->SetAlpha(0.0f);
	shotRenderer2_->Off();

	shotRenderer3_ = CreateTransformComponent<GameEngineEffectRenderer>();
	shotRenderer3_->SetImage("rShot.png", "PointSmp");
	shotRenderer3_->GetTransform()->SetLocalPosition({ 0.0f, 500.0f, 0.0f });
	shotRenderer3_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	shotRenderer3_->GetTransform()->SetLocalScaling(shotRenderer3_->GetCurrentTexture()->GetTextureSize() * 0.8f);
	shotRenderer3_->SetAlpha(0.0f);
	shotRenderer3_->Off();

	renderState_.CreateState(MakeState(AyaREffect, Sleep));
	renderState_.CreateState(MakeState(AyaREffect, Awake));
	renderState_.CreateState(MakeState(AyaREffect, Explode));
	renderState_.CreateState(MakeState(AyaREffect, Fade));
	renderState_ << "Sleep";
}

void AyaREffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void AyaREffect::startSleep()
{
	rangeRenderer_->Off();
	gatherRenderer_->Off();
	shotRenderer1_->Off();
	shotRenderer2_->Off();
	shotRenderer3_->Off();
	rangeRenderer_->GetTransform()->SetLocalScaling(rangeRenderer_->GetCurrentTexture()->GetTextureSize() * 4.5f);
	gatherRenderer_->GetTransform()->SetLocalScaling(gatherRenderer_->GetCurrentTexture()->GetTextureSize() * 2.5f);
	rangeScale_ = rangeRenderer_->GetTransform()->GetLocalScaling();
	gatherScale_ = gatherRenderer_->GetTransform()->GetLocalScaling();
	timer_ = 0.0f;
	spinCounter_ = 0.0f;
	gatherScale_ = gatherRenderer_->GetTransform()->GetLocalScaling();
}

void AyaREffect::updateSleep(float _deltaTime)
{
}

void AyaREffect::startAwake()
{
	rangeRenderer_->On();
	gatherRenderer_->On();

	timer_ = 0.0f;
}

void AyaREffect::updateAwake(float _deltaTime)
{

	timer_ += _deltaTime;

	spinCounter_ += 120.0f * _deltaTime;

	if (1.45f <= timer_)
	{
		timer_ = 0.0f;
		spinCounter_ = 0.0f;
		renderState_ << "Explode";
		return;
	}

	if (timer_ <= 0.17f)
	{
		rangeRenderer_->SetAlpha(timer_ / 0.2f);
	}

	if (0.17f >= timer_)
	{
		gatherRenderer_->SetAlpha(timer_ / 0.2f);
	
	}
	gatherRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, spinCounter_, 0.0f });
}

void AyaREffect::startExplode()
{
	shotRenderer1_->On();
	shotRenderer2_->On();
	shotRenderer3_->On();
	timer_ = 0.0f;
	fadeTimer_ = 0.3f;
	rangeScale_ = { 0.0f, 0.0f };
	gatherScale_ = { 0.0f, 0.0f };
}

void AyaREffect::updateExplode(float _deltaTime)
{

	timer_ += _deltaTime;
	fadeTimer_ -= _deltaTime;

	if (timer_ >= 0.1f)
	{
		timer_ = 0.0f;
		spinCounter_ = 0.0f;
		fadeTimer_ = 0.3f;
	
		rangeRenderer_->Off();
		gatherRenderer_->Off();
		renderState_ << "Fade";
		return;
	}

	spinCounter_ += 360.0f * _deltaTime;

	if (timer_ <= 0.1f)
	{
		shotRenderer1_->SetAlpha(timer_ / 0.1f);
		shotRenderer2_->SetAlpha(timer_ / 0.1f);
		shotRenderer3_->SetAlpha(timer_ / 0.1f);

		shotRenderer1_->GetTransform()->SetLocalRotationDegree({ 90.0f, spinCounter_, 0.0f });
		shotRenderer2_->GetTransform()->SetLocalRotationDegree({ 90.0f, spinCounter_, 0.0f });
		shotRenderer3_->GetTransform()->SetLocalRotationDegree({ 90.0f, spinCounter_, 0.0f });
	}

	rangeScale_ += { 3600.0f * _deltaTime, 3600.0f * _deltaTime };
	gatherScale_ += { 7200.0f * _deltaTime, 7200.0f * _deltaTime };

	rangeRenderer_->GetTransform()->SetLocalScaling(rangeRenderer_->GetCurrentTexture()->GetTextureSize() * 4.5f - rangeScale_);
	gatherRenderer_->GetTransform()->SetLocalScaling(gatherRenderer_->GetCurrentTexture()->GetTextureSize() * 2.5f - gatherScale_);
	rangeRenderer_->SetAlpha(fadeTimer_ / 0.3f);
	gatherRenderer_->SetAlpha(fadeTimer_ / 0.3f);
}


void AyaREffect::startFade()
{
	fadeTimer_ = 0.3f;
	rangeScale_ = { 0.0f, 0.0f };
	gatherScale_ = { 0.0f, 0.0f };

	rangeRenderer_->GetTransform()->SetLocalScaling(rangeRenderer_->GetCurrentTexture()->GetTextureSize() * 4.5f);
	gatherRenderer_->GetTransform()->SetLocalScaling(gatherRenderer_->GetCurrentTexture()->GetTextureSize() * 2.5f);
	rangeScale_ = rangeRenderer_->GetTransform()->GetLocalScaling();
	gatherScale_ = gatherRenderer_->GetTransform()->GetLocalScaling();
}


void AyaREffect::updateFade(float _deltaTime)
{
	fadeTimer_ -= _deltaTime;

	if (0.0f >= fadeTimer_)
	{
		renderState_ << "Sleep";
		return;
	}

	shotRenderer1_->SetAlpha(fadeTimer_ / 0.3f);
	shotRenderer2_->SetAlpha(fadeTimer_ / 0.3f);
	shotRenderer3_->SetAlpha(fadeTimer_ / 0.3f);
	

}

