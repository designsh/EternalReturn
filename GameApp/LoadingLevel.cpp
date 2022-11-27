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
	// �ε�ȭ�� �⺻ ���� ����

	// �ΰ��� ���콺 ����
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// BackDrop
	BackDrop_ = CreateActor<LoadingLevel_BackDrop>();

	// Connection Player Loading Percent
	// -> ���� ���ӿ� ������ �÷��̾��� ���� ���� �ε����� ����ϸ�,
	//    ��� �÷��̾ �ε��� �Ϸ�Ǹ� 100%�� ǥ��
	LoadPercent_ = CreateActor<LoadingLevel_LoadPercent>();
	LoadPercent_->SetConnectPlayerCount();

	// LoadingLevel Basic Actor Creation End
	ResourceLoadEndCheck = true;
}

void LoadingLevel::CreationCommand()
{
	LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));

	// ��̾Ʒ����� �������� ����
	PlayerLevel->HostCreateCommand();

	// ������� �� ��Ŷ ���ۿϷ�
	ReadyCreationCommand = true;
}

void LoadingLevel::CheckThreadCreationInfoSettingEnd()
{
	// �����̸�, ��������������Ŷ���� Flag On�϶�
	if (true == GameServer::GetInstance()->IsOpened() && (true == MonsterInfoManager::FirstCreationPacketFlag || true == MonsterInfoManager::SecondCreationPacketFlag))
	{
		// ��Ŷ����
		bool IsConnect = false;
		int PlayerCount = static_cast<int>(PlayerInfoManager::GetInstance()->GetPlayerList().size());
		if (1 < PlayerCount)
		{
			for (int PlayerNum = 0; PlayerNum < PlayerCount; ++PlayerNum)
			{
				// ������ �����ϰ� ���ӽ��۹�ưŬ���� ���������� 1�������Ƿ� ���������� ������ Ŭ���̾�Ʈ(�Խ�Ʈ)����� ���ӽ��۽� ������Ŷ����
				if (JobType::DUMMY == static_cast<JobType>(PlayerInfoManager::GetInstance()->GetPlayerList()[PlayerNum].character_))
				{
					IsConnect = true;
					break;
				}
			}
		}

		if (false == IsConnect)
		{
			// ���� �������� �����϶�
			// -> ��Ŷ�����ð� �ذ��ʿ�!!!!!!!!!!!!!!!!!!
			if (true == MonsterInfoManager::FirstCreationPacketFlag)
			{
				// ���� �������������Ͻ� ����������Ŷ����
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
			// ������������������ �����ϴ� �����϶�
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
	// ������ ����� ó��
	if (true == ThreadLoadingEnd)
	{
		// Ŭ���̾�Ʈ���
		if (true == GameClient::GetInstance()->IsConnected())
		{
			LumiaLevel* LumiaLv = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));

			// A* ���(Ÿ��)���� ����
			// -> �ֽ������ �ε������尡 Context�� ���������� �������� Public���� �����Ͽ� �ܺο��� ȣ��
			LumiaLv->GetMap()->UpdateAStarNodeVertexInfo();
			auto charList = LumiaLv->GetCharacterActorList();
			for (Character* c : charList)
			{
				if (c->IsFocused())
				{
					c->InitEyeSight();
				}
			}


			// Ŭ���̾�Ʈ(�Խ�Ʈ)�� ��� �ε��Ϸ�� �ε��Ϸ� ��Ŷ ����
			PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
			pm->GetPlayerList()[pm->GetMyNumber()].IsLoading_ = 1;
			LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();

			// Client LoadingEnd Packet Send
			LoadingEndPacket EndPacket;
			EndPacket.SetUniqueID(pm->GetMyNumber());
			EndPacket.SetLoadingFlag(pm->GetPlayerList()[pm->GetMyNumber()].IsLoading_);
			GameClient::GetInstance()->Send(&EndPacket);

			// ��� ó�� ���� �� Flag Off ���·� ��ȯ
			ThreadLoadingEnd = false;
		}
		// �������
		else if (true == GameServer::GetInstance()->IsOpened())
		{
			LumiaLevel* LumiaLv = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));

			// A* ���(Ÿ��)���� ����
			// -> �ֽ������ �ε������尡 Context�� ���������� �������� Public���� �����Ͽ� �ܺο��� ȣ��
			LumiaLv->GetMap()->UpdateAStarNodeVertexInfo();
			auto charList = LumiaLv->GetCharacterActorList();
			for (Character* c : charList)
			{
				if (c->IsFocused())
				{
					c->InitEyeSight();
				}
			}

			// ����(ȣ��Ʈ)�ε����� ����
			PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
			pm->GetPlayerList()[pm->GetMyNumber()].IsLoading_ = 1;
			LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();

			// Server LoadingEnd Packet Send
			LoadingEndPacket EndPacket;
			EndPacket.SetUniqueID(pm->GetMyNumber());
			EndPacket.SetLoadingFlag(pm->GetPlayerList()[pm->GetMyNumber()].IsLoading_);
			GameServer::GetInstance()->Send(&EndPacket);

			// �������� �ε�������Ŷ�� ���������� ����� ��� Ŭ���̾�Ʈ�� �ε����¸� üũ�Ͽ� ��̾Ʒ����� �Ѿ������
			// ������ ���� �������� �ε��Ϸ�Ǵ� ���°� �Ǿ����� ������ ����ü����
			if (true == pm->AllPlayerLoadingStateCheck() && LumiaLv != UserGame::CurrentLevel())
			{
				// ��ȯ���� true �̶�� ��� �÷��̾�(ȣ��Ʈ ����) �ε��Ϸ� �Ǿ��ٴ� �ǹ��̹Ƿ� ���� ü���� ��Ŷ �۽�
				LevelChangePacket Packet;
				Packet.SetChangeLevelName("LumiaLevel");
				GameServer::GetInstance()->Send(&Packet);

				// ��Ŷ�۽��� �������Ƿ� ���� ������ ü����
				UserGame::LevelChange("LumiaLevel");
			}

			// ��� ó�� ���� �� Flag Off ���·� ��ȯ
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
	// ������Ŷ�������� ����������üũ
	CheckThreadCreationInfoSettingEnd();

	// �ε����������� üũ
	CheckThreadLoadingEnd();

	// ���ҽ� �ε��Ϸ� �� ���ͻ��� ��� ��Ŷ �۽�
	if (true == GameServer::GetInstance()->IsOpened())
	{
		if (true == ResourceLoadEndCheck && false == ReadyCreationCommand)
		{
			CreationCommand();
		}
	}

	// ���ҽ� �ε����� üũ �� �⺻ ���� ����
	if (0 >= UserGame::LoadingFolder && false == ResourceLoadEndCheck)
	{
		LoadingLevelInitalize();
	}

	// ������ ��Ŷ ����ó��
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->ProcessPacket();
	}

	// Ŭ���̾�Ʈ�� ��Ŷ ����ó��
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
