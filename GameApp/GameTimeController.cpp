#include "Precompile.h"
#include "GameTimeController.h"

#include <GameEngine/GameEngineCore.h>

#include "GameServer.h"
#include "MonsterStateChangePacket.h"

#include "PlayerInfoManager.h"
#include "LumiaLevel.h"
#include "PlayerUIController.h"
#include "LevelUpSystem.h"
#include "ProhibitedArea.h"
#include "NoticeBroadcastPacket.h"
#include "ProhibitedAreaPacket.h"

GameTimeController* GameTimeController::GetInstance()
{
	static GameTimeController instance;
	return &instance;
}

tm GameTimeController::GetTotalGameTimeToMinute()
{
	// 소수점자리빼버리고 분/초로 반환
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(CurGameTime_);
	CurMin = CurTimeToInt / 60;							// 초/60 = 분
	CurSec = CurTimeToInt % 60;							// 초%60 = 초

	tm ReturnGameTime = {};
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

tm GameTimeController::GetTotalGameTimeToHour()
{
	// 소수점자리빼버리고 시/분/초로 반환
	int CurHour = 0;
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(CurGameTime_);
	CurMin = CurTimeToInt / 60;							// 초/60 = 분
	CurHour = CurMin / 60;								// 분/60 = 시
	CurSec = CurTimeToInt % 60;							// 시/분/초이므로 초%60 = 초
	CurMin = CurMin % 60;								// 시/분/초이므로 분%60 = 분

	tm ReturnGameTime = {};
	ReturnGameTime.tm_hour = CurHour;
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

float GameTimeController::GetTotalGameTimeToSec()
{
	return CurGameTime_;
}

tm GameTimeController::GetRemainTimeToMinute()
{
	// 소수점자리빼버리고 분/초로 반환
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(DayAndNightTime_);
	CurMin = CurTimeToInt / 60;							// 초/60 = 분
	CurSec = CurTimeToInt % 60;							// 초%60 = 초

	tm ReturnGameTime = {};
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

tm GameTimeController::GetRemainTimeToHour()
{
	// 소수점자리빼버리고 시/분/초로 반환
	int CurHour = 0;
	int CurMin = 0;
	int CurSec = 0;
	int CurTimeToInt = static_cast<int>(DayAndNightTime_);
	CurMin = CurTimeToInt / 60;							// 초/60 = 분
	CurHour = CurMin / 60;								// 분/60 = 시
	CurSec = CurTimeToInt % 60;							// 시/분/초이므로 초%60 = 초
	CurMin = CurMin % 60;								// 시/분/초이므로 분%60 = 분

	tm ReturnGameTime = {};
	ReturnGameTime.tm_hour = CurHour;
	ReturnGameTime.tm_min = CurMin;
	ReturnGameTime.tm_sec = CurSec;

	return ReturnGameTime;
}

float GameTimeController::GetRemainTimeToSec()
{
	return DayAndNightTime_;
}

void GameTimeController::HostInitialize()
{
	// 일차별 낮/밤 시간 초기화
	CreateDailytimes();

	// 레벨업시스템 관련


	// 몬스터타입별 첫등장관련정보 초기화
	CreateMonsterFirstAppearInfo();
}

void GameTimeController::HostUpdate(float _DeltaTime)
{
	// 레벨업관련 갱신


	// 각 몬스터타입별 첫등장시간 체크 및 이벤트 등 처리
	UpdateMonsterFirstAppearTime(_DeltaTime);

	// 게임진행에 따른 시간누적 및 낮/밤전환, 일차증가
	CalcGameTime(_DeltaTime);
}

void GameTimeController::GuestUpdate(float _GameTime, DayAndNightType _Type, int _CurDay, float _DayAndNightTime)
{
	PrevDay_ = CurDay_;
	CurDay_ = _CurDay;
	CurDayOrNight_ = _Type;
	CurGameTime_ = _GameTime;
	DayAndNightTime_ = _DayAndNightTime;

	// 일차가 달라졌다면 레벨업 명령(게스트전용)
	if (PrevDay_ != CurDay_)
	{
		LevelUpSystem::GetInstance()->AllUnitLevelUP();
	}
}

void GameTimeController::DayAndNightForecedConversion()
{
	// 강제 낮/밤 시간 초기화하여 전환
	if (true == GameServer::GetInstance()->IsOpened())
	{
		DayAndNightTime_ = 1.0f;
	}
}

void GameTimeController::CreateDailytimes()
{
	// 4일 낮/밤시간 정의
	// -> 4일마다 반복
	DailyTime OneDay = {};
	//OneDay.DayTime_ = 170.0f;
	//OneDay.NightTime_ = 140.0f;

	OneDay.DayTime_ = 180.0f;
	OneDay.NightTime_ = 180.0f;

	DailyTime TwoDay = {};
	//TwoDay.DayTime_ = 140.0f;
	//TwoDay.NightTime_ = 140.0f;

	TwoDay.DayTime_ = 180.0f;
	TwoDay.NightTime_ = 180.0f;

	DailyTime ThreeDay = {};
	//ThreeDay.DayTime_ = 125.0f;
	//ThreeDay.NightTime_ = 125.0f;

	ThreeDay.DayTime_ = 180.0f;
	ThreeDay.NightTime_ = 180.0f;

	DailyTime FourDay = {};
	FourDay.DayTime_ = 600.0f;
	FourDay.NightTime_ = 600.0f;

	Dailytimes_.push_back(OneDay);
	Dailytimes_.push_back(TwoDay);
	Dailytimes_.push_back(ThreeDay);
	Dailytimes_.push_back(FourDay);

	// 0일차 낮부터 시작이므로 낮/밤 전환시간 Dailytimes_[0]의 낮시간으로 설정
	DayAndNightTime_ = Dailytimes_[0].DayTime_;
}

void GameTimeController::CalcGameTime(float _DeltaTime)
{

	// DeltaTime 누적(총 게임진행시간)
	CurGameTime_ += _DeltaTime;

	// 현재일차에 따라 낮과밤전환 및 일차증가
	DayAndNightTime_ -= _DeltaTime;							// 낮->밤 or 밤->낮 전환시간 감소


	if (false == isFirstAnnounced_)
	{
		FT::PlaySoundAllAndSendPacket("Announce_Start_Game_02.wav");

		GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
		LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);

		PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
		PlayLevel->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetNoticeUI()->SetText("실험이 시작되었습니다. 레이더를 활성화합니다.", 3.f);

		NoticeBroadcastPacket packet;
		packet.SetString("실험이 시작되었습니다. 레이더를 활성화합니다.");
		packet.SetTimer(3.0f);
		if (true == GameServer::GetInstance()->IsOpened())
		{
			GameServer::GetInstance()->Send(&packet);
		}

		isFirstAnnounced_ = true;
	}


	if (Dailytimes_[CurDay_ % 4].DayTime_ * 0.5f >= DayAndNightTime_ && false == isReserved_)	// 낮밤 시간이 절반이 되었을 때 (낮 60초면 30초가 되었을 때)
	{
		MakeNextProhibitedArea();
		isReserved_ = true;
	}


	if (0.0f >= DayAndNightTime_)							// 낮/밤 전환시간 도달시
	{
		int DayAndNightIndex = CurDay_ % 4;
	
		if (DayAndNightType::DAY == CurDayOrNight_)			// 현재 낮일때
		{
			if (true == isReserved_)
			{
				SetProhibitedArea();
				isReserved_ = false;
			}

			// 전환되었으므로 타입을 밤으로 설정
			CurDayOrNight_ = DayAndNightType::NIGHT;

			//밤 FOW 설정
			GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
			if (nullptr == CurLevel)
			{
				GameEngineDebug::MsgBoxError("현재 게임의 현재레벨을 찾을수 없습니다!!!!");
				return;
			}
			LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);
			PlayLevel->GetCharacterActorList()[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetUIController()->GetFOW()->SetFOW(true);

			// 현재 밤일때 전환시간을 현재전환시간으로 설정
			DayAndNightTime_ = Dailytimes_[DayAndNightIndex].NightTime_;

		}
		else if (DayAndNightType::NIGHT == CurDayOrNight_)	// 현재 밤일때
		{
			if (true == isReserved_)
			{
				SetProhibitedArea();
				isReserved_ = false;
			}


			// 전환되었으므로 타입을 낮으로 설정
			CurDayOrNight_ = DayAndNightType::DAY;

			// 현재 낮일때 전환시간을 현재전환시간으로 설정
			DayAndNightTime_ = Dailytimes_[DayAndNightIndex].DayTime_;

			//낮 FOW 설정
			GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
			if (nullptr == CurLevel)
			{
				GameEngineDebug::MsgBoxError("현재 게임의 현재레벨을 찾을수 없습니다!!!!");
				return;
			}
			LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);
			PlayLevel->GetCharacterActorList()[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetUIController()->GetFOW()->SetFOW(false);

			// 밤->낮으로 전환되었으므로 현재일차를 이전일차에 저장후 증가
			PrevDay_ = CurDay_;
			CurDay_ += 1;

			// 일차 증가로 현재 게임에 배치된 모든 유닛에게 레벨업명령(서버전용)
			LevelUpSystem::GetInstance()->AllUnitLevelUP();

		
		}
	}
}

void GameTimeController::CreateMonsterFirstAppearInfo()
{
	// !!! 이터널리턴의 시간은 초단위 기준 !!!
	// Ex) 00:40 -> 40초, 02:20 -> 2분 20초(140초)

	// WOLF(늑대): 1일차 낮 00:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearTime_ = 40.0f;

	// BEAR(곰): 1일차 밤 02:20
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ = false;
	//MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_ = 140.0f;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_ = 40.0f;						// 221107 SJHD EDIT : 낮/밤 시간 제한 변경으로 인한 수정

	// BAT(박쥐): 1일차 낮 01:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ = false;
	//MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_ = 100.0f;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_ = 40.0f;						// 221107 SJHD EDIT : 낮/밤 시간 제한 변경으로 인한 수정

	// DOG(들개): 1일차 낮 00:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearTime_ = 40.0f;

	// CHICKEN(닭): 게임시작과동시에 등장
	MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ = false;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].AppearTime_ = 1.0f;

	// BOAR(멧돼지): 1일차 낮 01:40
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ = false;
	//MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_ = 100.0f;
	MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_ = 40.0f;						// 221107 SJHD EDIT : 낮/밤 시간 제한 변경으로 인한 수정
}

void GameTimeController::UpdateMonsterFirstAppearTime(float _DeltaTime)
{
	// 각 몬스터 타입별 첫등장조건, 첫등장시간에 따른 첫등장상태전환 처리
	if (false == AllMonsterAppearEnd_)
	{
		// 0일차 게임시작일때
		// CHICKEN(닭): 게임시작과동시에 등장(서버-클라 동기화 처리로인해 1.0초의 딜레이를 부여)
		if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_)
		{
			MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].AppearTime_ -= _DeltaTime;
			if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].AppearTime_)
			{
				// 게임시작과 현재맵에 배치된 닭(몬스터)에게 등장모션을 실행하라는 명령을 전달
				FirstAppearMonsters(MonsterType::CHICKEN);

				// 등장완료로 인한 Flag On
				MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ = true;
			}
		}

		// 1일차 낮일때
		if (1 == CurDay_ && DayAndNightType::DAY == CurDayOrNight_)
		{
			// WOLF(늑대): 1일차 낮 00:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::WOLF);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ = true;
				}
			}

			// BAT(박쥐): 1일차 낮 01:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::BAT);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ = true;
				}
			}

			// DOG(들개): 1일차 낮 00:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::DOG);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ = true;
				}
			}

			// BOAR(멧돼지): 1일차 낮 01:40
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::BOAR);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ = true;
				}
			}
		}

		// 1일차 밤일때
		if (1 == CurDay_ && DayAndNightType::NIGHT == CurDayOrNight_)
		{
			// BEAR(곰): 1일차 밤 02:20
			if (false == MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_)
			{
				MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_ -= _DeltaTime;
				if (0.0f >= MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].AppearTime_)
				{
					// 등장모션실행 명령 전달
					FirstAppearMonsters(MonsterType::BEAR);

					// 등장완료로 인한 Flag On
					MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ = true;
				}
			}
		}

#pragma region 테스트용
		//TestTime_ -= _DeltaTime;
		//if (0.0f >= TestTime_)
		//{
		//	if (MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ == false)
		//	{
		//		FirstAppearMonsters(MonsterType::WOLF);
		//		FirstAppearMonsters(MonsterType::BEAR);
		//		FirstAppearMonsters(MonsterType::BAT);
		//		FirstAppearMonsters(MonsterType::DOG);
		//		FirstAppearMonsters(MonsterType::CHICKEN);
		//		FirstAppearMonsters(MonsterType::BOAR);
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ = true;
		//		MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ = true;
		//	}
		//}
#pragma endregion
	}
}

void GameTimeController::FirstAppearMonsters(MonsterType _MonsterType)
{
	// 첫등장조건, 첫등장시간초과시 각 몬스터타입의 첫등장상태로 전환처리
	GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
	if (nullptr == CurLevel)
	{
		GameEngineDebug::MsgBoxError("현재 게임의 현재레벨을 찾을수 없습니다!!!!");
		return;
	}

	// 루미아레벨로 다운캐스팅 후 해당 몬스터타입 첫등장처리
	LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);
	PlayLevel->HostMonsterFirstAppearBatchProcessing(_MonsterType);

	// 몬스터 첫등장상태 일괄변환 패킷전송
	MonsterStateChangePacket ChangeAppearStatePacket;
	ChangeAppearStatePacket.SetIndex(-1);
	ChangeAppearStatePacket.SetMonsterType(_MonsterType);
	ChangeAppearStatePacket.SetMonsterStateType(MonsterStateType::APPEAR);
	ChangeAppearStatePacket.SetTargetIndex(-1);
	GameServer::GetInstance()->Send(&ChangeAppearStatePacket);

	// 모든 몬스터가 첫등장완료시 AllMonsterAppearEnd_ Flag On
	if (MonsterFirstAppearList_[static_cast<int>(MonsterType::WOLF)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::BEAR)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::BAT)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::DOG)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::CHICKEN)].IsAppear_ == true &&
		MonsterFirstAppearList_[static_cast<int>(MonsterType::BOAR)].IsAppear_ == true)
	{
		AllMonsterAppearEnd_ = true;
	}
}

void GameTimeController::MakeNextProhibitedArea()
{
	GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
	LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);

	// "예정" 금지구역을 두 군데 설정 후 멤버변수로 저장하며, 이를 나래이션으로 브로드캐스트합니다.
	for (int i = 0; i < 2; i++)
	{
		int dice0 = randomGenerator_.RandomInt(0, 14);

		if (true == PlayLevel->GetProhibitedAreaList()[dice0]->IsProhibited())
		{
			i--;
			continue;
		}
	
		if (i == 0)
		{
			nextProhibitArea0_ = dice0;
		}
		else if (i == 1)
		{
			if (nextProhibitArea0_ == dice0)
			{
				i = 0;
				continue;
			}

			nextProhibitArea1_ = dice0;
		}
	}

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	PlayLevel->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetNoticeUI()->SetText("다음 금지 구역이 예정되었습니다 : " + PlayLevel->GetProhibitedAreaList()[nextProhibitArea0_]->GetKoreanName() + " & " + PlayLevel->GetProhibitedAreaList()[nextProhibitArea1_]->GetKoreanName(), 5.f);

	NoticeBroadcastPacket packet;
	packet.SetString("다음 금지 구역이 예정되었습니다 : " + PlayLevel->GetProhibitedAreaList()[nextProhibitArea0_]->GetKoreanName() + " & " + PlayLevel->GetProhibitedAreaList()[nextProhibitArea1_]->GetKoreanName());
	packet.SetTimer(5.0f);
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->Send(&packet);
	}

	PlayLevel->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetNextProhibitedArea(static_cast<Location>(nextProhibitArea0_));
	PlayLevel->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetNextProhibitedArea(static_cast<Location>(nextProhibitArea1_));

	ProhibitedAreaPacket propacket;
	propacket.SetReserved(1);
	propacket.SetLocation(nextProhibitArea0_, nextProhibitArea1_);

	if (nextProhibitArea0_ < 0 && nextProhibitArea0_ > 14)
	{
		GameEngineDebug::MsgBoxError("쓔ㅣ발1 next");
	}

	if (nextProhibitArea1_ < 0 && nextProhibitArea1_ > 14)
	{
		GameEngineDebug::MsgBoxError("쓔ㅣ발2 next");
	}

	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->Send(&propacket);
	}
}

void GameTimeController::MakeNextProhibitedArea(Location _location0, Location _location1)
{
}

void GameTimeController::SetProhibitedArea()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	GameEngineLevel* CurLevel = GameEngineCore::CurrentLevel();
	LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurLevel);

	PlayLevel->GetProhibitedAreaList()[nextProhibitArea0_]->SetProhibited(true);
	PlayLevel->GetProhibitedAreaList()[nextProhibitArea1_]->SetProhibited(true);

	PlayLevel->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetProhibitedArea(static_cast<Location>(nextProhibitArea0_));
	PlayLevel->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetProhibitedArea(static_cast<Location>(nextProhibitArea1_));

	PlayLevel->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetNoticeUI()->SetText("새 금지 구역이 지정되었습니다 : " + PlayLevel->GetProhibitedAreaList()[nextProhibitArea0_]->GetKoreanName() + " & " + PlayLevel->GetProhibitedAreaList()[nextProhibitArea1_]->GetKoreanName(), 5.f);

	GameEngineSoundManager::GetInstance()->PlaySoundByName("Announce_Restricted Area_01.wav");

	NoticeBroadcastPacket packet;
	packet.SetString("새 금지 구역이 지정되었습니다 : " + PlayLevel->GetProhibitedAreaList()[nextProhibitArea0_]->GetKoreanName() + " & " + PlayLevel->GetProhibitedAreaList()[nextProhibitArea1_]->GetKoreanName());
	packet.SetTimer(5.0f);

	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->Send(&packet);
	}


	if (nextProhibitArea0_ < 0 && nextProhibitArea0_ > 14)
	{
		GameEngineDebug::MsgBoxError("쓔ㅣ발1 set");
	}

	if (nextProhibitArea1_ < 0 && nextProhibitArea1_ > 14)
	{
		GameEngineDebug::MsgBoxError("쓔ㅣ발2 set");
	}

	ProhibitedAreaPacket propacket;
	propacket.SetReserved(0);
	propacket.SetLocation(nextProhibitArea0_, nextProhibitArea1_);
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->Send(&propacket);
	}

	nextProhibitArea0_ = -1;
	nextProhibitArea1_ = -1;
}

GameTimeController::GameTimeController()
	: IsForcedConversion_(false)
	, DayAndNightTime_(0.0f)
	, CurGameTime_(0.0f)
	, PrevDay_(0)
	, CurDay_(0)
	, CurDayOrNight_(DayAndNightType::DAY)
	, AllMonsterAppearEnd_(false)
	, MonsterFirstAppearList_{}
	, TestTime_(1.0f)
	, nextProhibitArea0_(-1)
	, nextProhibitArea1_(-1)
	, isReserved_(false)
	, isFirstAnnounced_(false)
{
}

GameTimeController::~GameTimeController()
{
}
