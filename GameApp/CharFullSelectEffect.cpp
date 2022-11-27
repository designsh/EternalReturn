#include "PreCompile.h"
#include "CharFullSelectEffect.h"

CharFullSelectEffect::CharFullSelectEffect() // default constructer 디폴트 생성자
	: timer_(0.5f), charStencilRenderer_(nullptr), scaleContainer_({ 0.0f, 0.0f })
{

}

CharFullSelectEffect::~CharFullSelectEffect() // default destructer 디폴트 소멸자
{

}

CharFullSelectEffect::CharFullSelectEffect(CharFullSelectEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void CharFullSelectEffect::PlayAwake()
{
	renderState_ << "Awake";
}

void CharFullSelectEffect::Start()
{
	charStencilRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UIPANEL2));
	charStencilRenderer_->SetImage("Lobby_CharFull_Hyunwoo.png", "PointSmp");
	charStencilRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 125.0f});
	charStencilRenderer_->GetTransform()->SetLocalScaling(charStencilRenderer_->GetCurrentTexture()->GetTextureSize());
	charStencilRenderer_->Off();

	renderState_.CreateState(MakeState(CharFullSelectEffect, Sleep));
	renderState_.CreateState(MakeState(CharFullSelectEffect, Awake));
	renderState_ << "Sleep";
}

void CharFullSelectEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void CharFullSelectEffect::startSleep()
{
	timer_ = 0.5f;
	scaleContainer_ = { 0.0f, 0.0f };
	charStencilRenderer_->Off();
}

void CharFullSelectEffect::updateSleep(float _deltaTime)
{
}

void CharFullSelectEffect::startAwake()
{
	timer_ = 0.5f;
	scaleContainer_ = charStencilRenderer_->GetCurrentTexture()->GetTextureSize();
	charStencilRenderer_->On();
}

void CharFullSelectEffect::updateAwake(float _deltaTime)
{
	charStencilRenderer_->SetAlpha(timer_/0.5f);

	scaleContainer_ += float4{ 300.0f * _deltaTime, 300.0f * _deltaTime };

	charStencilRenderer_->GetTransform()->SetLocalScaling(scaleContainer_);

	timer_ -= _deltaTime;

	if (0.0f >= timer_)
	{
		renderState_ << "Sleep";
		return;
	}
}

void CharFullSelectEffect::SetCharImage(JobType _type)
{
	switch (_type)
	{
	case JobType::NONE:
		break;
	case JobType::YUKI:
		charStencilRenderer_->SetImage("Lobby_CharFull_Yuki.png", "PointSmp");
		break;
	//case JobType::FIORA:
	//	break;
	//case JobType::ZAHIR:
	//	break;
	//case JobType::NADINE:
	//	break;
	case JobType::HYUNWOO:
		charStencilRenderer_->SetImage("Lobby_CharFull_Hyunwoo.png", "PointSmp");
		break;
	case JobType::JACKIE:
		charStencilRenderer_->SetImage("Lobby_CharFull_Jackie.png", "PointSmp");
		break;
	case JobType::RIO:
		charStencilRenderer_->SetImage("Lobby_CharFull_Rio.png", "PointSmp");
		break;
	case JobType::AYA:
		charStencilRenderer_->SetImage("Lobby_CharFull_Aya.png", "PointSmp");
		break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}



}
