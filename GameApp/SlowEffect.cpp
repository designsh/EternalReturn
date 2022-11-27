#include "PreCompile.h"
#include "SlowEffect.h"

#include <GameEngine/GameEngineEffectRenderer.h>

SlowEffect::SlowEffect() // default constructer 디폴트 생성자
	:timer_(0.0f), zoneRenderer0_(nullptr), zoneRenderer1_(nullptr), zoneRenderer2_(nullptr), rotationDegree_(0.0f)
{

}

SlowEffect::~SlowEffect() // default destructer 디폴트 소멸자
{

}

SlowEffect::SlowEffect(SlowEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void SlowEffect::PlayAwake(float _time)
{
	timer_ = _time;

	if (0.0f >= timer_)
	{
		timer_ = 0.0f;
	}

	renderState_ << "Awake";
}

void SlowEffect::Start()
{
	zoneRenderer0_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	zoneRenderer0_->SetImage("slow.png", "PointSmp");

	zoneRenderer0_->GetTransform()->SetLocalRotationDegree({ 90.f, 0.0f, 0.f });
	zoneRenderer0_->GetTransform()->SetLocalPosition({ 0.0f, 30.0f, 0.0f });

	zoneRenderer0_->GetTransform()->SetLocalScaling(zoneRenderer0_->GetCurrentTexture()->GetTextureSize());
	zoneRenderer0_->Off();

	zoneRenderer1_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	zoneRenderer1_->SetImage("slow.png", "PointSmp");

	zoneRenderer1_->GetTransform()->SetLocalRotationDegree({ 60.f,0.0f,0.f });
	zoneRenderer1_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 0.0f });

	zoneRenderer1_->GetTransform()->SetLocalScaling(zoneRenderer1_->GetCurrentTexture()->GetTextureSize() * 0.9f);
	zoneRenderer1_->Off();

	zoneRenderer2_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	zoneRenderer2_->SetImage("slow.png", "PointSmp");

	zoneRenderer2_->GetTransform()->SetLocalRotationDegree({ 115.f,0.0f,0.f });
	zoneRenderer2_->GetTransform()->SetLocalPosition({ 0.0f, 70.0f, 0.0f });

	zoneRenderer2_->GetTransform()->SetLocalScaling(zoneRenderer2_->GetCurrentTexture()->GetTextureSize() * 0.85f);
	zoneRenderer2_->Off();

	renderState_.CreateState(MakeState(SlowEffect, Sleep));
	renderState_.CreateState(MakeState(SlowEffect, Awake));
	renderState_ << "Sleep";

}

void SlowEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void SlowEffect::startAwake()
{
	zoneRenderer0_->On();
	zoneRenderer1_->On();
	zoneRenderer2_->On();


}

void SlowEffect::updateAwake(float _deltaTime)
{
	if (timer_ <= 0.0f)
	{
		timer_ = 0.0f;
		renderState_ << "Sleep";
		return;
	}

	rotationDegree_ += 80.0f * _deltaTime;

	timer_ -= _deltaTime;
	zoneRenderer0_->GetTransform()->SetLocalRotationDegree({90.0f, rotationDegree_, 0.0f});
	zoneRenderer1_->GetTransform()->SetLocalRotationDegree({60.0f, rotationDegree_, 0.0f });
	zoneRenderer2_->GetTransform()->SetLocalRotationDegree({115.0f, rotationDegree_, 0.0f });

}

void SlowEffect::startSleep()
{
	zoneRenderer0_->Off();
	zoneRenderer1_->Off();
	zoneRenderer2_->Off();
	timer_ = 0.0f;
}

void SlowEffect::updateSleep(float _deltaTime)
{

	return;
}

