#include "PreCompile.h"
#include "HyunwooREffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include "Hyunwoo.h"
HyunwooREffect::HyunwooREffect() // default constructer 디폴트 생성자
	: isActivated_(false), isTriggered_(false), timer_(0.0f)
{

}

HyunwooREffect::~HyunwooREffect() // default destructer 디폴트 소멸자
{

}

HyunwooREffect::HyunwooREffect(HyunwooREffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void HyunwooREffect::SetMyCharacter(Hyunwoo* _character)
{
	myCharacter_ = _character;
}

void HyunwooREffect::PlayAwake()
{
	renderState_ << "Awaken";
}

void HyunwooREffect::PlayExplode()
{
	renderState_ << "Explode";
}

void HyunwooREffect::SetSleepMandatory()
{
	renderState_ << "Sleeping";
}

void HyunwooREffect::Start()
{
	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_William_Skill04_ExpLine2X3.png");
	hitBase->Cut(2, 3);
	
	hitBoxRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();	
	hitBoxRenderer_->SetImage("FX_BI_Jan_Skill01_01_Range_add.png", "PointSmp");
	hitBoxRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 150.0f });
	hitBoxRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	hitBoxRenderer_->GetTransform()->SetLocalScaling({350.0f, 10.0f, 0.0f});
	hitBoxRenderer_->SetAlpha(1.0f);
	
	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("FX_BI_William_Skill04_ExpLine2X3.png", "PointSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 30.0f, 480.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,-90.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize() * 3);
	impactRenderer_->CreateAnimation("FX_BI_William_Skill04_ExpLine2X3.png", "FX_BI_William_Skill04_ExpLine2X3", 0, 5, 0.03f, false);
	impactRenderer_->SetChangeAnimation("FX_BI_William_Skill04_ExpLine2X3", true);
	impactRenderer_->SetAlpha(0.85f);

	renderState_.CreateState(MakeState(HyunwooREffect, Sleeping));
	renderState_.CreateState(MakeState(HyunwooREffect, Awaken));
	renderState_.CreateState(MakeState(HyunwooREffect, Explode));

	renderState_ << "Sleeping";
}

void HyunwooREffect::Update(float _deltaTime)
{


	renderState_.Update(_deltaTime);
}

void HyunwooREffect::startAwaken()
{
	timer_ = 0.0f;
	hitBoxRenderer_->On();
	hitBoxRenderer_->GetTransform()->SetLocalScaling({ 650.0f, 150.0f, 0.0f });
}

void HyunwooREffect::updateAwaken(float _deltaTime)
{
	//if (true == isTriggered_)
	//{
	//	renderState_ << "Explode";
	//	return;
	// 
	// 
	//}

	if ("SkillR_start" != myCharacter_->curAnimationName_ && "SkillR_loop" != myCharacter_->curAnimationName_)
	{
		renderState_ << "Explode";	
		return;
	}

	timer_ += _deltaTime;

	hitBoxRenderer_->SetAlpha(1.0f);
	hitBoxRenderer_->GetTransform()->SetLocalPosition({ 0.f,10.f,150.f + (timer_ * 50.0f) });
	hitBoxRenderer_->GetTransform()->SetLocalScaling({ 650.0f, 150.0f + (timer_ * 100.0f), 0.0f });
}

void HyunwooREffect::startExplode()
{
	hitBoxRenderer_->Off();
	impactRenderer_->SetAlpha(0.85f);
	impactRenderer_->SetChangeAnimation("FX_BI_William_Skill04_ExpLine2X3", true);
	impactRenderer_->AnimationPlay();

	//if (myCharacter_ != nullptr)
	//{
	//	
	//}

}

void HyunwooREffect::updateExplode(float _deltaTime)
{
	impactRenderer_->SetAlpha(0.95f);
	if (true == impactRenderer_->IsCurAnimationEnd())
	{
		renderState_ << "Sleeping";
		return;
	}
}



void HyunwooREffect::startSleeping()
{
	hitBoxRenderer_->SetAlpha(0.0f);
	impactRenderer_->SetAlpha(0.0f);

	isActivated_ = false;
	isTriggered_ = false;

	hitBoxRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 150.0f });
	hitBoxRenderer_->GetTransform()->SetLocalScaling(hitBoxRenderer_->GetCurrentTexture()->GetTextureSize());

	timer_ = 0.0f;
}

void HyunwooREffect::updateSleeping(float _deltaTime)
{
	//if (true == isActivated_)
	//{
	//	renderState_ << "Awaken";
	//	return;
	//}
}

