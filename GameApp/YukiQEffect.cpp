#include "PreCompile.h"
#include "YukiQEffect.h"
#include "GameEngine/GameEngineEffectRenderer.h"

YukiQEffect::YukiQEffect() // default constructer 디폴트 생성자
	: kiCoreRenderer_(nullptr), kiGatheringRenderer_(nullptr), timer_(0.0f)
{

}

YukiQEffect::~YukiQEffect() // default destructer 디폴트 소멸자
{

}

YukiQEffect::YukiQEffect(YukiQEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void YukiQEffect::PlayAwake()
{
	timer_ = 0.0f;
	renderState_ << "Awake";
}


void YukiQEffect::Stop()
{
	renderState_ << "Sleep";
}

void YukiQEffect::Start()
{
	GameEngineTexture* kiGatheringTexture = GameEngineTextureManager::GetInst().Find("FX_BI_Dust_04.png");
	kiGatheringTexture->Cut(6, 6);

	kiCoreRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	kiCoreRenderer_->SetImage("Fx_UI_Tierlight_OW.png", "PointSmp");
	kiCoreRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 150.0f, 30.0f });
	kiCoreRenderer_->GetTransform()->SetLocalRotationDegree({ 60.f,-35.f,0.f });
	kiCoreRenderer_->GetTransform()->SetLocalScaling(kiCoreRenderer_->GetCurrentTexture()->GetTextureSize());
	kiCoreRenderer_->SetColor({ 0.8f, 0.8f, 1.0f });
	kiCoreRenderer_->Off();

	kiGatheringRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	kiGatheringRenderer_->SetImage("FX_BI_Dust_04.png", "PointSmp");
	kiGatheringRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 150.0f, 30.0f });
	kiGatheringRenderer_->GetTransform()->SetLocalRotationDegree({ 60.f,-35.f,0.f });
	kiGatheringRenderer_->GetTransform()->SetLocalScaling(kiGatheringRenderer_->GetCurrentTexture()->GetTextureSize());
	kiGatheringRenderer_->CreateAnimation("FX_BI_Dust_04.png", "FX_BI_Dust_04", 0, 35, 0.02f, false);
	//kiGatheringRenderer_->SetChangeAnimation("FX_BI_Dust_04", true);
	kiGatheringRenderer_->SetColor({ 0.8f, 0.8f, 1.0f });
	kiGatheringRenderer_->Off();



	renderState_.CreateState(MakeState(YukiQEffect, Sleep));
	renderState_.CreateState(MakeState(YukiQEffect, Awake));
	renderState_ << "Sleep";
}

void YukiQEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void YukiQEffect::startSleep()
{
	timer_ = 0.0f;
	kiCoreRenderer_->Off();
	kiGatheringRenderer_->Off();

}

void YukiQEffect::updateSleep(float _deltaTime)
{
}

void YukiQEffect::startAwake()
{
	timer_ = 1.0f;
	kiCoreRenderer_->On();
	kiGatheringRenderer_->On();
	kiGatheringRenderer_->SetChangeAnimation("FX_BI_Dust_04", true);
	kiGatheringRenderer_->AnimationPlay();
	scaleContainer_ = {512.0f, 512.0f};
}

void YukiQEffect::updateAwake(float _deltaTime)
{
	if (0.0f >= timer_)
	{
		kiCoreRenderer_->Off();
		kiGatheringRenderer_->Off();
		return;
	}

	timer_ -= _deltaTime;

	rotationDegree_ += 720.0f * _deltaTime;

	if (0.0f <= scaleContainer_.x)
	{
		scaleContainer_ -= {360.0f * _deltaTime, 360.0f * _deltaTime};
	}

	kiCoreRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, rotationDegree_, 0.0f });
	kiCoreRenderer_->GetTransform()->SetLocalScaling(scaleContainer_);

	//kiGatheringRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, rotationDegree_, 0.0f });
	kiGatheringRenderer_->GetTransform()->SetLocalScaling(scaleContainer_);
}