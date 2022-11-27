#include "PreCompile.h"
#include "HyunwooQEffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>

HyunwooQEffect::HyunwooQEffect() // default constructer 디폴트 생성자
	: groundCrackRenderer_(nullptr), dustRenderer_(nullptr), isActivated_(false), crackFadeTime_(1.0f), timer_(0.0f)
{

}

HyunwooQEffect::~HyunwooQEffect() // default destructer 디폴트 소멸자
{

}

HyunwooQEffect::HyunwooQEffect(HyunwooQEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void HyunwooQEffect::PlayAwake()
{
	renderState_ << "Awake";
}

void HyunwooQEffect::Start()
{
	groundCrackRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	groundCrackRenderer_->SetImage("FX_BI_GroundBomb_01.png", "PointSmp");
	groundCrackRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 40.0f, 150.0f});
	groundCrackRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	groundCrackRenderer_->GetTransform()->SetLocalScaling(groundCrackRenderer_->GetCurrentTexture()->GetTextureSize());
	groundCrackRenderer_->Off();

	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("FX_BI_UpSmoke_02.png", "PointSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 250.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,-90.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize() * 2.5f);
	impactRenderer_->Off();

	renderState_.CreateState(MakeState(HyunwooQEffect, Sleep));
	renderState_.CreateState(MakeState(HyunwooQEffect, Awake));
	renderState_ << "Sleep";
}

void HyunwooQEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void HyunwooQEffect::startSleep()
{

//	groundCrackRenderer_->Off();
//	groundCrackRenderer_->SetAlpha(0.0f);
//	impactRenderer_->SetAlpha(0.0f);
//	impactRenderer_->Off();

	timer_ = 0.8f;
	crackFadeTime_ = 0.8f;
}

void HyunwooQEffect::updateSleep(float _deltaTime)
{
	if (true == isActivated_)
	{
		renderState_ << "Awake";
		return;
	}
}

void HyunwooQEffect::startAwake()
{

	groundCrackRenderer_->SetAlpha(1.0f);
	groundCrackRenderer_->On();
	impactRenderer_->SetAlpha(1.0f);
	impactRenderer_->On();
	impactRenderer_->AnimationPlay();
	crackFadeTime_ = 1.0f;
}

void HyunwooQEffect::updateAwake(float _deltaTime)
{

	crackFadeTime_ -= _deltaTime;
	timer_ -= _deltaTime;
	groundCrackRenderer_->SetColor({ crackFadeTime_, 0.0f, 0.0f });
	groundCrackRenderer_->SetAlpha(crackFadeTime_);
	impactRenderer_->SetAlpha(crackFadeTime_);

	if (0.0f >= crackFadeTime_)
	{
		timer_ = 0.0f;
		crackFadeTime_ = 1.0f;
		groundCrackRenderer_->Off();
		groundCrackRenderer_->SetAlpha(1.0f);

		impactRenderer_->Off();
		impactRenderer_->SetAlpha(1.0f);
		renderState_ << "Sleep";

		return;
	}
}
