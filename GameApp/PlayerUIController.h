#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 로비레벨에 들어갈 UI액터를 모아두는 곳
// 설명 : 

#include "UI_Minimap.h"
#include "UI_CharPicture.h"
#include "UI_Equip.h"
#include "UI_Inventory.h"
#include "UI_Skill.h"
#include "UI_Status.h"
#include "UI_Time.h"
#include "UI_Notice.h"
#include "UI_HyperMap.h"
#include "UI_HPBars.h"
#include "UI_CalculateHelper.h"
#include "UI_CharFollow.h"
#include "UI_DamagePopUp.h"
#include "UI_ItemRoute.h"
#include "UI_SkillGauge.h"
#include "UI_FOW.h"
#include "UI_BloodBackground.h"

// 221018 박종원
// 죽거나 마지막 생존자일 때 표시되는 UI
#include "UI_WinLose.h"


class PlayerUIController : public GameEngineActor
{
public:
	PlayerUIController(); // default constructer 디폴트 생성자
	~PlayerUIController(); // default destructer 디폴트 소멸자
	PlayerUIController(const PlayerUIController& _other) = delete; // default Copy constructer 디폴트 복사생성자
	PlayerUIController(PlayerUIController&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	PlayerUIController& operator=(const PlayerUIController& _other) = delete; // default Copy operator 디폴트 대입 연산자
	PlayerUIController& operator=(const PlayerUIController&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void InitUI();
	//UI컨트롤러가 내 캐릭터가 뭔지 정보를 가지고있는게 좋을듯

	UI_CharPicture* GetCharPictureUI()
	{
		return charPicture_UI;
	}

	UI_Equip* GetEquipUI()
	{
		return equip_UI;
	}

	UI_Inventory* GetInventoryUI()
	{
		return inventory_UI;
	}

	UI_Skill* GetSkillUI()
	{
		return skill_UI;
	}

	UI_Status* GetStatusUI()
	{
		return status_UI;
	}

	UI_Notice* GetNoticeUI()
	{
		return notice_UI;
	}

	UI_WinLose* GetWinLoseUI()
	{
		return winLose_UI;
	}

	UI_BloodBackground* GetBloodBackground()
	{
		return blood_UI;
	}

	UI_FOW* GetFOW()
	{
		return FOW_UI;
	}

	UI_HPBars* GetHPBars()
	{
		return hpbars_UI;
	}
	
	UI_CharFollow* GetUICharFollow()
	{
		return charfollow_UI;
	}

	UI_Time* GetTimeUI()
	{
		return time_UI;
	}

	UI_Minimap* GetMinimapUI()
	{
		return minimap_UI;
	}

	UI_ItemRoute* GetItemRouteUI()
	{
		return itemroute_UI;
	}

	UI_SkillGauge* GetSkillGauge()
	{
		return skillGauge_UI;
	}
	//UI_HyperMap* GetHyperMap()
	//{
	//	return hypermap_UI;
	//}



	void SetJobType(JobType _MyJob);
	
	void UIOff();

public:


protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_CharPicture* charPicture_UI;
	UI_Equip* equip_UI;
	UI_Inventory* inventory_UI;
	UI_Skill* skill_UI;
	UI_Status* status_UI;
	UI_Time* time_UI;
	UI_Notice* notice_UI;
	UI_HPBars* hpbars_UI;
	UI_CharFollow* charfollow_UI;
	UI_SkillGauge* skillGauge_UI;
	UI_ItemRoute* itemroute_UI;
	UI_Minimap* minimap_UI;
	UI_CalculateHelper* calhelper_;

	UI_FOW* FOW_UI;

	UI_BloodBackground* blood_UI;
	UI_WinLose* winLose_UI;
	bool winLoseFlag_;

	JobType MyJob;

	vector<UI_CharFollow*> charfollows_;
	vector<UI_CharFollow*> Monstercharfollows_;
	bool followcreateflag;

};

