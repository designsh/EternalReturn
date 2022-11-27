#include "Precompile.h"
#include "UI_DamagePopUp.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include <GameEngine/GameEngineProgressBarRenderer.h>
#include "LumiaLevel.h"

UI_DamagePopUp::UI_DamagePopUp()
	: UIOn(true), MyChar(JobType::NONE), Time_(0.5f)
{
}

UI_DamagePopUp::~UI_DamagePopUp()
{
}

void UI_DamagePopUp::DamageFontAppear(float4 _Pos, string _DamageValue)
{
	float4 Pos = Cal3Dto2D(_Pos);
	Pos = Pos + float4{0.f, 60.f, 0.f, 0.f};
	HPBar_Renderer->GetTransform()->SetLocalPosition(Pos);
	HPBar_Renderer->TextSetting("HMKMRHD", _DamageValue, 21, FW1_CENTER, float4::RED);
	Time_ = 0.5f;
}

void UI_DamagePopUp::DamageFontAppearMonster(float4 _Pos, string _DamageValue)
{
}

void UI_DamagePopUp::Start()
{
	//HpBarPos = {0.0f, 0.0f};
	HpBarSize = { 70.0f, 9.0f };


	{
		HPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);

		//HPBar_Renderer->TextSetting("±¼¸²", "", 11, FW1_CENTER, float4::RED);
	}

}

void UI_DamagePopUp::Update(float _DeltaTime)
{
	{
		if (false == UIOn)
		{
			HPBar_Renderer->Off();
		}
		else
		{
			HPBar_Renderer->On();
		}
	}
	
	Time_ -= _DeltaTime;

	

	if (Time_ <= 0.0f)
	{
		HPBar_Renderer->Off();
		return;
		//HPBar_Renderer->TextSetting("±¼¸²", "", 13, FW1_CENTER, float4::RED);
	}

	HPBar_Renderer->GetTransform()->SetLocalDeltaTimeMove(float4{ 0.f,30.f });
}
