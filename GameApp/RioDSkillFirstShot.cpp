#include "PreCompile.h"
#include "RioDSkillFirstShot.h"
#include <GameEngine/GameEngineEffectRenderer.h>

RioDSkillFirstShot::RioDSkillFirstShot() // default constructer 디폴트 생성자
{

}

RioDSkillFirstShot::~RioDSkillFirstShot() // default destructer 디폴트 소멸자
{

}

RioDSkillFirstShot::RioDSkillFirstShot(RioDSkillFirstShot&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void RioDSkillFirstShot::PlayAwake()
{
	renderState_ << "Awake";
}

void RioDSkillFirstShot::Start()
{
	arrowRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	arrowRenderer_->SetImage("FX_BI_HitGlow_04.png", "LinerSmp");
	arrowRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 100.0f, 0.0f });
	arrowRenderer_->GetTransform()->SetLocalRotationDegree({ 0.f,90.f, -30.f });
	arrowRenderer_->GetTransform()->SetLocalScaling(arrowRenderer_->GetCurrentTexture()->GetTextureSize() * 2.0f);
	arrowRenderer_->SetColor({ 1.0f, 1.0f, 0.2f });
	arrowRenderer_->Off();

	target0Renderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	target0Renderer_->SetImage("rioHit2.png", "LinerSmp");
	target0Renderer_->GetTransform()->SetLocalPosition({ 0.0f, 150.0f, 150.0f });
	target0Renderer_->GetTransform()->SetLocalRotationDegree({ -30.f, 0.f,0.f });
	target0Renderer_->GetTransform()->SetLocalScaling(target0Renderer_->GetCurrentTexture()->GetTextureSize() * 0.2f);
	target0Renderer_->SetColor({ 1.0f, 1.0f, 0.2f });
	target0Renderer_->Off();

	target1Renderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	target1Renderer_->SetImage("rioHit.png", "LinerSmp");
	target1Renderer_->GetTransform()->SetLocalPosition({ 0.0f, 150.0f, 100.0f });
	target1Renderer_->GetTransform()->SetLocalRotationDegree({ -30.f, 0.f,0.f });
	target1Renderer_->GetTransform()->SetLocalScaling(target1Renderer_->GetCurrentTexture()->GetTextureSize() * 0.2f);
	target1Renderer_->SetColor({ 1.0f, 1.0f, 0.2f });
	target1Renderer_->Off();

	renderState_.CreateState(MakeState(RioDSkillFirstShot, Sleep));
	renderState_.CreateState(MakeState(RioDSkillFirstShot, Awake));
	renderState_ << "Sleep";
}

void RioDSkillFirstShot::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void RioDSkillFirstShot::startSleep()
{
	arrowRenderer_->Off();
	target0Renderer_->Off();
	target1Renderer_->Off();
	fadeTimer_ = 0.5f;
	timer_ = 0.0f;
	arrowRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 20.0f, 0.0f });
	target0Renderer_->SetAlpha(1.0f);
	target1Renderer_->SetAlpha(1.0f);
}

void RioDSkillFirstShot::updateSleep(float _deltaTime)
{
}

void RioDSkillFirstShot::startAwake()
{
}

void RioDSkillFirstShot::updateAwake(float _deltaTime)
{
	timer_ += _deltaTime;

	

	if (timer_ >= 0.3f)
	{

		fadeTimer_ -= _deltaTime;

		arrowRenderer_->On();
		arrowRenderer_->GetTransform()->SetLocalMove({ 0.0f, 3600.0f * _deltaTime, 5400.0f * _deltaTime });
		target0Renderer_->On();
		target1Renderer_->On();

		target0Renderer_->SetAlpha(fadeTimer_/0.5f);
		target1Renderer_->SetAlpha(fadeTimer_ / 0.5f);

	}

	

	if (timer_ >= 1.0f)
	{
		renderState_ << "Sleep";
		return;
	}
}

