#include "PreCompile.h"
#include "HyunwooWEffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>

HyunwooWEffect::HyunwooWEffect() // default constructer 디폴트 생성자
{

}

HyunwooWEffect::~HyunwooWEffect() // default destructer 디폴트 소멸자
{

}

HyunwooWEffect::HyunwooWEffect(HyunwooWEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void HyunwooWEffect::PlayAwake()
{
	renderState_ << "Awake";
}

void HyunwooWEffect::Start()
{
	maskRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	maskRenderer_->SetImage("FX_BI_Shield_04.png", "LinerSmp");
	maskRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 350.0f, 0.0f });
	maskRenderer_->GetTransform()->SetLocalRotationDegree({ 60.f,-35.f,0.f });
	maskRenderer_->GetTransform()->SetLocalScaling(maskRenderer_->GetCurrentTexture()->GetTextureSize() * 0.75f);
	maskRenderer_->SetColor({ 1.0f, 0.84f, 0.0f });
	maskRenderer_->SetAlpha(0.0f);
	maskRenderer_->Off();

	shieldRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	shieldRenderer_->SetImage("FX_BI_Shield_01.png", "LinerSmp");
	shieldRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 350.0f, 0.0f });
	shieldRenderer_->GetTransform()->SetLocalRotationDegree({ 60.f,-35.f,0.f });
	shieldRenderer_->GetTransform()->SetLocalScaling(shieldRenderer_->GetCurrentTexture()->GetTextureSize() * 0.75f);
	shieldRenderer_->SetColor({ 1.0f, 0.84f, 0.0f });
	shieldRenderer_->SetAlpha(0.0f);
	shieldRenderer_->Off();

	renderState_.CreateState(MakeState(HyunwooWEffect, Sleep));
	renderState_.CreateState(MakeState(HyunwooWEffect, Awake));
	renderState_.CreateState(MakeState(HyunwooWEffect, Fade));
	renderState_ << "Sleep";
}

void HyunwooWEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void HyunwooWEffect::startSleep()
{
	maskRenderer_->Off();
	shieldRenderer_->Off();

	maskRenderer_->SetAlpha(0.0f);
	shieldRenderer_->SetAlpha(0.0f);
}

void HyunwooWEffect::updateSleep(float _deltaTime)
{
}

void HyunwooWEffect::startAwake()
{
	awakeTimer_ = 0.0f;
	maskRenderer_->On();
	shieldRenderer_->On();
}

void HyunwooWEffect::updateAwake(float _deltaTime)
{
	awakeTimer_ += _deltaTime;

	if (0.5f >= awakeTimer_)
	{
		maskRenderer_->SetAlpha(awakeTimer_ / 0.6f);
		shieldRenderer_->SetAlpha(awakeTimer_ / 0.5f);
	}

	if (1.0f <= awakeTimer_)
	{
		maskRenderer_->SetAlpha(1.0f);
		shieldRenderer_->SetAlpha(1.0f);
		renderState_ << "Fade";
		return;
	}
}

void HyunwooWEffect::startFade()
{
	fadeTimer_ = 0.5f;
}

void HyunwooWEffect::updateFade(float _deltaTime)
{
	fadeTimer_ -= _deltaTime;

	maskRenderer_->SetAlpha(fadeTimer_ / 0.5f);
	shieldRenderer_->SetAlpha(fadeTimer_ / 0.5f);

	if (0.0f >= fadeTimer_)
	{


		
		renderState_ << "Sleep";
		return;
	}
}

