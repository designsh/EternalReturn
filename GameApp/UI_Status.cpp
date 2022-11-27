#include "PreCompile.h"
#include "UI_Status.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

UI_Status::UI_Status()
	: Time(1.0f), UIOn(true)
{
}

UI_Status::~UI_Status()
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

void UI_Status::Start()
{
	//리소스들을 SetImage하고 위치를 조정합니다
	//위치를 조정한 리소스들은 UIRenderMap에 String과 같이 insert됩니다.
	//인게임정보(HP나 스태미너 상태, 착용장비여부)등을 받으면, UIRendererMap에서 이미지이름으로 Find해서 리소스를 바꿀 예정업니다.

	float4 MainStatus_BackgroundPos = { -410.0f, -319.0f, 0.0f };
	float4 SubStatus_BackgroundPos = { -525.0f, -319.0f, 0.0f };

	Value_XPivot = {55.0f, 0.0f, 0.0f, 0.0f};
	Value_YPivot = {0.0f, -19.0f, 0.0f, 0.0f};
	float4 SubStat_XPivot = { 50.0f, 0.0f, 0.0f, 0.0f };


	BasicValue_Pos = { -442.f, -283.f, -1.f };
	AttackSpeedValue_Pos = BasicValue_Pos + Value_YPivot;
	CriticalValue_Pos = BasicValue_Pos + (Value_YPivot * 2.0f);
	SkillDamageIncreaseValue_Pos = BasicValue_Pos + (Value_YPivot * 3.0f);

	DefenseValue_Pos = BasicValue_Pos + Value_XPivot;
	MoveSpeedValue_Pos = BasicValue_Pos + Value_YPivot + Value_XPivot;
	CooldownValue_Pos = BasicValue_Pos + (Value_YPivot * 2.0f) + Value_XPivot;
	SkillDamageReductionValue_Pos = BasicValue_Pos + (Value_YPivot * 3.0f) + Value_XPivot;


	SubStatBasicValue_Pos = { -550.f, -283.f, -1.f };
	BasicAttackDamageReduction_Pos = SubStatBasicValue_Pos + Value_YPivot;
	DefPenetration_Pos = SubStatBasicValue_Pos + (Value_YPivot * 2.0f);
	HPRegeneration_Pos = SubStatBasicValue_Pos + (Value_YPivot * 3.0f);


	AttackRange_Pos = SubStatBasicValue_Pos + SubStat_XPivot;
	Tenacity_Pos = SubStatBasicValue_Pos + SubStat_XPivot + Value_YPivot;
	VampValue_Pos = SubStatBasicValue_Pos + SubStat_XPivot + (Value_YPivot * 2.0f);
	SPRegeneration_Pos = SubStatBasicValue_Pos + SubStat_XPivot + (Value_YPivot * 3.0f);

	//렌더러 초기화 모음
	{
		MainStatusRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		MainStatusRenderer->SetImage("MainStatus_Background.png", "PointSmp");
		MainStatusRenderer->GetTransform()->SetLocalPosition(MainStatus_BackgroundPos);
		MainStatusRenderer->GetTransform()->SetLocalScaling(MainStatusRenderer->GetCurrentTexture()->GetTextureSize());

		SubStatusRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		SubStatusRenderer->SetImage("SubStatus_Background.png", "PointSmp");
		SubStatusRenderer->GetTransform()->SetLocalPosition(SubStatus_BackgroundPos);
		SubStatusRenderer->GetTransform()->SetLocalScaling(SubStatusRenderer->GetCurrentTexture()->GetTextureSize());


		//메인 스테이터스
		AttackValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		AttackValue_Renderer->GetTransform()->SetLocalPosition(BasicValue_Pos);

		DefenseValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		DefenseValue_Renderer->GetTransform()->SetLocalPosition(DefenseValue_Pos);

		AttackSpeedValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		AttackSpeedValue_Renderer->GetTransform()->SetLocalPosition(AttackSpeedValue_Pos);

		MoveSpeedValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		MoveSpeedValue_Renderer->GetTransform()->SetLocalPosition(MoveSpeedValue_Pos);

		CriticalValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		CriticalValue_Renderer->GetTransform()->SetLocalPosition(CriticalValue_Pos);

		CooldownValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		CooldownValue_Renderer->GetTransform()->SetLocalPosition(CooldownValue_Pos);

		SkillDamageIncreaseValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		SkillDamageIncreaseValue_Renderer->GetTransform()->SetLocalPosition(SkillDamageIncreaseValue_Pos);

		SkillDamageReductionValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		SkillDamageReductionValue_Renderer->GetTransform()->SetLocalPosition(SkillDamageReductionValue_Pos);
		

		//서브스테이터스
		VisionRange_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		VisionRange_Renderer->GetTransform()->SetLocalPosition(SubStatBasicValue_Pos);

		AttackRange_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		AttackRange_Renderer->GetTransform()->SetLocalPosition(AttackRange_Pos);

		BasicAttackDamageReduction_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		BasicAttackDamageReduction_Renderer->GetTransform()->SetLocalPosition(BasicAttackDamageReduction_Pos);

		Tenacity_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		Tenacity_Renderer->GetTransform()->SetLocalPosition(Tenacity_Pos);

		DefPenetration_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		DefPenetration_Renderer->GetTransform()->SetLocalPosition(DefPenetration_Pos);

		VampValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		VampValue_Renderer->GetTransform()->SetLocalPosition(VampValue_Pos);

		HPRegeneration_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		HPRegeneration_Renderer->GetTransform()->SetLocalPosition(HPRegeneration_Pos);

		SPRegeneration_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		SPRegeneration_Renderer->GetTransform()->SetLocalPosition(SPRegeneration_Pos);
	}

}

void UI_Status::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI 온오프 체크
	//나중엔 무조건 나오므로 삭제예정
	{
		if (false == UIOn)
		{
			MainStatusRenderer->Off();
			SubStatusRenderer->Off();
			AttackValue_Renderer->Off();
			DefenseValue_Renderer->Off();
			AttackSpeedValue_Renderer->Off();
			MoveSpeedValue_Renderer->Off();
			CriticalValue_Renderer->Off();
			CooldownValue_Renderer->Off();
			SkillDamageIncreaseValue_Renderer->Off();
			SkillDamageReductionValue_Renderer->Off();
			VisionRange_Renderer->Off();
			AttackRange_Renderer						 ->Off();
			BasicAttackDamageReduction_Renderer		 ->Off();
			Tenacity_Renderer							 ->Off();
			DefPenetration_Renderer					 ->Off();
			VampValue_Renderer							 ->Off();
			HPRegeneration_Renderer					 ->Off();
			SPRegeneration_Renderer					 ->Off();
		}
		else
		{
			MainStatusRenderer->On();
			SubStatusRenderer->On();
			AttackValue_Renderer->On();
			DefenseValue_Renderer->On();
			AttackSpeedValue_Renderer->On();
			MoveSpeedValue_Renderer->On();
			CriticalValue_Renderer->On();
			CooldownValue_Renderer->On();
			SkillDamageIncreaseValue_Renderer->On();
			SkillDamageReductionValue_Renderer->On();
			VisionRange_Renderer->On();
			AttackRange_Renderer->On();
			BasicAttackDamageReduction_Renderer->On();
			Tenacity_Renderer->On();
			DefPenetration_Renderer->On();
			VampValue_Renderer->On();
			HPRegeneration_Renderer->On();
			SPRegeneration_Renderer->On();
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

void UI_Status::SetStatus(CharacterStat* _Mystat)
{
	UIStat = _Mystat;

	//0.770000
	//to_string은 6자리까지 표현되므로 소수점 2자리까지만 표기되게 설정
	string AttackSpeed = to_string(UIStat->AttackSpeed);
	AttackSpeed.erase(4, 7);

	//크리티컬 확률, 쿨감 등은 뒤에 %가 붙이는 작업이 들어감
	float CriticalChance = UIStat->CriticalChance;
	string CriticalChance_String = to_string(CriticalChance);
	AttachPercent(CriticalChance, CriticalChance_String);

	float CooldownChance = UIStat->CooldownReduction;
	string CooldownChance_String = to_string(UIStat->CooldownReduction);
	AttachPercent(CooldownChance, CooldownChance_String);

	float SDIncrease = UIStat->SkillDamageAmplification;
	string SDIncrease_String = to_string(UIStat->SkillDamageAmplification);
	AttachPercent(SDIncrease, SDIncrease_String);

	float SDReduction = UIStat->SkillDamageAmplification;
	string SDReduction_String = to_string(UIStat->SkillAttackDamageReduction);
	AttachPercent(SDReduction, SDReduction_String);

	float ADReduction = UIStat->BasicAttackDamageReduction;
	string ADReduction_String = to_string(UIStat->BasicAttackDamageReduction);
	AttachPercent(ADReduction, ADReduction_String);

	float Tenacity = UIStat->Tenacity;
	string Tenacity_String = to_string(UIStat->Tenacity);
	AttachPercent(Tenacity, Tenacity_String);

	float LifeSteel = UIStat->LifeSteel;
	string LifeSteel_String = to_string(UIStat->LifeSteel);
	AttachPercent(LifeSteel, LifeSteel_String);

	//메인 스테이터스
	AttackValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->AttackPower)), 12);
	DefenseValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->Defence)), 12);
	AttackSpeedValue_Renderer->TextSetting("HMKMRHD", AttackSpeed, 12);
	MoveSpeedValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->MovementSpeed)), 12);
	CriticalValue_Renderer->TextSetting("HMKMRHD", CriticalChance_String, 12);
	CooldownValue_Renderer->TextSetting("HMKMRHD", CooldownChance_String, 12);
	SkillDamageIncreaseValue_Renderer->TextSetting("HMKMRHD", SDIncrease_String, 12);
	SkillDamageReductionValue_Renderer->TextSetting("HMKMRHD", SDReduction_String, 12);

	//서브 스테이터스
	VisionRange_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->VisionRange)), 12);
	AttackRange_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->AttackRange)), 12);
	BasicAttackDamageReduction_Renderer->TextSetting("HMKMRHD", ADReduction_String, 12);
	Tenacity_Renderer->TextSetting("HMKMRHD", Tenacity_String, 12);
	//현재 방관자체의 스탯이 없음
	DefPenetration_Renderer->TextSetting("HMKMRHD", "0%", 12);
	VampValue_Renderer->TextSetting("HMKMRHD", LifeSteel_String, 12);
	HPRegeneration_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->HPRegeneration)), 12);
	SPRegeneration_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->SPRegeneration)), 12);
}

void UI_Status::AttachPercent(float _Value, string& _Result)
{
	//퍼센트 계산을 위해서 100을 곱함
	float value = _Value * 100.0f;

	if (value >= 100.0f)
	{
		value = 100.0f;
	}

	_Result = to_string(static_cast<int>(value));

	_Result += "%";

	return;
}