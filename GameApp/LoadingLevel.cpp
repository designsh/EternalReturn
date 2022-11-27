#include "Precompile.h"
#include "LoadingLevel.h"

#include "ePacketID.h"
#include "UserGame.h"
#include "LumiaLevel.h"
#include "PlayerInfoManager.h"
#include "MonsterInfoManager.h"
#include "GameServer.h"
#include "GameClient.h"

#include "CreationCommandPacket.h"
#include "LoadingEndPacket.h"
#include "LevelChangePacket.h"

#include "MousePointer.h"
#include "LoadingLevel_BackDrop.h"
#include "LoadingLevel_LoadPercent.h"

#include "LumiaMap.h"

bool LoadingLevel::ResourceLoadEndCheck = false;
bool LoadingLevel::ReadyCreationCommand = false;
bool LoadingLevel::ThreadLoadingEnd = false;

void LoadingLevel::LoadingLevelInitalize()
{
	// 로딩화면 기본 액터 생성

	// 인게임 마우스 생성
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// BackDrop
	BackDrop_ = CreateActor<LoadingLevel_BackDrop>();

	// Connection Player Loading Percent
	// -> 현재 게임에 참여한 플레이어의 수에 따라 로딩율을 계산하며,
	//    모든 플레이어가 로딩이 완료되면 100%로 표시
	LoadPercent_ = CreateActor<LoadingLevel_LoadPercent>();
	LoadPercent_->SetConnectPlayerCount();

	// LoadingLevel Basic Actor Creation End
	ResourceLoadEndCheck = true;
}

void LoadingLevel::CreationCommand()
{
	LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));

	// 루미아레벨에 강제생성 시작
	PlayerLevel->HostCreateCommand();

	// 생성명령 및 패킷 전송완료
	ReadyCreationCommand = true;
}

void LoadingLevel::CheckThreadCreationInfoSettingEnd()
{
	// 서버이며, 몬스터정보생성패킷전송 Flag On일때
	if (true == GameServer::GetInstance()->IsOpened() && (true == MonsterInfoManager::FirstCreationPacketFlag || true == MonsterInfoManager::SecondCreationPacketFlag))
	{
		// 패킷전송
		bool IsConnect = false;
		int PlayerCount = static_cast<int>(PlayerInfoManager::GetInstance()->GetPlayerList().size());
		if (1 < PlayerCount)
		{
			for (int PlayerNum = 0; PlayerNum < PlayerCount; ++PlayerNum)
			{
				// 서버만 생성하고 게임시작버튼클릭시 더미정보를 1개만들어내므로 더미정보를 제외한 클라이언트(게스트)연결된 게임시작시 생성패킷전송
				if (JobType::DUMMY == static_cast<JobType>(PlayerInfoManager::GetInstance()->GetPlayerList()[PlayerNum].character_))
				{
					IsConnect = true;
					break;
				}
			}
		}

		if (false == IsConnect)
		{
			// 최초 몬스터정보 생성일때
			// -> 패킷지연시간 해결필요!!!!!!!!!!!!!!!!!!
			if (true == MonsterInfoManager::FirstCreationPacketFlag)
			{
				// 최초 몬스터정보생성일시 몬스터정보패킷생성
				int TotCount = MonsterInfoManager::GetInstance()->GetCurMonsterListSize();
				for (int MonsterNum = 0; MonsterNum < TotCount; ++MonsterNum)
				{
					CreationCommandPacket CommandPacket;
					CommandPacket.FirstFlagOn();
					CommandPacket.SetTotMonsterCount(TotCount);
					CommandPacket.SetMonsterInfo(MonsterInfoManager::GetInstance()->GetAllMonsterListValue()[MonsterNum]);
					GameServer::GetInstance()->Send(&CommandPacket);
				}

				// Flag Off
				MonsterInfoManager::FirstCreationPacketFlag = false;
			}
			// 이전몬스터정보파일이 존재하는 생성일때
			else if(true == MonsterInfoManager::SecondCreationPacketFlag)
			{
				CreationCommandPacket CommandPacket;
				CommandPacket.FirstFlagOff();
				GameServer::GetInstance()->Send(&CommandPacket);

				// Flag Off
				MonsterInfoManager::SecondCreationPacketFlag = false;
			}
		}
	}
}

void LoadingLevel::CheckThreadLoadingEnd()
{
	// 스레드 종료시 처리
	if (true == ThreadLoadingEnd)
	{
		// 클라이언트라면
		if (true == GameClient::GetInstance()->IsConnected())
		{
			LumiaLevel* LumiaLv = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));

			// A* 노드(타일)정보 갱신
			// -> 주스레드와 로딩스레드가 Context에 동시접근을 막기위해 Public으로 변경하여 외부에서 호출
			LumiaLv->GetMap()->UpdateAStarNodeVertexInfo();
			auto charList = LumiaLv->GetCharacterActorList();
			for (Character* c : charList)
			{
				if (c->IsFocused())
				{
					c->InitEyeSight();
				}
			}


			// 클라이언트(게스트)의 모든 로딩완료시 로딩완료 패킷 전송
			PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
			pm->GetPlayerList()[pm->GetMyNumber()].IsLoading_ = 1;
			LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();

			// Client LoadingEnd Packet Send
			LoadingEndPacket EndPacket;
			EndPacket.SetUniqueID(pm->GetMyNumber());
			EndPacket.SetLoadingFlag(pm->GetPlayerList()[pm->GetMyNumber()].IsLoading_);
			GameClient::GetInstance()->Send(&EndPacket);

			// 모든 처리 종료 후 Flag Off 상태로 전환
			ThreadLoadingEnd = false;
		}
		// 서버라면
		else if (true == GameServer::GetInstance()->IsOpened())
		{
			LumiaLevel* LumiaLv = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));

			// A* 노드(타일)정보 갱신
			// -> 주스레드와 로딩스레드가 Context에 동시접근을 막기위해 Public으로 변경하여 외부에서 호출
			LumiaLv->GetMap()->UpdateAStarNodeVertexInfo();
			auto charList = LumiaLv->GetCharacterActorList();
			for (Character* c : charList)
			{
				if (c->IsFocused())
				{
					c->InitEyeSight();
				}
			}

			// 서버(호스트)로딩상태 갱신
			PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
			pm->GetPlayerList()[pm->GetMyNumber()].IsLoading_ = 1;
			LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();

			// Server LoadingEnd Packet Send
			LoadingEndPacket EndPacket;
			EndPacket.SetUniqueID(pm->GetMyNumber());
			EndPacket.SetLoadingFlag(pm->GetPlayerList()[pm->GetMyNumber()].IsLoading_);
			GameServer::GetInstance()->Send(&EndPacket);

			// 본로직은 로딩엔딩패킷을 수신했을때 연결된 모든 클라이언트의 로딩상태를 체크하여 루미아레벨로 넘어오지만
			// 서버가 제일 마지막에 로딩완료되는 상태가 되었을때 강제로 레벨체인지
			if (true == pm->AllPlayerLoadingStateCheck() && LumiaLv != UserGame::CurrentLevel())
			{
				// 반환값이 true 이라면 모든 플레이어(호스트 포함) 로딩완료 되었다는 의미이므로 레벨 체인지 패킷 송신
				LevelChangePacket Packet;
				Packet.SetChangeLevelName("LumiaLevel");
				GameServer::GetInstance()->Send(&Packet);

				// 패킷송신이 끝났으므로 나의 레벨도 체인지
				UserGame::LevelChange("LumiaLevel");
			}

			// 모든 처리 종료 후 Flag Off 상태로 전환
			ThreadLoadingEnd = false;
		}
	}
}

void LoadingLevel::LevelStart()
{
	// Camera Setting
	GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -400.0f));


	GameEngineDirectory spawnPointDir;
	spawnPointDir.MoveParent("EternalReturn");
	spawnPointDir / "Resources" / "FBX" / "Map";

	MonsterInfoManager::GetInstance()->setMonsterSpawnPoints(spawnPointDir);
}

void LoadingLevel::LevelUpdate(float _DeltaTime)
{
	// 생성패킷정보셋팅 스레드종료체크
	CheckThreadCreationInfoSettingEnd();

	// 로딩스레드종료 체크
	CheckThreadLoadingEnd();

	// 리소스 로딩완료 후 액터생성 명령 패킷 송신
	if (true == GameServer::GetInstance()->IsOpened())
	{
		if (true == ResourceLoadEndCheck && false == ReadyCreationCommand)
		{
			CreationCommand();
		}
	}

	// 리소스 로딩상태 체크 후 기본 액터 생성
	if (0 >= UserGame::LoadingFolder && false == ResourceLoadEndCheck)
	{
		LoadingLevelInitalize();
	}

	// 서버측 패킷 수신처리
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->ProcessPacket();
	}

	// 클라이언트측 패킷 수신처리
	if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->ProcessPacket();
	}
}

void LoadingLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void LoadingLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	// Add Socket Handle
	GameServer* server = GameServer::GetInstance();
	GameClient* client = GameClient::GetInstance();

	if (true == server->IsOpened())
	{
		server->AddPacketHandler(ePacketID::CreationPacket, new CreationCommandPacket);
		server->AddPacketHandler(ePacketID::LoadEndPacket, new LoadingEndPacket);
		server->AddPacketHandler(ePacketID::LvChangePacket, new LevelChangePacket);
	}

	if (true == client->IsConnected())
	{
		client->AddPacketHandler(ePacketID::CreationPacket, new CreationCommandPacket);
		client->AddPacketHandler(ePacketID::LoadEndPacket, new LoadingEndPacket);
		client->AddPacketHandler(ePacketID::LvChangePacket, new LevelChangePacket);
	}
}

LoadingLevel::LoadingLevel()
	: BackDrop_(nullptr)
	, LoadPercent_(nullptr)
{
}

LoadingLevel::~LoadingLevel()
{
}
