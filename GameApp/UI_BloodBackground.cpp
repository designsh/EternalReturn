#include "PreCompile.h"
#include "UI_BloodBackground.h"
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineFSM.h>
UI_BloodBackground::UI_BloodBackground() // default constructer 디폴트 생성자
	:bloodRenderer(nullptr), timer_(0.0f), playerDead_(false)
{

}

UI_BloodBackground::~UI_BloodBackground() // default destructer 디폴트 소멸자
{

}

UI_BloodBackground::UI_BloodBackground(UI_BloodBackground&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void UI_BloodBackground::Draw(bool _flag)
{
	if (true == _flag)
	{
		bloodRenderer->On();
	}
	else if (false == _flag)
	{
		bloodRenderer->Off();
	}
}

void UI_BloodBackground::Start()
{


	bloodRenderer = CreateTransformComponent<GameEngineUIRenderer>();
	bloodRenderer->SetImage("Fx_Blood_Background_01.png", "PointSmp");
	bloodRenderer->GetTransform()->SetLocalScaling(bloodRenderer->GetCurrentTexture()->GetTextureSize());
	bloodRenderer->Off();

	state_.CreateState(MakeState(UI_BloodBackground, GoRed));
	state_.CreateState(MakeState(UI_BloodBackground, GoBlack));

	state_.ChangeState("GoRed", true);
}

void UI_BloodBackground::Update(float _Time)
{
	state_.Update(_Time);
}

void UI_BloodBackground::startGoRed()
{
	timer_ = 0.0f;
	bloodRenderer->SetAlpha(0.0f);
}

void UI_BloodBackground::updateGoRed(float _Time)
{
	timer_ += _Time;

	if (1.0f <= timer_)
	{
		state_.ChangeState("GoBlack", true);
		return;
	}

	bloodRenderer->SetAlpha(timer_);
}

void UI_BloodBackground::startGoBlack()
{
	timer_ = 1.0f;
	bloodRenderer->SetAlpha(1.0f);
}

void UI_BloodBackground::updateGoBlack(float _Time)
{
	if (true == playerDead_)
	{
		return;
	}

	timer_ -= _Time;

	if (0.0f >= timer_)
	{
		state_.ChangeState("GoRed", true);
		return;
	}

	bloodRenderer->SetAlpha(timer_);
}

