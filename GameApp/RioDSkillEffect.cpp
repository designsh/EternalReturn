#include "PreCompile.h"
#include "RioDSkillEffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include "RioDSkillArrow.h"

RioDSkillEffect::RioDSkillEffect() // default constructer 디폴트 생성자
	: zoneRenderer_(nullptr), impactRenderer_(nullptr), collision_(nullptr), timer_(0.0f), rotationContainer_(0.0f), impactOriginalScale_({0.0f,0.0f})
	, impactScaleContainer_({0.0f,0.0f}), shotTimer_(0.3f), targetScaleContainer_({0.0f,0.0f}), bHitsound_(false)
{

}

RioDSkillEffect::~RioDSkillEffect() // default destructer 디폴트 소멸자
{

}

RioDSkillEffect::RioDSkillEffect(RioDSkillEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void RioDSkillEffect::PlayAwake()
{
	renderState_ << "Awake";
}

void RioDSkillEffect::PlayShot()
{
}

void RioDSkillEffect::Start()
{
	zoneRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	zoneRenderer_->SetImage("wMask.png", "LinerSmp");
	zoneRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 20.0f, 0.0f });
	zoneRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	zoneRenderer_->GetTransform()->SetLocalScaling(zoneRenderer_->GetCurrentTexture()->GetTextureSize() * 3.0f);
	zoneRenderer_->SetAlpha(0.0f);
	zoneRenderer_->Off();

	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("rio_dskillround.png", "LinerSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 25.0f, 0.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f, 0.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize() * 2.0f);
	impactOriginalScale_ = impactRenderer_->GetTransform()->GetLocalScaling();
	impactRenderer_->SetAlpha(0.0f);
	impactRenderer_->Off();

	targetRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	targetRenderer_->SetImage("rio_dskillround.png", "LinerSmp");
	targetRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 25.0f, 0.0f });
	targetRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f, 0.f,0.f });
	targetRenderer_->GetTransform()->SetLocalScaling(targetRenderer_->GetCurrentTexture()->GetTextureSize() * 2.0f);
	targetOriginalScale_ = targetRenderer_->GetTransform()->GetLocalScaling();
	targetRenderer_->SetAlpha(0.0f);
	targetRenderer_->Off();

	for (int i = 0; i < 30; i++)
	{
		RioDSkillArrow* arrow = GetLevel()->CreateActor<RioDSkillArrow>();
		arrowList_.push_back(arrow);
	}

	renderState_.CreateState(MakeState(RioDSkillEffect, Sleep));
	renderState_.CreateState(MakeState(RioDSkillEffect, Awake));
	renderState_.CreateState(MakeState(RioDSkillEffect, Shot));
	renderState_ << "Sleep";
}

void RioDSkillEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void RioDSkillEffect::startSleep()
{
	impactRenderer_->Off();
	zoneRenderer_->Off();
	targetRenderer_->Off();
	zoneRenderer_->SetAlpha(0.0f);
	impactRenderer_->SetAlpha(0.0f);
	impactScaleContainer_ = { 0.0f, 0.0f };
	impactRenderer_->GetTransform()->SetLocalScaling(impactOriginalScale_);
	targetScaleContainer_ = { 0.0f, 0.0f };
	targetRenderer_->GetTransform()->SetLocalScaling(targetOriginalScale_);

}

void RioDSkillEffect::updateSleep(float _deltaTime)
{

}

void RioDSkillEffect::startAwake()
{
	impactRenderer_->On();
	zoneRenderer_->On();

}

void RioDSkillEffect::updateAwake(float _deltaTime)
{
	timer_ += _deltaTime;

	if (timer_ <= 1.0f)
	{
		impactRenderer_->SetAlpha(timer_);
		zoneRenderer_->SetAlpha(timer_);

		
	}

	if (timer_ >= 1.8f && false == bHitsound_)
	{
		FT::PlaySoundAndSendPacket("skillBow_hit.wav", transform_.GetWorldPosition());
		bHitsound_ = true;
	}
	
	if (timer_ >= 3.0f)
	{
		renderState_ << "Shot";
	}
	impactScaleContainer_ += {120.0f * _deltaTime, 120.0f * _deltaTime};
	impactRenderer_->GetTransform()->SetLocalScaling(impactOriginalScale_ + impactScaleContainer_);
}

void RioDSkillEffect::startShot()
{

	targetRenderer_->On();
	shotTimer_ = 0.3f;

	for (int i = 0; i < 30; i++)
	{
		arrowList_[i]->GetTransform()->SetWorldPosition(zoneRenderer_->GetTransform()->GetWorldPosition());
	//	arrowList_[i]->PlayAwake(300.0f, {0.0f, 0.0f, 0.0f});
		arrowList_[i]->PlayAwake(random_.RandomFloat(6400.0f, 7200.0f), { random_.RandomFloat(-35.f, 35.f) , 3000.0f, random_.RandomFloat(-35.f, 35.f) });
	}
}

void RioDSkillEffect::updateShot(float _deltaTime)
{
	targetScaleContainer_ += {360.0f * _deltaTime, 360.0f * _deltaTime};
	targetRenderer_->GetTransform()->SetLocalScaling(impactOriginalScale_ + impactScaleContainer_);
	targetRenderer_->SetAlpha(shotTimer_ / 0.3f);

	shotTimer_ -= _deltaTime;

	if (shotTimer_ <= 0.0f)
	{
		renderState_ << "Sleep";

		this->Release();

		return;
	}
}


