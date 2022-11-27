#include "PreCompile.h"
#include "UI_SkillGauge.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineProgressBarRenderer.h>
UI_SkillGauge::UI_SkillGauge() // default constructer 디폴트 생성자
	: standardTime_(0.0f), gaugeBgRenderer_(nullptr), gaugeRenderer_(nullptr), loadingPercent_(0.0f), gaugeTimer_(0.0f), fadeTimer_(0.0f)
{

}

UI_SkillGauge::~UI_SkillGauge() // default destructer 디폴트 소멸자
{

}

UI_SkillGauge::UI_SkillGauge(UI_SkillGauge&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void UI_SkillGauge::PlayAwake(float _time, const std::string& _text)
{
	standardTime_ = _time;
	
	// 텍스트 정의하는 자리
	text_ = _text;

	gaugeState_ << "Awake";
}

void UI_SkillGauge::PlayReverseAwake(float _time, const std::string& _text)
{
	standardTime_ = _time;

	gaugeState_ << "ReverseAwake";

	text_ = _text;

}

void UI_SkillGauge::PlayFadeForce()
{
	text_ = " ";
	gaugeState_ << "Fade";
}

void UI_SkillGauge::Start()
{
	gaugeBgRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UIPANEL0));
	gaugeBgRenderer_->SetImage("Img_AbilityInfo_GageBg_01.png", "LinerSmp");
	gaugeBgRenderer_->GetTransform()->SetLocalScaling({ 247.f, 11.f });
	gaugeBgRenderer_->GetTransform()->SetLocalPosition({0.0f, -200.0f});
	gaugeBgRenderer_->SetAlpha(1.0f);	
	gaugeBgRenderer_->Off();

	gaugeRenderer_ = CreateTransformComponent<GameEngineProgressBarRenderer>(static_cast<int>(UIRenderOrder::UI));
	gaugeRenderer_->SetImage("Img_AbilityInfo_GageBg_03.png", "LinerSmp");
	gaugeRenderer_->SetProgressBarDirect(static_cast<int>(ProgressBarDirect::RightToLeft));
	gaugeRenderer_->GetTransform()->SetLocalScaling(float4(247.f, 9.f));
	gaugeRenderer_->GetTransform()->SetLocalPosition({0.0f, -200.0f});
	gaugeBgRenderer_->TextSetting("굴림", " ", 12, FW1_CENTER, float4::WHITE, { 0.0f, 20.0f, 0.0f });
	gaugeRenderer_->SetPercent(loadingPercent_);
	gaugeRenderer_->Off();

	whiteMaskRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UICOL));
	whiteMaskRenderer_->SetImage("Img_AbilityInfo_GageBg_02.png", "LinerSmp");
	whiteMaskRenderer_->GetTransform()->SetLocalScaling({ 247.f, 11.f });
	whiteMaskRenderer_->GetTransform()->SetLocalPosition({ 0.0f, -200.0f });
	whiteMaskRenderer_->SetAlpha(0.0f);
	whiteMaskRenderer_->Off();

	gaugeState_.CreateState(MakeState(UI_SkillGauge, Sleep));
	gaugeState_.CreateState(MakeState(UI_SkillGauge, Awake));
	gaugeState_.CreateState(MakeState(UI_SkillGauge, ReverseAwake));
	gaugeState_.CreateState(MakeState(UI_SkillGauge, Fade));
	gaugeState_ << "Sleep";
}

void UI_SkillGauge::Update(float _DeltaTime)
{
	gaugeState_.Update(_DeltaTime);
}

void UI_SkillGauge::startSleep()
{
	standardTime_ = 0.0f;
	gaugeTimer_ = 0.0f;
	reverseGaugeTimer_ = 0.0f;
	gaugeBgRenderer_->Off();
	gaugeBgRenderer_->SetAlpha(1.0f);
	text_ = " ";
	gaugeBgRenderer_->SetPrintText(text_);
	gaugeRenderer_->SetPercent(0.0f);
	gaugeRenderer_->Off();
}

void UI_SkillGauge::updateSleep(float _deltaTime)
{
}

void UI_SkillGauge::startAwake()
{
	gaugeBgRenderer_->On();
	gaugeBgRenderer_->SetAlpha(1.0f);
	gaugeBgRenderer_->SetPrintText(text_);
	gaugeRenderer_->On();
}

void UI_SkillGauge::updateAwake(float _deltaTime)
{
	gaugeTimer_ += _deltaTime;

	gaugeRenderer_->SetPercent(gaugeTimer_ / standardTime_);

	if (gaugeTimer_ >= standardTime_)
	{
		gaugeTimer_ = standardTime_;

		gaugeTimer_ = 0.0f;
		gaugeRenderer_->SetPercent(1.0f);

		gaugeState_ << "Fade";
		return;
	}

}

void UI_SkillGauge::startReverseAwake()
{
	gaugeBgRenderer_->On();
	gaugeBgRenderer_->SetAlpha(1.0f);
	gaugeRenderer_->On();
	gaugeBgRenderer_->SetPrintText(text_);
	reverseGaugeTimer_ = standardTime_;
}

void UI_SkillGauge::updateReverseAwake(float _deltaTime)
{
	reverseGaugeTimer_ -= _deltaTime;

	gaugeRenderer_->SetPercent(reverseGaugeTimer_ / standardTime_);

	if (reverseGaugeTimer_ <= 0.0f)
	{
		reverseGaugeTimer_ = 0.0f;

		
		gaugeRenderer_->SetPercent(0.0f);

		gaugeState_ << "Fade";
		return;
	}

}

void UI_SkillGauge::startFade()
{
	fadeTimer_ = 0.0f;
	whiteMaskRenderer_->On();
}

void UI_SkillGauge::updateFade(float _deltaTime)
{
	fadeTimer_ += _deltaTime;

	whiteMaskRenderer_->SetAlpha(fadeTimer_/0.1f);

	if (fadeTimer_ >= 0.1f)
	{
		fadeTimer_ = 0.0f;
		whiteMaskRenderer_->Off();
		gaugeState_ << "Sleep";
		return;
	}
}