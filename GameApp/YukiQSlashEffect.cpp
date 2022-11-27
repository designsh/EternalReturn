#include "PreCompile.h"
#include "YukiQSlashEffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
YukiQSlashEffect::YukiQSlashEffect() // default constructer 디폴트 생성자
{

}

YukiQSlashEffect::~YukiQSlashEffect() // default destructer 디폴트 소멸자
{

}

YukiQSlashEffect::YukiQSlashEffect(YukiQSlashEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void YukiQSlashEffect::PlayBurst(float4 _pos)
{
	targetPos_ = _pos;
	renderState_ << "Burst";
}

void YukiQSlashEffect::Stop()
{
	renderState_ << "Sleep";
}

void YukiQSlashEffect::Start()
{

	GameEngineTexture* slashTexture = GameEngineTextureManager::GetInst().Find("FX_BI_SwordLine_01SE.png");
	slashTexture->Cut(12, 1);

	GameEngineTexture* burstTexture = GameEngineTextureManager::GetInst().Find("qburst.png");
	burstTexture->Cut(2, 2);

	kiBurstRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	kiBurstRenderer_->SetImage("qburst.png", "LinerSmp");
	kiBurstRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	kiBurstRenderer_->GetTransform()->SetLocalRotationDegree({ 60.f,-35.f,0.f });
	kiBurstRenderer_->GetTransform()->SetLocalScaling(kiBurstRenderer_->GetCurrentTexture()->GetTextureSize());
	kiBurstRenderer_->CreateAnimation("qburst.png", "qburst", 0, 3, 0.04f, false);
	kiBurstRenderer_->SetChangeAnimation("qburst", true);
	kiBurstRenderer_->SetColor({ 0.8f, 0.8f, 1.0f });
	kiBurstRenderer_->Off();

	float4 slashOriSclae = { 42.7f, 512.0f };
	slashRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	slashRenderer_->SetImage("FX_BI_SwordLine_01SE.png", "LinerSmp");
	slashRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	slashRenderer_->GetTransform()->SetLocalRotationDegree({ 0.f, -35.f,0.f });
	slashRenderer_->GetTransform()->SetLocalScaling(slashOriSclae * 3.0f);
	slashRenderer_->CreateAnimation("FX_BI_SwordLine_01SE.png", "FX_BI_SwordLine_01SE", 0, 11, 0.02f, false);
	slashRenderer_->SetChangeAnimation("FX_BI_SwordLine_01SE", true);
	slashRenderer_->Off();

	renderState_.CreateState(MakeState(YukiQSlashEffect, Sleep));
	renderState_.CreateState(MakeState(YukiQSlashEffect, Burst));
	renderState_ << "Sleep";
}

void YukiQSlashEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void YukiQSlashEffect::startSleep()
{
	timer_ = 0.0f;
	kiBurstRenderer_->Off();
	slashRenderer_->Off();
}

void YukiQSlashEffect::updateSleep(float _deltaTime)
{
}

void YukiQSlashEffect::startBurst()
{
	timer_ = 0.0f;

	kiBurstRenderer_->GetTransform()->SetWorldPosition(targetPos_);
	slashRenderer_->GetTransform()->SetWorldPosition(targetPos_);
}

void YukiQSlashEffect::updateBurst(float _deltaTime)
{
	timer_ += _deltaTime;

	if (0.1f >= timer_)
	{
		slashRenderer_->On();
		slashRenderer_->SetChangeAnimation("FX_BI_SwordLine_01SE", true);
		slashRenderer_->AnimationPlay();

		kiBurstRenderer_->On();
		kiBurstRenderer_->SetChangeAnimation("qburst", true);
		kiBurstRenderer_->AnimationPlay();
		renderState_ << "Sleep";
	}
}

