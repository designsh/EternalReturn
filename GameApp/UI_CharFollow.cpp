#include "Precompile.h"
#include "UI_CharFollow.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include <GameEngine/GameEngineProgressBarRenderer.h>
#include "LumiaLevel.h"
UI_CharFollow::UI_CharFollow()
	: UIOn(true), MyChar(JobType::NONE)
{
}

UI_CharFollow::~UI_CharFollow()
{
}

void UI_CharFollow::Start()
{
	//HpBarPos = {0.0f, 0.0f};
	HpBarSize = { 70.0f, 9.0f };

	//SpBarPos = {0.0f,-7.0f};
	SpBarSize = { 70.f, 4.f };

	//EmptyBarPos = {0.0f, -2.0f };
	EmptyBarSize = {72.f, 14.f};

	PassiveCountSize = {};

	{
		HPBar_Renderer = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		HPBar_Renderer->SetImage("HPBar_UI.png", "PointSmp");
		HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
		HPBar_Renderer->GetTransform()->SetLocalScaling(HpBarSize);
		HPBar_Renderer->SetProgressBarDirect(static_cast<int>(ProgressBarDirect::RightToLeft));
		HPBar_Renderer->SetPercent(0.5f);

		SPBar_Renderer = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		SPBar_Renderer->SetImage("SPBar_UI.png", "PointSmp");
		SPBar_Renderer->GetTransform()->SetLocalPosition(SpBarPos);
		SPBar_Renderer->GetTransform()->SetLocalScaling(SpBarSize);
		SPBar_Renderer->SetProgressBarDirect(static_cast<int>(ProgressBarDirect::RightToLeft));

		EmptyBar_Renderer = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		EmptyBar_Renderer->SetImage("EmptyBar_UI.png", "PointSmp");
		EmptyBar_Renderer->GetTransform()->SetLocalScaling(EmptyBarSize);
		EmptyBar_Renderer->GetTransform()->SetLocalPosition(EmptyBarPos);
		EmptyBar_Renderer->SetProgressBarDirect(static_cast<int>(ProgressBarDirect::RightToLeft));
	}

	{
		PassiveRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		PassiveRenderer->Off();
	//switch (MyChar)
	//{
	//case JobType::NONE:
	//	PassiveRenderer->Off();
	//	break;
	//case JobType::YUKI:
	//	PassiveRenderer->SetImage("Yuki_Gage_Zero", "PointSmp");
	//	PassiveRenderer->GetTransform()->SetLocalScaling(PassiveRenderer->GetCurrentTexture()->GetTextureSize());
	//	break;
	//case JobType::HYUNWOO:
	//	PassiveRenderer->Off();
	//	break;
	//case JobType::JACKIE:
	//	PassiveRenderer->Off();
	//	break;
	//case JobType::RIO:
	//	PassiveRenderer->Off();
	//	break;
	//case JobType::AYA:
	//	PassiveRenderer->Off();
	//	break;
	//case JobType::DUMMY:
	//	PassiveRenderer->Off();
	//	break;
	//default:
	//	break;
	//}
	
	}


}

void UI_CharFollow::Update(float _DeltaTime)
{
	{
		if (false == UIOn)
		{
			HPBar_Renderer->Off();
			SPBar_Renderer->Off();
			EmptyBar_Renderer->Off();
		}
		else
		{
			HPBar_Renderer->On();
			SPBar_Renderer->On();
			EmptyBar_Renderer->On();
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
}

void UI_CharFollow::SetFollowInfo(float4 _Pos, CharacterStat* _Stat) 
{
	if (_Stat->HP <= 0.0f)
	{
		this->Off();
	}

	float HPPercent = _Stat->HP / _Stat->HPMax;
	float SPPercent = _Stat->SP / _Stat->SPMax;

	HPBar_Renderer->SetPercent(HPPercent);
	SPBar_Renderer->SetPercent(SPPercent);

	HpBarPos = _Pos;
	//계산된 플레이어의 위치에 맞춰서 피봇을 맞춘다
	HpBarPos = HpBarPos + float4{0.f, 80.f, 0.f ,0.f};
	SpBarPos = HpBarPos + float4{ 0.0f, -7.f, 0.f,0.f };
	EmptyBarPos = HpBarPos + float4{ 0.f, -2.f, 0.f,0.f };
	

	HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
	SPBar_Renderer->GetTransform()->SetLocalPosition(SpBarPos);
	EmptyBar_Renderer->GetTransform()->SetLocalPosition(EmptyBarPos);
}

void UI_CharFollow::SetFollowInfoMonster(float4 _Pos, MonsterStateInfo _Stat)
{
	SPBar_Renderer->SetImage("ReturnBar_UI.png", "PointSmp");

	float HPPercent = _Stat.HP_ / _Stat.HPMax_;
	float SPPercent = _Stat.HomingInstinctValue_ / _Stat.HomingInstinctValueMax_;

	HPBar_Renderer->SetPercent(HPPercent);
	SPBar_Renderer->SetPercent(SPPercent);

	HpBarPos = _Pos;
	//계산된 플레이어의 위치에 맞춰서 피봇을 맞춘다
	HpBarPos = HpBarPos + float4{ 0.f, 60.f, 0.f ,0.f };
	SpBarPos = HpBarPos + float4{ 0.0f, -7.f, 0.f,0.f };
	EmptyBarPos = HpBarPos + float4{ 0.f, -2.f, 0.f,0.f };

	HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
	SPBar_Renderer->GetTransform()->SetLocalPosition(SpBarPos);
	EmptyBar_Renderer->GetTransform()->SetLocalPosition(EmptyBarPos);

	if (_Stat.HP_ <= 0.0f)
	{
		this->Off();
	}


	//if (_Stat.RegenTime_ > 0.0f)
	//{
	//	this->Off();
	//}
}

void UI_CharFollow::SetPassiveUI(int _num)
{
	switch (MyChar)
	{
	case JobType::NONE:
		break;
	case JobType::YUKI:
	{
		if (false == PassiveRenderer->IsUpdate())
		{
			PassiveRenderer->On();
			PassiveRenderer->SetImage("Yuki_Gage_Zero.png", "PointSmp");
			PassiveRenderer->GetTransform()->SetLocalScaling(PassiveRenderer->GetCurrentTexture()->GetTextureSize());
		}

		PassiveCountPos = HpBarPos + float4{ -5.f, -15.f, 0.f,0.f };

		if (0 == _num)
		{
			PassiveRenderer->SetImage("Yuki_Gage_Zero.png", "PointSmp");
		}
		else if (1 == _num)
		{
			PassiveRenderer->SetImage("Yuki_Gage_One.png", "PointSmp");
		}
		else if (2 == _num)
		{
			PassiveRenderer->SetImage("Yuki_Gage_Two.png", "PointSmp");
		}
		else if (3 == _num)
		{
			PassiveRenderer->SetImage("Yuki_Gage_Three.png", "PointSmp");
		}
		else if (4 == _num)
		{
			PassiveRenderer->SetImage("Yuki_Gage_Four.png", "PointSmp");
		}

		PassiveRenderer->GetTransform()->SetLocalPosition(PassiveCountPos);
	}
	break;
	case JobType::HYUNWOO:
	{
		return;
	}
	break;
	case JobType::JACKIE:
	{
		return;
	}
	break;
	case JobType::RIO:
		break;
	case JobType::AYA:
	{
		if (false == PassiveRenderer->IsUpdate())
		{
			PassiveRenderer->On();
			PassiveRenderer->SetImage("Aya_Gage_Zero.png", "PointSmp");
			PassiveRenderer->GetTransform()->SetLocalScaling(PassiveRenderer->GetCurrentTexture()->GetTextureSize());
		}

		PassiveCountPos = HpBarPos + float4{ 0.f, -15.f, 0.f,0.f };

		if (0 == _num)
		{
			PassiveRenderer->SetImage("Aya_Gage_Zero.png", "PointSmp");
		}
		else if (1 == _num)
		{
			PassiveRenderer->SetImage("Aya_Gage_One.png", "PointSmp");
		}
		else if (2 == _num)
		{
			PassiveRenderer->SetImage("Aya_Gage_Two.png", "PointSmp");
		}
		else if (3 == _num)
		{
			PassiveRenderer->SetImage("Aya_Gage_Three.png", "PointSmp");
		}
		else if (4 == _num)
		{
			PassiveRenderer->SetImage("Aya_Gage_Four.png", "PointSmp");
		}
		else if (5 == _num)
		{
			PassiveRenderer->SetImage("Aya_Gage_Five.png", "PointSmp");
		}
		else if (6 == _num)
		{
			PassiveRenderer->SetImage("Aya_Gage_Six.png", "PointSmp");
		}

		PassiveRenderer->GetTransform()->SetLocalPosition(PassiveCountPos);
	}
		break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}
}
