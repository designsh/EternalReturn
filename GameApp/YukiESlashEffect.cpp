#include "PreCompile.h"
#include "YukiESlashEffect.h"

#include <GameEngine/GameEngineEffectRenderer.h>
YukiESlashEffect::YukiESlashEffect() // default constructer 디폴트 생성자
	: timer_(0.5f), slashRenderer_(nullptr)
{

}

YukiESlashEffect::~YukiESlashEffect() // default destructer 디폴트 소멸자
{

}

YukiESlashEffect::YukiESlashEffect(YukiESlashEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void YukiESlashEffect::PlayAwake()
{
	renderState_ << "Awake";
}

void YukiESlashEffect::Start()
{
	slashRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	slashRenderer_->SetImage("FX_BI_Sword_01.png", "LinerSmp");
	slashRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 100.0f, 0.0f });
	slashRenderer_->GetTransform()->SetLocalRotationDegree({ -90.0f, -30.0f, 0.0f });
	slashRenderer_->GetTransform()->SetLocalScaling({ 640.0f, 640.0f });
	slashRenderer_->Off();

	renderState_.CreateState(MakeState(YukiESlashEffect, Sleep));
	renderState_.CreateState(MakeState(YukiESlashEffect, Awake));
	renderState_ << "Sleep";
}

void YukiESlashEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void YukiESlashEffect::startSleep()
{	
	timer_ = 0.5f;
	slashRenderer_->Off();
	slashRenderer_->SetAlpha(1.0f);
}

void YukiESlashEffect::updateSleep(float _deltaTime)
{
}

void YukiESlashEffect::startAwake()
{
	slashRenderer_->On();

}

void YukiESlashEffect::updateAwake(float _deltaTime)
{
	timer_ -= _deltaTime;

	if (timer_ <= 0.0f)
	{
		slashRenderer_->SetAlpha(0.0f);
		renderState_ << "Sleep";
		return;
	}

	slashRenderer_->SetAlpha(timer_/0.5f);
}

