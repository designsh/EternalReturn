#include "PreCompile.h"
#include "UI_Time.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameTimeController.h"

UI_Time::UI_Time()
	: Time(1.0f), UIOn(true), ProhibitedTimeRenderer_(nullptr)
{
}

UI_Time::~UI_Time()
{
	//for (auto& UIRenderer : UIRendererMap)
	//{
	//	if (true)
	//	{
	//		delete UIRenderer.second;
	//		UIRenderer.second = nullptr;
	//	}
	//}
}


void UI_Time::Start()
{

	UI_Pos = { 0.0f, 326.0f, 0.0f };
	Day_Pos = { -35.0f, 330.0f, 0.0f };
	Time_Pos = { -20.0f, 344.0f, 0.0f };
	Clock_Pos = { -40.f, 303.f, 0.0f };
	Prohibit_Pos = { 10.f, 303.f, 0.0f };
	ClockTime_Pos = { -10.f, 318.f, 0.0f };
	DayCount_Pos = { -20.f, 361.f, 0.0f };

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		BackGroundRenderer->SetImage("UI_TimeBg_Test.png", "LinerSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(UI_Pos);
		//BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize() * 0.8f);
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());

		DayNightRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		DayNightRenderer->SetImage("UI_Sun.png", "LinerSmp");
		DayNightRenderer->GetTransform()->SetLocalPosition(Day_Pos);
		DayNightRenderer->GetTransform()->SetLocalScaling(DayNightRenderer->GetCurrentTexture()->GetTextureSize());

		TimeRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		TimeRenderer->GetTransform()->SetLocalPosition(Time_Pos);
	}

	{
		NightChangeClockRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		NightChangeClockRenderer->SetImage("UI_Clock.png", "LinerSmp");
		NightChangeClockRenderer->GetTransform()->SetLocalPosition(Clock_Pos);
		NightChangeClockRenderer->GetTransform()->SetLocalScaling({ 21.f,21.f });

		NightChangeTimeRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		NightChangeTimeRenderer->GetTransform()->SetLocalPosition(ClockTime_Pos);
	}

	{
		DayCountRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		DayCountRenderer->GetTransform()->SetLocalPosition(DayCount_Pos);
	}

	{
		ProhibitedTimeRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::FONT));
		ProhibitedTimeRenderer_->SetImage("UI_Prohibited.png", "LinerSmp");
		ProhibitedTimeRenderer_->GetTransform()->SetLocalScaling({ 21.0f, 21.0f });
		ProhibitedTimeRenderer_->GetTransform()->SetLocalPosition(Prohibit_Pos);
		ProhibitedTimeRenderer_->TextSetting("HMKMRHD", "20", 20, FW1_RIGHT, float4::WHITE, { 34.0f, 15.0f, 0.0f });
	}
}

void UI_Time::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}

	//UI 온오프 체크
	{
		if (false == UIOn)
		{
			BackGroundRenderer->Off();
			DayNightRenderer->Off();
			TimeRenderer->Off();
			NightChangeClockRenderer->Off();
			NightChangeTimeRenderer->Off();
			DayCountRenderer->Off();
		}
		else
		{
			BackGroundRenderer->On();
			DayNightRenderer->On();
			TimeRenderer->On();
			NightChangeClockRenderer->On();
			NightChangeTimeRenderer->On();
			DayCountRenderer->On();
		}
	}

	if (true == GameEngineInput::GetInst().Down("Esc"))
	{
		if (UIOn == true)
		{
			UIOn = false;
		}
		else
		{
			UIOn = true;
		}
	}

	DayAndNightType SunMoon = GameTimeController::GetInstance()->GetCurrentDayType();

	switch (SunMoon)
	{
	case DayAndNightType::NONE:
		DayNightRenderer->SetImage("UI_Sun.png", "PointSmp");
		break;
	case DayAndNightType::DAY:
		DayNightRenderer->SetImage("UI_Sun.png", "PointSmp");
		break;
	case DayAndNightType::NIGHT:
		DayNightRenderer->SetImage("UI_Moon.png", "PointSmp");
		break;
	case DayAndNightType::MAX:
		DayNightRenderer->SetImage("UI_Sun.png", "PointSmp");
		break;
	default:
		break;
	}

	TimeSetting();

}

void UI_Time::TimeSetting()
{
	{
		tm InGameTime = GameTimeController::GetInstance()->GetTotalGameTimeToHour();
		int Minute = InGameTime.tm_min;
		int Second = InGameTime.tm_sec;
		string Time = to_string(Minute) + ":" + to_string(Second);
		TimeRenderer->TextSetting("HMKMRHD", Time, 20);
	}

	{
		float NightChangeTime = GameTimeController::GetInstance()->GetRemainTimeToSec();
		tm ClockTime = GameTimeController::GetInstance()->GetRemainTimeToMinute();
		int Minute = ClockTime.tm_min;
		int Second = ClockTime.tm_sec;
	//	string Time = to_string(Minute) + ":" + to_string(Second);
		string Time = to_string(static_cast<int>(NightChangeTime));
		NightChangeTimeRenderer->TextSetting("HMKMRHD", Time, 20, FW1_RIGHT, float4::WHITE, {8.0f ,0.0f, 0.0f});
	}

	{
		int DayCount = GameTimeController::GetInstance()->GetCurrentDay();
		string DayCountString = to_string(DayCount) + "일 차";
		DayCountRenderer->TextSetting("HMKMRHD", DayCountString, 15);
	}
}

