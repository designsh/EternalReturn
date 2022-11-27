#include "PreCompile.h"
#include "BasicAttackEffect.h"

#include <GameEngine/GameEngineEffectRenderer.h>

BasicAttackEffect::BasicAttackEffect() // default constructer ����Ʈ ������
	: atkRenderer_(nullptr)
{

}

BasicAttackEffect::~BasicAttackEffect() // default destructer ����Ʈ �Ҹ���
{

}

BasicAttackEffect::BasicAttackEffect(BasicAttackEffect&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void BasicAttackEffect::PlayAwake(const std::string& _animationName)
{
	animationName_ = _animationName;
	renderState_ << "Awake";
}

void BasicAttackEffect::PlayFade(float _time)
{
	SetFadeTime(_time);
	renderState_ << "Fade";
}

void BasicAttackEffect::SetFadeTime(float _time)
{
	fadeTime_ = _time;
	standardTime_ = _time;
}

void BasicAttackEffect::Start()
{
	atkRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	atkRenderer_->SetImage("effectDummy.png", "PointSmp");

	renderState_.CreateState(MakeState(BasicAttackEffect, Sleep));
	renderState_.CreateState(MakeState(BasicAttackEffect, Awake));
	renderState_.CreateState(MakeState(BasicAttackEffect, Fade));
	renderState_ << "Sleep";
}

void BasicAttackEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void BasicAttackEffect::startSleep()
{

	atkRenderer_->Off();
}

void BasicAttackEffect::updateSleep(float _deltaTime)
{
}

void BasicAttackEffect::startAwake()
{
	atkRenderer_->On();
	atkRenderer_->SetChangeAnimation(animationName_, true);
	atkRenderer_->AnimationPlay();
}

void BasicAttackEffect::updateAwake(float _deltaTime)
{
	if (true == atkRenderer_->IsCurAnimationEnd())
	{

		renderState_ << "Sleep";
	}
}

void BasicAttackEffect::startFade()
{
	atkRenderer_->On();
	atkRenderer_->SetAlpha(1.0f);
}

void BasicAttackEffect::updateFade(float _deltaTime)
{
	fadeTime_ -= _deltaTime;

	if (fadeTime_ <= 0.0f)
	{
		fadeTime_ = 0.0f;
		standardTime_ = 0.0f;
		atkRenderer_->Off();
		renderState_ << "Sleep";
		return;
	}

	atkRenderer_->SetAlpha(fadeTime_ / standardTime_);

}

