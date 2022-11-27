#include "PreCompile.h"
#include "UI_Notice.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameTimeController.h"



UI_Notice::UI_Notice()
	: FadeWaitTime(0.0f), FadeTime(0.0f), UIOn(false), Changing_Flag(false)
{
}

UI_Notice::~UI_Notice()
{
}


void UI_Notice::Start()
{

	UI_Pos = { 0.0f, 170.0f, 0.0f };
	Font_Pos = { 0.0f, 180.0f, -1.0f };
	BasicAlpha = 0.4f;

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UICOL);
		BackGroundRenderer->SetImage("UI_HalfAlphaRect.png", "PointSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(UI_Pos);
		BackGroundRenderer->GetTransform()->SetLocalScaling(float4{ 620.0f, 30.0f });
		BackGroundRenderer->SetAlpha(BasicAlpha);
	}

	{
		//폰트출력용
		FontRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		FontRenderer->GetTransform()->SetLocalPosition(Font_Pos);
		FontRenderer->TextSetting("굴림", "Test", 16, FW1_CENTER);
	}

	BackGroundRenderer->Off();
	FontRenderer->Off();
}

void UI_Notice::Update(float _Time)
{
	
	TimeCount(_Time);

	//UI 온오프 체크
	{
		if (false == UIOn)
		{
			BackGroundRenderer->Off();
			FontRenderer->Off();
			//BackGroundRenderer->On();
			//FontRenderer->On();
		}
		else
		{
			BackGroundRenderer->On();
			FontRenderer->On();
		}
	}

	//if (true == GameEngineInput::GetInst().Down("Esc"))
	//{
	//	if (UIOn == true)
	//	{
	//		UIOn = false;
	//	}
	//	else
	//	{
	//		UIOn = true;
	//	}
	//}


	if (Changing_Flag == false)
	{
		//플래그가 on이 아니면 실행되지 않습니다
		return;
	}


	if (FadeWaitTime <= 0.0f)
	{
		FontRenderer->Off();
	}

	if (FadeTime >= 0.0f)
	{
		//기본알파에 시간 비율을 곱해서 2초동안 투명해지게 설정
		BackGroundRenderer->SetAlpha((BasicAlpha)*(FadeTime));
		//FontRenderer->SetAlpha((BasicAlpha) * (FadeTime * 0.5f));
	}
	else
	{
		UISwitch();
		Changing_Flag = false;
	}

}

void UI_Notice::UISwitch()
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

void UI_Notice::SetText(string _Text, float _Time)
{
	if (Changing_Flag == false)
	{
		UISwitch();
	}

	FadeTime = 1.0f;
	FadeWaitTime = (_Time - FadeTime);


	FontRenderer->TextSetting("굴림", _Text, 18, FW1_CENTER);
	Changing_Flag = true;
}

void UI_Notice::TimeCount(float _Time)
{
	if (FadeWaitTime >= 0.0f)
	{
		FadeWaitTime -= _Time;
	}
	else if (FadeWaitTime <= 0.0f)
	{
		if (FadeTime >= 0.0f)
		{
			FadeTime -= _Time;
		}
		else
		{
			FadeWaitTime = 0.0f;
			FadeTime = 0.0f;
		}
	}

	
}
