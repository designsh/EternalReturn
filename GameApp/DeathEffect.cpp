#include "PreCompile.h"
#include "DeathEffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>

DeathEffect::DeathEffect() // default constructer 디폴트 생성자
	: isExplosion_(false), barBreakTimer_(0.0f), barEffectTimer_(1.0f), barBreakRenderer_(nullptr), barEffectRenderer_(nullptr), restrictionExplodeRenderer_(nullptr), scaleContainer_({0.0f,0.0f})
{

}

DeathEffect::~DeathEffect() // default destructer 디폴트 소멸자
{

}

DeathEffect::DeathEffect(DeathEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void DeathEffect::PlayAwake(bool _isProhibitedArea)
{
	isExplosion_ = _isProhibitedArea;
	renderState_ << "Awake";
}

void DeathEffect::Start()
{
	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_UI_HPDestruction3.png");
	hitBase->Cut(3, 3);

	GameEngineTexture* hitBase1 = GameEngineTextureManager::GetInst().Find("FX_BI_Flame_01SE.png");
	hitBase1->Cut(5, 5);

	restrictionExplodeRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	restrictionExplodeRenderer_->SetImage("FX_BI_Flame_01SE.png", "PointSmp");
	restrictionExplodeRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 200.0f, 0.0f });
	restrictionExplodeRenderer_->GetTransform()->SetLocalRotationDegree({ 60.0f, -35.0f,0.0f });
	restrictionExplodeRenderer_->GetTransform()->SetLocalScaling(restrictionExplodeRenderer_->GetCurrentTexture()->GetTextureSize() * 0.5f);
	restrictionExplodeRenderer_->CreateAnimation("FX_BI_Flame_01SE.png", "FX_BI_Flame_01SE", 0, 24, 0.01f, false);
	//restrictionExplodeRenderer_->SetChangeAnimation("FX_BI_Flame_01SE", true);
	restrictionExplodeRenderer_->Off();

	barBreakRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	barBreakRenderer_->SetImage("FX_UI_HPDestruction3.png", "PointSmp");
	barBreakRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 280.0f, 0.0f });
	barBreakRenderer_->GetTransform()->SetLocalRotationDegree({ 60.0f, -35.0f ,0.0f });
	barBreakRenderer_->GetTransform()->SetLocalScaling(barBreakRenderer_->GetCurrentTexture()->GetTextureSize() * 0.33f);
	barBreakRenderer_->CreateAnimation("FX_UI_HPDestruction3.png", "FX_UI_HPDestruction3", 0, 8, 0.03f, false);
	//barBreakRenderer_->SetChangeAnimation("FX_UI_HPDestruction3", true);
	barBreakRenderer_->Off();

	barEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	barEffectRenderer_->SetImage("Fx_Death.png", "PointSmp");
	barEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 280.0f, 0.0f });
	barEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 60.0f, -35.0f, 0.0f });
	barEffectRenderer_->GetTransform()->SetLocalScaling(barEffectRenderer_->GetCurrentTexture()->GetTextureSize());
	barEffectRenderer_->Off();



	renderState_.CreateState(MakeState(DeathEffect, Sleep));
	renderState_.CreateState(MakeState(DeathEffect, Awake));
	renderState_ << "Sleep";
}

void DeathEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void DeathEffect::startSleep()
{
	barBreakTimer_ = 0.0f;
	barEffectTimer_ = 1.0f;
	barBreakRenderer_->Off();
	barEffectRenderer_->Off();
	barEffectRenderer_->SetAlpha(1.0f);
	restrictionExplodeRenderer_->Off();
	barEffectRenderer_->GetTransform()->SetLocalScaling(barEffectRenderer_->GetCurrentTexture()->GetTextureSize());
	scaleContainer_ = { 0.0f, 0.0f };
}

void DeathEffect::updateSleep(float _deltaTime)
{
}

void DeathEffect::startAwake()
{
	barBreakTimer_ = 0.0f;
	barEffectTimer_ = 1.0f;
	barBreakRenderer_->On();
	
	if (true == isExplosion_)
	{
		restrictionExplodeRenderer_->On();
		FT::PlaySoundAndSendPacket("Sfx_RestrictedDamage.wav", transform_.GetWorldPosition());
		restrictionExplodeRenderer_->SetChangeAnimation("FX_BI_Flame_01SE", true);
	}

	barEffectRenderer_->On();
	barEffectRenderer_->SetAlpha(1.0f);
	FT::PlaySoundAndSendPacket("death.wav", transform_.GetWorldPosition());
	barBreakRenderer_->SetChangeAnimation("FX_UI_HPDestruction3", true);
}

void DeathEffect::updateAwake(float _deltaTime)
{
	barBreakTimer_ += _deltaTime;
	barEffectTimer_ -= _deltaTime;

	scaleContainer_ += {120.0f * _deltaTime, 120.0f * _deltaTime};
	barEffectRenderer_->SetAlpha(barEffectTimer_);
	barEffectRenderer_->GetTransform()->SetLocalScaling(barEffectRenderer_->GetCurrentTexture()->GetTextureSize()  + scaleContainer_);


	if (barEffectTimer_ <= 0.0f)
	{
		isExplosion_ = false;
		renderState_ << "Sleep";
		return;
	}
}

