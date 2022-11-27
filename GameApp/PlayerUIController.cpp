#include "PreCompile.h"
#include "PlayerUIController.h"
#include "PlayerInfoManager.h"
#include "LumiaLevel.h"
#include "GameTimeController.h"
#include "Monsters.h"
#include "MonsterInfoManager.h"
#include "Character.h"


PlayerUIController::PlayerUIController()
	: MyJob(JobType::HYUNWOO), winLoseFlag_(false), blood_UI(nullptr), followcreateflag(false)
{

}

PlayerUIController::~PlayerUIController() // default destructer ����Ʈ �Ҹ���
{

}

PlayerUIController::PlayerUIController(PlayerUIController&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void PlayerUIController::InitUI()
{
	FOW_UI = GetLevel()->CreateActor<UI_FOW>();
	blood_UI = GetLevel()->CreateActor<UI_BloodBackground>();
	charPicture_UI = GetLevel()->CreateActor<UI_CharPicture>();
	equip_UI = GetLevel()->CreateActor<UI_Equip>();
	inventory_UI = GetLevel()->CreateActor<UI_Inventory>();
	skill_UI = GetLevel()->CreateActor<UI_Skill>();
	status_UI = GetLevel()->CreateActor<UI_Status>();
	time_UI = GetLevel()->CreateActor<UI_Time>();
	notice_UI = GetLevel()->CreateActor<UI_Notice>();
	winLose_UI = GetLevel()->CreateActor<UI_WinLose>();
	hpbars_UI = GetLevel()->CreateActor<UI_HPBars>();
	//charfollow_UI = GetLevel()->CreateActor<UI_CharFollow>();
	minimap_UI = GetLevel()->CreateActor<UI_Minimap>();
	skillGauge_UI = GetLevel()->CreateActor<UI_SkillGauge>();
	calhelper_ = GetLevel()->CreateActor<UI_CalculateHelper>();
	itemroute_UI = GetLevel()->CreateActor<UI_ItemRoute>();


	//�׽�Ʈ��
	//notice_UI->SetText("�˸� UI �׽�Ʈ�� �ؽ�Ʈ�Դϴ�", 5.f);
}


void PlayerUIController::SetJobType(JobType _MyJob)
{
	MyJob = _MyJob;

	charPicture_UI->SetJobType(MyJob);
	skill_UI->SetJobType(MyJob);
	itemroute_UI->SetJobType(MyJob);
}

void PlayerUIController::UIOff()
{
	hpbars_UI->Off();
	charPicture_UI->Off();
	equip_UI ->Off();
	inventory_UI->Off();
	skill_UI->Off();
	status_UI->Off();
	time_UI->Off();
	hpbars_UI->Off();
}


void PlayerUIController::Start()
{
	InitUI();

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	skill_UI->SetStatus(pm->GetMyPlayer().stat_);
}

void PlayerUIController::Update(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	// ���� ������ ��̾� ������ �ƴ� ��� ó������ ����
	LumiaLevel* lumiaLevel = GetLevelConvert<LumiaLevel>();
	if (nullptr == lumiaLevel)
	{
		return;
	}

	if (pm == nullptr)
	{
		return;
	}


	Character* myChar = lumiaLevel->GetCharacterActorList()[pm->GetMyNumber()];
	status_UI->SetStatus(myChar->GetStat());
	hpbars_UI->SetStatus(myChar->GetStat());
	DayAndNightType SunMoon = GameTimeController::GetInstance()->GetCurrentDayType();
	float DayVision = (FT::Char::DEFAULT_VISION_RANGE_DAY / 2.f) + 50.f;
	float NightVision = (FT::Char::DEFAULT_VISION_RANGE_NIGHT / 2.f) + 50.f;
	
	if (false == followcreateflag)
	{
		
		//�÷��̾�� HP��
		for (size_t i = 0; i < lumiaLevel->GetCharacterActorList().size(); i++)
		{
			UI_CharFollow* follow_UI = GetLevel()->CreateActor<UI_CharFollow>();
			charfollows_.push_back(follow_UI);
		}

		//���͵� HP��
		for (size_t i = 0; i < lumiaLevel->GetMonsterActorList().size(); i++)
		{
			UI_CharFollow* follow_UI = GetLevel()->CreateActor<UI_CharFollow>();
			Monstercharfollows_.push_back(follow_UI);
		}

		//�� �ѹ��� ����ǰ� ����
		followcreateflag = true;
	}
	
	for (size_t i = 0; i < lumiaLevel->GetCharacterActorList().size(); i++)
	{
		float4 pos;

		if (i == pm->GetMyNumber() && false == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
		{
			pos = { 0.0f, 0.0f };
		}
		else
		{
			pos = lumiaLevel->GetCharacterActorList()[i]->GetTransform()->GetLocalPosition();
			pos = calhelper_->Cal3Dto2D(pos);
			pos = calhelper_->CalRound(pos);
		}

		bool hidden = lumiaLevel->GetCharacterActorList()[i]->IsHidden();

		if (true == hidden)
		{
			charfollows_[i]->Off();
		}
		else
		{
			charfollows_[i]->On();
			charfollows_[i]->SetChar(lumiaLevel->GetCharacterActorList()[i]->GetJobType());
			charfollows_[i]->SetFollowInfo(pos, lumiaLevel->GetCharacterActorList()[i]->GetStat());
			charfollows_[i]->SetPassiveUI(lumiaLevel->GetCharacterActorList()[i]->GetStat()->passive_Count);
		}

	}

	for (size_t i = 0; i < lumiaLevel->GetMonsterActorList().size(); i++)
	{
		float4 pos = lumiaLevel->GetMonsterActorList()[i]->GetTransform()->GetLocalPosition();
		pos = calhelper_->Cal3Dto2D(pos);
	
		bool hidden = lumiaLevel->GetMonsterActorList()[i]->IsHidden();

		if (true == hidden)
		{
			Monstercharfollows_[i]->Off();
		}
		else
		{
			Monstercharfollows_[i]->On();
			Monstercharfollows_[i]->SetFollowInfoMonster(pos, lumiaLevel->GetMonsterActorList()[i]->GetMonsterStateInfo());
		}
	
		if (false == lumiaLevel->GetMonsterActorList()[i]->IsUpdate())
		{
			//���� �ȳ��� ���͵��� ü�¹� �Ⱥ���
			Monstercharfollows_[i]->Off();
		}

	}

	if (itemroute_UI->isOn())
	{
		if (true == GameEngineInput::GetInst().Down("LButton"))
		{
			itemroute_UI->SlotMouseCollisionCheck();
		}
	}

	//if (true == GameEngineInput::GetInst().Down("L"))
	//{
	//	notice_UI->SetText("�˸� UI �׽�Ʈ�� �ؽ�Ʈ�Դϴ�", 2.f);
	//}
	
}

