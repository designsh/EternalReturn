#include "PreCompile.h"
#include "LobbyUIController.h"
#include "LobbyLevel.h"

#include "LobbyBackground.h"
#include "Lobby_ButtonOne.h"
#include "Lobby_ButtonTwo.h"
#include "Lobby_PortraitBg.h"
#include "Lobby_CharFull.h"
#include "Lobby_StartButton.h"
#include "Lobby_Map.h"
#include "CharFullSelectEffect.h"

#include "PlayerInfoManager.h"

JobType LobbyUIController::SelectedChar = JobType::MAX;
bool LobbyUIController::CharSelectOn = false;


LobbyUIController::LobbyUIController() // default constructer 디폴트 생성자
{

}

LobbyUIController::~LobbyUIController() // default destructer 디폴트 소멸자
{

}

LobbyUIController::LobbyUIController(LobbyUIController&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void LobbyUIController::InitUI()
{
	{
		backGroundUI_ = GetLevel()->CreateActor<LobbyBackground>();
		hostButtonUI_ = GetLevel()->CreateActor<Lobby_ButtonOne>();
		clientButtonUI_ = GetLevel()->CreateActor<Lobby_ButtonTwo>();
		charFullUI_= GetLevel()->CreateActor<Lobby_CharFull>();
		startButtonUI_ = GetLevel()->CreateActor<Lobby_StartButton>();
		mapUI_ = GetLevel()->CreateActor<Lobby_Map>();
		charFullEffect_ = GetLevel()->CreateActor<CharFullSelectEffect>();
	}

	{
		for (int x = 0; x < (int)JobType::DUMMY; x++)
		{
			Lobby_PortraitBg* Portrait = GetLevel()->CreateActor<Lobby_PortraitBg>();

			float4 BasicPosition = { -580.0f, 200.0f};
			int Vertical = (x / 4);
			int Horizonal = x - (Vertical * 4);

			Portrait->GetTransform()->SetLocalPosition(BasicPosition + float4{ float(Horizonal) * 72.0f, (float(Vertical) * -115.0f), 0.0f });
			Portrait->SetChar(static_cast<JobType>(x));

			PortraitBg.emplace_back(Portrait);
		}
	}
}

Lobby_PortraitBg* LobbyUIController::GetPortraitVector(int _index)
{
	return PortraitBg[_index];
}

void LobbyUIController::Start()
{
	InitUI();
}

void LobbyUIController::Update(float _DeltaTime)
{
	for (int i = 0; i < PlayerInfoManager::GetInstance()->GetPlayerList().size(); i++)
	{
		if (nullptr == mapUI_->GetNodeList()[i])
		{
			continue;
		}

		if (i == PlayerInfoManager::GetInstance()->GetMyNumber())
		{
			GameEngineImageRenderer* renderer = mapUI_->GetNodeList()[i]->GetRenderer();

			switch (static_cast<Location>(PlayerInfoManager::GetInstance()->GetPlayerList()[i].startPoint_))
			{
			case Location::NONE:
				renderer->Off();

				break;
			case Location::DOCK:
				renderer->On();
				renderer->SetImage("Map_Harbor_Pin.png", "PointSmp");
				break;
			case Location::POND:
				renderer->On();
				renderer->SetImage("Map_Pond_Pin.png", "PointSmp");
				break;
			case Location::BEACH:
				renderer->On();
				renderer->SetImage("Map_SandyBeach_Pin.png", "PointSmp");
				break;
			case Location::UPTOWN:
				renderer->On();
				renderer->SetImage("Map_UpTown_Pin.png", "PointSmp");
				break;
			case Location::ALLEY:
				renderer->On();
				renderer->SetImage("Map_Alley_Pin.png", "PointSmp");
				break;
			case Location::HOTEL:
				renderer->On();
				renderer->SetImage("Map_Hotel_Pin.png", "PointSmp");
				break;
			case Location::AVENUE:
				renderer->On();
				renderer->SetImage("Map_DownTown_Pin.png", "PointSmp");
				break;
			case Location::HOSPITAL:
				renderer->On();
				renderer->SetImage("Map_Hospital_Pin.png", "PointSmp");
				break;
			case Location::TEMPLE:
				renderer->On();
				renderer->SetImage("Map_Temple_Pin.png", "PointSmp");
				break;
			case Location::ARCHERY_RANGE:
				renderer->On();
				renderer->SetImage("Map_Archery_Pin.png", "PointSmp");
				break;
			case Location::CEMETERY:
				renderer->On();
				renderer->SetImage("Map_Cemetery_Pin.png", "PointSmp");
				break;
			case Location::FOREST:
				renderer->On();
				renderer->SetImage("Map_Forest_Pin.png", "PointSmp");
				break;
			case Location::FACTORY:
				renderer->On();
				renderer->SetImage("Map_Factory_Pin.png", "PointSmp");
				break;
			case Location::CHAPEL:
				renderer->On();
				renderer->SetImage("Map_Church_Pin.png", "PointSmp");
				break;
			case Location::SCHOOL:
				renderer->On();
				renderer->SetImage("Map_School_Pin.png", "PointSmp");
				break;
			case Location::RESERCH_CENTRE:
				//renderer->On();
				//renderer->SetImage("Map_Laboratory_Pin.png", "PointSmp");
				break;
			case Location::ESCAPE_DOCK:
				renderer->Off();
				break;
			case Location::MAX:
				renderer->Off();
				break;
			default:
				break;
			}
		}
		else
		{
			GameEngineImageRenderer* renderer = mapUI_->GetNodeList()[i]->GetRenderer();

			switch (static_cast<Location>(PlayerInfoManager::GetInstance()->GetPlayerList()[i].startPoint_))
			{
			case Location::NONE:
				renderer->Off();

				break;
			case Location::DOCK:
				renderer->On();
				renderer->SetImage("Map_Harbor_Pin_Enemy.png", "PointSmp");
				break;
			case Location::POND:
				renderer->On();
				renderer->SetImage("Map_Pond_Pin_Enemy.png", "PointSmp");
				break;
			case Location::BEACH:
				renderer->On();
				renderer->SetImage("Map_SandyBeach_Pin_Enemy.png", "PointSmp");
				break;
			case Location::UPTOWN:
				renderer->On();
				renderer->SetImage("Map_UpTown_Pin_Enemy.png", "PointSmp");
				break;
			case Location::ALLEY:
				renderer->On();
				renderer->SetImage("Map_Alley_Pin_Enemy.png", "PointSmp");
				break;
			case Location::HOTEL:
				renderer->On();
				renderer->SetImage("Map_Hotel_Pin_Enemy.png", "PointSmp");
				break;
			case Location::AVENUE:
				renderer->On();
				renderer->SetImage("Map_DownTown_Pin_Enemy.png", "PointSmp");
				break;
			case Location::HOSPITAL:
				renderer->On();
				renderer->SetImage("Map_Hospital_Pin_Enemy.png", "PointSmp");
				break;
			case Location::TEMPLE:
				renderer->On();
				renderer->SetImage("Map_Temple_Pin_Enemy.png", "PointSmp");
				break;
			case Location::ARCHERY_RANGE:
				renderer->On();
				renderer->SetImage("Map_Archery_Pin_Enemy.png", "PointSmp");
				break;
			case Location::CEMETERY:
				renderer->On();
				renderer->SetImage("Map_Cemetery_Pin_Enemy.png", "PointSmp");
				break;
			case Location::FOREST:
				renderer->On();
				renderer->SetImage("Map_Forest_Pin_Enemy.png", "PointSmp");
				break;
			case Location::FACTORY:
				renderer->On();
				renderer->SetImage("Map_Factory_Pin_Enemy.png", "PointSmp");
				break;
			case Location::CHAPEL:
				renderer->On();
				renderer->SetImage("Map_Church_Pin_Enemy.png", "PointSmp");
				break;
			case Location::SCHOOL:
				renderer->On();
				renderer->SetImage("Map_School_Pin_Enemy.png", "PointSmp");
				break;
			case Location::RESERCH_CENTRE:
				//renderer->On();
				//renderer->SetImage("Map_Laboratory_Pin_Enemy.png", "PointSmp");
				break;
			case Location::ESCAPE_DOCK:
				renderer->Off();
				break;
			case Location::MAX:
				renderer->Off();
				break;
			default:
				break;
			}
		}
	
	

	}

	for (int i = 0; i < mapUI_->GetCapacityCounterList().size(); i++)
	{

		int tempCount = 0;
		// 모든 맵의 해당되는 카운터 리스트를 쭉 타고 돌아다니며 내부 카운트를 갱신한다.
		for (int o = 0; o < PlayerInfoManager::GetInstance()->GetPlayerList().size(); o++)
		{
			if (i == PlayerInfoManager::GetInstance()->GetPlayerList()[o].startPoint_)
			{
				tempCount++;
			}
			else
			{
				continue;
			}
		}

		mapUI_->GetCapacityCounterList()[i]->SetCounter(tempCount);

	}
}

