#include "PreCompile.h"
#include "LevelUpEffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
LevelUpEffect::LevelUpEffect() // default constructer 디폴트 생성자
	: timer_(1.5f), levelUpRenderer_(nullptr), glowLineRenderer_(nullptr), ringRenderer_(nullptr), rotationDegree_(0.0f), glowTimer_(0.8f), scaleContainer_({0.0f,0.0f,0.0f,0.0f})
{

}

LevelUpEffect::~LevelUpEffect() // default destructer 디폴트 소멸자
{

}

LevelUpEffect::LevelUpEffect(LevelUpEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void LevelUpEffect::PlayAwake()
{
	renderState_ << "Awake";
}

void LevelUpEffect::Hide(bool _tf)
{
	levelUpRenderer_->SetAlpha(0.0f);
	glowLineRenderer_->SetAlpha(0.0f);
	ringRenderer_->SetAlpha(0.0f);
}

void LevelUpEffect::Start()
{
	levelUpRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	levelUpRenderer_->SetImage("FX_BI_Levelup_01.png", "PointSmp");
	levelUpRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 380.0f, 0.0f });
	levelUpRenderer_->GetTransform()->SetLocalRotationDegree({60.0f, -35.0f ,0.0f});
	levelUpRenderer_->GetTransform()->SetLocalScaling(levelUpRenderer_->GetCurrentTexture()->GetTextureSize() * 0.8f);
	levelUpRenderer_->Off();

	glowLineRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	glowLineRenderer_->SetImage("FX_BI_GlowLine_01.png", "PointSmp");
	glowLineRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 0.0f });
	glowLineRenderer_->GetTransform()->SetLocalRotationDegree({ 60.0f, -35.0f, 0.0f });
	glowLineRenderer_->GetTransform()->SetLocalScaling(glowLineRenderer_->GetCurrentTexture()->GetTextureSize() * 2.0f);
	glowLineRenderer_->Off();

	ringRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	ringRenderer_->SetImage("levelupring.png", "PointSmp");
	ringRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 0.0f });
	ringRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f,0.0f,0.0f });
	ringRenderer_->GetTransform()->SetLocalScaling(ringRenderer_->GetCurrentTexture()->GetTextureSize() * 1.2f);
	ringRenderer_->Off();

	renderState_.CreateState(MakeState(LevelUpEffect, Sleep));
	renderState_.CreateState(MakeState(LevelUpEffect, Awake));
	renderState_.CreateState(MakeState(LevelUpEffect, Fade));
	renderState_ << "Sleep";
}

void LevelUpEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void LevelUpEffect::startAwake()
{
	levelUpRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 380.0f, 0.0f });
	levelUpRenderer_->SetAlpha(1.0f);

	glowLineRenderer_->SetAlpha(0.8f);
	glowLineRenderer_->GetTransform()->SetLocalScaling(glowLineRenderer_->GetCurrentTexture()->GetTextureSize() * 2.0f);

	ringRenderer_->GetTransform()->SetLocalScaling(ringRenderer_->GetCurrentTexture()->GetTextureSize() * 1.2f);

	levelUpRenderer_->On();
	glowLineRenderer_->On();
	ringRenderer_->On();

	glowTimer_ = 0.8f;
	rotationDegree_ = 0.0f;
	timer_ = 1.5f;
	scaleContainer_ = { 0.0f, 0.0f };

	GameEngineSoundManager::GetInstance()->PlaySoundByName("effect_levelup.wav");
}

void LevelUpEffect::updateAwake(float _deltaTime)
{
	timer_ -= _deltaTime;
	glowTimer_ -= _deltaTime;
	if (timer_ <= 0.0f)
	{
		timer_ = 1.5f;
		renderState_ << "Fade";
		return;
	}

	rotationDegree_ += 60.0f * _deltaTime;
	scaleContainer_ += {120.0f * _deltaTime, 120.0f * _deltaTime};
	if (400.0f >= levelUpRenderer_->GetTransform()->GetLocalPosition().y)
	{
		levelUpRenderer_->GetTransform()->SetLocalMove({ 0.0f, 20.0f * _deltaTime, 0.0f }); // 위로 올라가면서 투명
	}

	ringRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, rotationDegree_, 0.0f });
	ringRenderer_->GetTransform()->SetLocalScaling(ringRenderer_->GetCurrentTexture()->GetTextureSize() * 1.2f + scaleContainer_);
	ringRenderer_->SetAlpha(timer_/1.5f);

	glowLineRenderer_->GetTransform()->SetLocalScaling(glowLineRenderer_->GetCurrentTexture()->GetTextureSize() * 2.0f + scaleContainer_);
	glowLineRenderer_->SetAlpha(glowTimer_);


}

void LevelUpEffect::startFade()
{
	ringRenderer_->Off();
	glowLineRenderer_->Off();
}

void LevelUpEffect::updateFade(float _deltaTime)
{
	timer_ -= _deltaTime;

	if (timer_ <= 0.0f)
	{
		timer_ = 1.5f;
		renderState_ << "Sleep";
		return;
	}

	levelUpRenderer_->SetAlpha(timer_ / 1.5f);

}



void LevelUpEffect::startSleep()
{
	levelUpRenderer_->Off();
	glowLineRenderer_->Off();
	ringRenderer_->Off();
	timer_ = 1.0f;
	rotationDegree_ = 0.0f;

	glowLineRenderer_->SetAlpha(0.7f);
	levelUpRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 400.0f, 0.0f });
	levelUpRenderer_->SetAlpha(1.0f);

	glowLineRenderer_->GetTransform()->SetLocalScaling(glowLineRenderer_->GetCurrentTexture()->GetTextureSize());

	ringRenderer_->GetTransform()->SetLocalScaling(ringRenderer_->GetCurrentTexture()->GetTextureSize());
}

void LevelUpEffect::updateSleep(float _deltaTime)
{
}

