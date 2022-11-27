#include "Precompile.h"
#include "LumiaLevel.h"

#include <GameEngine/GameEngineLightComponent.h>
#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/LightActor.h>

#include "ItemBoxWindow.h"
#include "MonsterDebugWindow.h"

#include "Enums.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include "MonsterInfoManager.h"
#include "UserGame.h"
#include "GameServer.h"
#include "GameClient.h"

#include "CharMovementPacket.h"
#include "CharAnimationPacket.h"
#include "CharStatPacket.h"
#include "CharCrowdControlPacket.h"
#include "CreationCommandPacket.h"
#include "CharEffectPacket.h"
#include "LoadingEndPacket.h"
#include "PacketCreateProjectile.h"
#include "PacketSoundPlay.h"
#include "MonsterStateChangePacket.h"
#include "GameTimeSyncPacket.h"
#include "CharDeathPacket.h"
#include "MonsterCrowdControlPacket.h"
#include "NoticeBroadcastPacket.h"
#include "ProhibitedAreaPacket.h"

//======================== Level
#include "LoadingLevel.h"

//======================== Controller
#include "GameTimeController.h"
#include "LevelUpSystem.h"
#include "PlayerUIController.h"

//======================== Map
#include "LumiaMap.h"
#include "ItemBoxManager.h"
#include "HyperLoop.h"

//======================== Characters
#include "Character.h"
#include "Dummy.h"
#include "Rio.h"
#include "Hyunwoo.h"
#include "Jackie.h"
#include "Yuki.h"

//======================== Monsters
#include "Monsters.h"
#include "Wolf.h"
#include "Bat.h"
#include "Chicken.h"
#include "Bear.h"
#include "Boar.h"
#include "Dog.h"
#include "Weekline.h"
#include "Aya.h"

//======================== Area
#include "ProhibitedArea.h"
#include "AreaChangeTrigger.h"

Character* LumiaLevel::GetSpecificCharacter(int _Index)
{
	// Ư�� �ε����� ĳ���� ��ü������ Get
	if (_Index >= static_cast<int>(CharacterActorList_.size()) || 0 > _Index)
	{
		GameEngineDebug::MsgBoxError("�߸��� �ε����� ĳ���͸� ��ȸ�Ϸ��õ��߽��ϴ�!!!");
		return nullptr;
	}

	return CharacterActorList_[_Index];
}

Monsters* LumiaLevel::GetSpecificMonster(int _Index)
{
	// Ư�� �ε����� ���� ��ü������ Get
	if (_Index >= static_cast<int>(MonsterActorList_.size()) || 0 > _Index)
	{
		GameEngineDebug::MsgBoxError("�߸��� �ε����� ���͸� ��ȸ�Ϸ��õ��߽��ϴ�!!!");
		return nullptr;
	}

	return MonsterActorList_[_Index];
}

std::vector<Monsters*>& LumiaLevel::GetMonsterTypeList(MonsterType _MonsterType)
{
	return MonsterActorByTypeList_[static_cast<int>(_MonsterType)];
}

void LumiaLevel::HostCreateCommand()
{
	// ����(ȣ��Ʈ) ������ �۾��Ҵ�
	GameEngineCore::ThreadQueue.JobPost([&]
		{
			// ����(ȣ��Ʈ) ���� ����� �����Ϸ��� ���� ���� ����
			CreateMonsterInfo();

			// ����(ȣ��Ʈ)�� ���ҽ� �ε� �� ���ͻ��� ����
			HostAllCreationCommand();

			// Loading Thread End Flag On
			LoadingLevel::ThreadLoadingEnd = true;
		}
	);
}

void LumiaLevel::GuestCreateCommand()
{
	int Count = GameEngineCore::ThreadQueue.GetWorkingCount();

	// Ŭ���̾�Ʈ(�Խ�Ʈ)�� ���ҽ� �ε� �� ���ͻ��� ����
	GameEngineCore::ThreadQueue.JobPost([&]
		{
			GuestAllCreationCommand();

			// Loading Thread End Flag On
			LoadingLevel::ThreadLoadingEnd = true;
		}
	);
}

void LumiaLevel::HostMonsterFirstAppearBatchProcessing(MonsterType _MonsterType)
{
	// ����ó��
	if (MonsterType::NONE == _MonsterType || MonsterType::MAX == _MonsterType)
	{
		GameEngineDebug::MsgBoxError("�߸��� Ÿ���� ���͸� �����Ű���� �õ��߽��ϴ�!!!!!");
		return;
	}

	// �ش� ����Ÿ���� ���ʹ� �ϰ������� ù������·� ��ȯ
	int MonsterCount = static_cast<int>(MonsterActorByTypeList_[static_cast<int>(_MonsterType)].size());
	for (int MonsterNum = 0; MonsterNum < MonsterCount; ++MonsterNum)
	{
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->On();
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->ChangeAnimationAndState(MonsterStateType::APPEAR);
	}
}

void LumiaLevel::GuestMonsterFirstAppearBatchProcessing(MonsterType _MonsterType)
{
	// ����ó��
	if (MonsterType::NONE == _MonsterType || MonsterType::MAX == _MonsterType)
	{
		GameEngineDebug::MsgBoxError("�߸��� Ÿ���� ���͸� �����Ű���� �õ��߽��ϴ�!!!!!");
		return;
	}

	// �ش� ����Ÿ���� ���ʹ� �ϰ������� ù������·� ��ȯ
	int MonsterCount = static_cast<int>(MonsterActorByTypeList_[static_cast<int>(_MonsterType)].size());
	for (int MonsterNum = 0; MonsterNum < MonsterCount; ++MonsterNum)
	{
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->On();
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->ChangeAnimationAndState(MonsterStateType::APPEAR);
	}
}

void LumiaLevel::CreateMonsterInfo()
{
	if (true == MonsterInfoManager::GetInstance()->CreatMonsterInfomation())
	{
		GameEngineDebug::OutPutDebugString("����(ȣ��Ʈ)���� ����ʿ� ��ġ�Ϸ��� ���� ���� ������ �Ϸ��߽��ϴ�!!!!");
	}
}

void LumiaLevel::HostAllCreationCommand()
{
	// Host(����) �������� ���
	MapCreationCommand();
	CharacterCreationCommand();
	MonsterCreationCommand();
}

void LumiaLevel::GuestAllCreationCommand()
{
	// Guest(Ŭ���̾�Ʈ) �������� ���
	MapCreationCommand();
	CharacterCreationCommand();
	MonsterCreationCommand();
}

void LumiaLevel::MapCreationCommand()
{
	// �ʰ��� ���ҽ� �ε�
	MapResourceLoad();

	// �� ����
	CurMap_ = CreateActor<LumiaMap>();

	std::vector<float4> hyperLoopSpawnPoints = CurMap_->GetHyperLoopSpawnPoints();


	GameEngineActor* HyperLoops[8] = { nullptr, };

	for (int i = 0; i < 8; i++)
	{
		HyperLoops[i] = CreateActor<HyperLoop>();
		HyperLoops[i]->GetTransform()->SetWorldPosition(hyperLoopSpawnPoints[i] + float4(0.0f, 5.0f, 0.0f));
	}

	HyperLoops[0]->GetTransform()->SetWorldRotationDegree({ 0.0f, 180.0f, 0.0f });
	HyperLoops[1]->GetTransform()->SetWorldRotationDegree({ 0.0f, 90.0f, 0.0f });
	HyperLoops[2]->GetTransform()->SetWorldRotationDegree({ 0.0f, 90.0f, 0.0f });
	HyperLoops[3]->GetTransform()->SetWorldRotationDegree({ 0.0f, 180.0f, 0.0f });
	HyperLoops[4]->GetTransform()->SetWorldRotationDegree({ 0.0f, 180.0f, 0.0f });
	HyperLoops[5]->GetTransform()->SetWorldRotationDegree({ 0.0f, 90.0f, 0.0f });
	HyperLoops[6]->GetTransform()->SetWorldRotationDegree({ 0.0f, 90.0f, 0.0f });
	HyperLoops[7]->GetTransform()->SetWorldRotationDegree({ 0.0f, 180.0f, 0.0f });

	//#ifdef _DEBUG
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "��° �÷��̾��� �ʻ����� �Ϸ��߽��ϴ�\n");
	//#endif // _DEBUG
}

void LumiaLevel::MonsterCreationCommand()
{
	// ������������� �����Ͽ� ���� ��������
	GameTimeController* gm = GameTimeController::GetInstance();
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	int AllMonsterCount = mm->GetCurMonsterListSize();
	for (int MonsterNum = 0; MonsterNum < AllMonsterCount; ++MonsterNum)
	{
		Monsters* NewMonster = nullptr;

		// ���� ���� �������� Get
		MonsterInfo CurMonsterInfo = mm->GetAllMonsterListRef()[MonsterNum];

		// ���� ������ Ÿ��üũ
		MonsterType CurMonsterType = CurMonsterInfo.MonsterType_;
		switch (CurMonsterType)
		{
		case MonsterType::WOLF:					// ����
		{
			NewMonster = CreateActor<Wolf>();
			break;
		}
		case MonsterType::BEAR:					// ��
		{
			NewMonster = CreateActor<Bear>();
			break;
		}
		case MonsterType::BAT:					// ����
		{
			NewMonster = CreateActor<Bat>();
			break;
		}
		case MonsterType::DOG:					// �鰳
		{
			NewMonster = CreateActor<Dog>();
			break;
		}
		case MonsterType::CHICKEN:				// ��
		{
			NewMonster = CreateActor<Chicken>();
			break;
		}
		case MonsterType::BOAR:					// �����
		{
			NewMonster = CreateActor<Boar>();
			break;
		}
		//case MonsterType::WEEKLINE:				// Dr.��Ŭ����
		//{
		//	GameEngineDebug::OutPutDebugString("������������ ����Ÿ��(Dr.��Ŭ����)�Դϴ�!!!!");
		//	break;
		//}
		}

		// ����ó��
		if (nullptr == NewMonster)
		{
			//GameEngineDebug::OutPutDebugString("�߸��� Ÿ���� ���ͻ����� �õ��߽��ϴ�!!!! ����Ÿ��: " + std::to_string(static_cast<int>(CurMonsterType)) + " \n");
			continue;
		}

		// ������ ���� �������� ����
		NewMonster->SetMonsterIndex(CurMonsterInfo.Index_);
		NewMonster->SetMonsterAreaType(CurMonsterInfo.RegionType_);

		// Ʈ������������ ����
		NewMonster->InitalizeSpawnPosition(CurMonsterInfo.SpawnPosition_);

		// GameController�� ���Ͽ� ù���������� Off ���·� ����
		// -> GameController�� ù�������� ������ On���·� ��ȯ�Ǹ� Appear State�� ����
		NewMonster->Off();

		// ������� �߰�
		MonsterActorList_.push_back(NewMonster);

		// �ϰ�ó���� ���͸�Ͽ� �߰�
		MonsterActorByTypeList_[static_cast<int>(CurMonsterType)].push_back(NewMonster);
	}

//#ifdef _DEBUG
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "��° �÷��̾��� ���ͻ����� �Ϸ��߽��ϴ�\n");
//#endif // _DEBUG
}

void LumiaLevel::CharacterCreationCommand()
{
	// ĳ���Ͱ��� ���ҽ� �ε�
	CharacterResourceLoad();

	// Get ���� ���ӿ� ������ �÷��̾� ����
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	// ĳ���� ����
	int PlayerCount = static_cast<int>(pm->GetPlayerList().size());
	SurvivorCount_ = PlayerCount;

	for (int PlayerNum = 0; PlayerNum < PlayerCount; ++PlayerNum)
	{
		// ĳ���� Ÿ�Ժ�
		JobType CurCharacterType = static_cast<JobType>(pm->GetPlayerList()[PlayerNum].character_);

		Character* NewCharacter = nullptr;
		switch (CurCharacterType)
		{
		case JobType::YUKI:
		{
			NewCharacter = CreateActor<Yuki>();
			break;
		}
		case JobType::HYUNWOO:
		{
			NewCharacter = CreateActor<Hyunwoo>();
			break;
		}
		case JobType::AYA:
		{
			NewCharacter = CreateActor<Aya>();
			break;
		}
		case JobType::RIO:
		{
			NewCharacter = CreateActor<Rio>();
			break;
		}
		case JobType::JACKIE:
		{
			NewCharacter = CreateActor<Jackie>();
			break;
		}
		case JobType::DUMMY:
		{
			NewCharacter = CreateActor<Rio>();
			break;
		}
		}

		// ����ó��
		if (nullptr == NewCharacter)
		{
			GameEngineDebug::MsgBoxError("��ϵ������� ĳ���� Ÿ������ �����Ϸ��� �÷��̾ �����մϴ�!!!!\n");
			continue;
		}

		// 
		pm->GetPlayerList()[PlayerNum].stat_ = NewCharacter->GetStat();
		int spawnArea = pm->GetPlayerList()[PlayerNum].startPoint_;
		NewCharacter->SetCurLocation(static_cast<Location>(pm->GetPlayerList()[PlayerNum].startPoint_));

		// ���� ���� ���ϸ� ����Ʈ�� ����
		if (-1 == spawnArea)
		{
			pm->GetPlayerList()[PlayerNum].curPos_ = float4(-2500.f, 0.0f, 10000.f);
			NewCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			NewCharacter->SetCurLocation(Location::AVENUE);
		}
		else
		{
			std::vector<float4> spawnPoints = CurMap_->GetCharacterSpawnPoints(spawnArea);
			GameEngineRandom random;
			int point = random.RandomInt(0, static_cast<int>(spawnPoints.size()) - 1);
			pm->GetPlayerList()[PlayerNum].curPos_ = spawnPoints[point];
			NewCharacter->InitSpawnPoint(spawnPoints[point]);
		}

		NewCharacter->SetIndex(PlayerNum);
		NewCharacter->UnitType_ = UnitType::CHARACTER;

		if (pm->GetMyNumber() == PlayerNum)
		{
			NewCharacter->SetUIController();
		}

		// ������Ͽ� �߰�
		CharacterActorList_.emplace_back(NewCharacter);

		// ���� ȣ��Ʈ or �Խ�Ʈ�� ���� ĳ���� ��Ŀ��
		// ��, ��������->ĳ���ͼ���->���ӽ��۽� ����Ÿ���� ĳ���͸� �����ϹǷ� ����Ÿ���� ĳ���Ͱ� ����ĳ���ͷ� �����Ǵ� ������ �����Ѵ�.
		if (PlayerNum == pm->GetMyNumber() && JobType::DUMMY != CurCharacterType)
		{
			CharacterActorList_[PlayerNum]->Focus();
			pm->SetMainCharacter(NewCharacter);
		}
	}

	{
		CharacterActorList_[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->InitCharList();
		CharacterActorList_[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->InitAreaRenderList();
	}

	//#ifdef _DEBUG
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "��° �÷��̾��� ĳ���ͻ����� �Ϸ��߽��ϴ�\n");
	//#endif // _DEBUG
}

void LumiaLevel::MapResourceLoad()
{
	//========================================== FBX & UserMesh Files
	GameEngineDirectory NaviMeshDir;
	NaviMeshDir.MoveParent("EternalReturn");
	NaviMeshDir / "Resources" / "FBX" / "Map";
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(NaviMeshDir.PathToPlusFileName("Bg_NaviMesh.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(NaviMeshDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	GameEngineDirectory UserMapMeshDir;
	UserMapMeshDir.MoveParent("EternalReturn");
	UserMapMeshDir / "Resources" / "FBX" / "UserMesh" / "Map";
	std::vector<GameEngineFile> vecMapFile = UserMapMeshDir.GetAllFile(".UserMesh");
	for (size_t i = 0; i < vecMapFile.size(); i++)
	{
		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecMapFile[i].GetFullPath()))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecMapFile[i].GetFullPath());
		}
	}

	//==================== ItemBox
	GameEngineDirectory UserItemBoxMeshDir;
	UserItemBoxMeshDir.MoveParent("EternalReturn");
	UserItemBoxMeshDir / "Resources" / "FBX" / "UserMesh" / "ItemBox";
	std::vector<GameEngineFile> vecItemBoxFile = UserItemBoxMeshDir.GetAllFile(".UserMesh");
	for (size_t i = 0; i < vecItemBoxFile.size(); i++)
	{
		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecItemBoxFile[i].GetFullPath()))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecItemBoxFile[i].GetFullPath());
		}
	}

	GameEngineDirectory ItemBoxInfoDir;
	ItemBoxInfoDir.MoveParent("EternalReturn");
	ItemBoxInfoDir / "Resources" / "FBX" / "UserMesh" / "ItemBox" / "ItemBoxInfo";
	ItemBoxManager_ = CreateActor<ItemBoxManager>();
	ItemBoxManager_->UserAllLoad(ItemBoxInfoDir);
	ItemBoxManager_->GetTransform()->SetLocalScaling(100.0f);
	ItemBoxManager_->SetGatherItem();
}

void LumiaLevel::CharacterResourceLoad()
{
	Character::LoadResource();
	Hyunwoo::LoadResource();
	Rio::LoadResource();
	Jackie::LoadResource();
	Yuki::LoadResource();
	Aya::LoadResource();
}

void LumiaLevel::CreateBasicActor()
{
	// �ΰ��� ���콺 ����
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// Create SkysphereActor
	SKySphereActor* Actor = CreateActor<SKySphereActor>();

	// Create Default Light
	MainLight_ = CreateActor<LightActor>();
	MainLight_->GetLight()->SetDiffusePower(1.f);
	MainLight_->GetLight()->SetAmbientPower(10.f);
	MainLight_->GetLight()->SetSpacularLightPow(50.f);
	MainLight_->GetLight()->SetLightShadowClipingRange({ 1280.0f, 720.0f });
	MainLight_->GetTransform()->SetLocalRotationDegree({ 90.0f, 0.0f, 0.0f });
	MainLight_->GetTransform()->SetWorldPosition({ -2500.f, 0.0f, 10000.f });
	MainLight_->GetTransform()->AddWorldPosition(MainLight_->GetTransform()->GetWorldBackVector() * 200.0f);
}

void LumiaLevel::CreateLevelInput()
{
	// Character Related Key
	if (false == GameEngineInput::GetInst().IsKey("A"))
	{
		GameEngineInput::GetInst().CreateKey("A", 'A');
	}

	if (false == GameEngineInput::GetInst().IsKey("D"))
	{
		GameEngineInput::GetInst().CreateKey("D", 'D');
	}

	if (false == GameEngineInput::GetInst().IsKey("W"))
	{
		GameEngineInput::GetInst().CreateKey("W", 'W');
	}

	if (false == GameEngineInput::GetInst().IsKey("S"))
	{
		GameEngineInput::GetInst().CreateKey("S", 'S');
	}

	if (false == GameEngineInput::GetInst().IsKey("Q"))
	{
		GameEngineInput::GetInst().CreateKey("Q", 'Q');
	}

	if (false == GameEngineInput::GetInst().IsKey("E"))
	{
		GameEngineInput::GetInst().CreateKey("E", 'E');
	}

	if (false == GameEngineInput::GetInst().IsKey("O"))
	{
		GameEngineInput::GetInst().CreateKey("O", 'O');
	}

	if (false == GameEngineInput::GetInst().IsKey("N"))
	{
		GameEngineInput::GetInst().CreateKey("N", 'N');
	}

	if (false == GameEngineInput::GetInst().IsKey("L"))
	{
		GameEngineInput::GetInst().CreateKey("L", 'L');
	}

	if (false == GameEngineInput::GetInst().IsKey("I"))
	{
		GameEngineInput::GetInst().CreateKey("I", 'I');
	}

	if (false == GameEngineInput::GetInst().IsKey("LButton"))
	{
		GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	}

	if (false == GameEngineInput::GetInst().IsKey("RButton"))
	{
		GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
	}

	if (false == GameEngineInput::GetInst().IsKey("Esc"))
	{
		GameEngineInput::GetInst().CreateKey("Esc", VK_ESCAPE);
	}

	// Inventory Related Key
	if (false == GameEngineInput::GetInst().IsKey("1"))
	{
		GameEngineInput::GetInst().CreateKey("1", '1');
	}

	if (false == GameEngineInput::GetInst().IsKey("2"))
	{
		GameEngineInput::GetInst().CreateKey("2", '2');
	}

	if (false == GameEngineInput::GetInst().IsKey("3"))
	{
		GameEngineInput::GetInst().CreateKey("3", '3');
	}
	
	if (false == GameEngineInput::GetInst().IsKey("4"))
	{
		GameEngineInput::GetInst().CreateKey("4", '4');
	}
	
	if (false == GameEngineInput::GetInst().IsKey("5"))
	{
		GameEngineInput::GetInst().CreateKey("5", '5');
	}
	
	if (false == GameEngineInput::GetInst().IsKey("6"))
	{
		GameEngineInput::GetInst().CreateKey("6", '6');
	}
	
	if (false == GameEngineInput::GetInst().IsKey("7"))
	{
		GameEngineInput::GetInst().CreateKey("7", '7');
	}
	
	if (false == GameEngineInput::GetInst().IsKey("8"))
	{
		GameEngineInput::GetInst().CreateKey("8", '8');
	}
	
	if (false == GameEngineInput::GetInst().IsKey("9"))
	{
		GameEngineInput::GetInst().CreateKey("9", '9');
	}
	
	if (false == GameEngineInput::GetInst().IsKey("0"))
	{
		GameEngineInput::GetInst().CreateKey("0", '0');
	}

	if (false == GameEngineInput::GetInst().IsKey("B"))
	{
		GameEngineInput::GetInst().CreateKey("B", 'B');
	}

	// Monster Related Key
	if (false == GameEngineInput::GetInst().IsKey("MonsterDebugWindowOnOrOff"))
	{
		GameEngineInput::GetInst().CreateKey("MonsterDebugWindowOnOrOff", VK_LCONTROL);
	}

	// Test Related Key
	if (false == GameEngineInput::GetInst().IsKey("ForcedChangeDayAndNight"))				// ���� ��/�� ��ȯŰ ����
	{
		GameEngineInput::GetInst().CreateKey("ForcedChangeDayAndNight", 'v');
	}
}

void LumiaLevel::AddSocketHandle()
{
	GameServer* server = GameServer::GetInstance();
	GameClient* client = GameClient::GetInstance();

	if (true == server->IsOpened())
	{
		server->RemovePacketHandler(ePacketID::LobbyToLumiaPacket);
		server->RemovePacketHandler(ePacketID::GameJoinPacket2);
		server->RemovePacketHandler(ePacketID::SetPlayerNumberPacket);
		server->RemovePacketHandler(ePacketID::CharSelectPacket);
		server->RemovePacketHandler(ePacketID::StartPointSelectPacket);
		server->RemovePacketHandler(ePacketID::ReadyPacket);
		server->RemovePacketHandler(ePacketID::StartPacket);

		server->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
		server->AddPacketHandler(ePacketID::CharAnimationPacket, new CharAnimationPacket);
		server->AddPacketHandler(ePacketID::CharStatPacket, new CharStatPacket);
		server->AddPacketHandler(ePacketID::CharCrowdControlPacket, new CharCrowdControlPacket);
		server->AddPacketHandler(ePacketID::PacketCreateProjectile, new PacketCreateProjectile);
		server->AddPacketHandler(ePacketID::PacketSoundPlay, new PacketSoundPlay);
		server->AddPacketHandler(ePacketID::MonsterStatePacket, new MonsterStateChangePacket);
		server->AddPacketHandler(ePacketID::TimeSyncPacket, new GameTimeSyncPacket);
		server->AddPacketHandler(ePacketID::CharEffectPacket, new CharEffectPacket);
		server->AddPacketHandler(ePacketID::CharDeathPacket, new CharDeathPacket);
		server->AddPacketHandler(ePacketID::MonsterDamagePacket, new MonsterStateChangePacket);
		server->AddPacketHandler(ePacketID::MonsterCCPacket, new MonsterCrowdControlPacket);
		server->AddPacketHandler(ePacketID::NoticeBroadcastPacket, new NoticeBroadcastPacket);
		server->AddPacketHandler(ePacketID::ProhibitedAreaPacket, new ProhibitedAreaPacket);
	}

	if (true == client->IsConnected())
	{
		client->RemovePacketHandler(ePacketID::LobbyToLumiaPacket);
		client->RemovePacketHandler(ePacketID::GameJoinPacket2);
		client->RemovePacketHandler(ePacketID::SetPlayerNumberPacket);
		client->RemovePacketHandler(ePacketID::CharSelectPacket);
		client->RemovePacketHandler(ePacketID::StartPointSelectPacket);
		client->RemovePacketHandler(ePacketID::ReadyPacket);
		client->RemovePacketHandler(ePacketID::StartPacket);

		client->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
		client->AddPacketHandler(ePacketID::CharAnimationPacket, new CharAnimationPacket);
		client->AddPacketHandler(ePacketID::CharStatPacket, new CharStatPacket);
		client->AddPacketHandler(ePacketID::CharCrowdControlPacket, new CharCrowdControlPacket);
		client->AddPacketHandler(ePacketID::PacketCreateProjectile, new PacketCreateProjectile);
		client->AddPacketHandler(ePacketID::PacketSoundPlay, new PacketSoundPlay);
		client->AddPacketHandler(ePacketID::MonsterStatePacket, new MonsterStateChangePacket);
		client->AddPacketHandler(ePacketID::TimeSyncPacket, new GameTimeSyncPacket);
		client->AddPacketHandler(ePacketID::CharEffectPacket, new CharEffectPacket);
		client->AddPacketHandler(ePacketID::CharDeathPacket, new CharDeathPacket);
		client->AddPacketHandler(ePacketID::MonsterDamagePacket, new MonsterStateChangePacket);
		client->AddPacketHandler(ePacketID::MonsterCCPacket, new MonsterCrowdControlPacket);
		client->AddPacketHandler(ePacketID::NoticeBroadcastPacket, new NoticeBroadcastPacket);
		client->AddPacketHandler(ePacketID::ProhibitedAreaPacket, new ProhibitedAreaPacket);
	}
}

void LumiaLevel::InitIMGUIWindow()
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("���� ī�޶� Ÿ��", GetMainCamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("UI ī�޶� Ÿ��", GetUICamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("���� ī�޶� G-Buffer", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size);
		Window->PushRenderTarget("���� ī�޶� ���۵� ����Ʈ", GetMainCamera()->GetCameraDeferredLightTarget(), Size);
		Window->PushRenderTarget("���� ī�޶� ���۵� Ÿ��", GetMainCamera()->GetCameraDeferredTarget(), Size);
	}

	DebugAndControlWindow_ = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");

	ItemBoxWindow* Window = nullptr;
	if (nullptr == GameEngineGUI::GetInst()->FindGUIWindow("ItemBoxWindow"))
	{
		Window = GameEngineGUI::GetInst()->CreateGUIWindow<ItemBoxWindow>("ItemBoxWindow");
	}

	ItemListWindow_ = nullptr;
	if (nullptr == GameEngineGUI::GetInst()->FindGUIWindow("ItemListWindow"))
	{
		ItemListWindow_ = GameEngineGUI::GetInst()->CreateGUIWindow<ItemListWindow>("ItemListWindow");
	}

	if (nullptr != ItemListWindow_ && nullptr != ItemBoxManager_)
	{
		ItemListWindow_->ItemBoxManager_ = ItemBoxManager_;
	}

	ItemListWindow_->Off();

	if (nullptr != Window && nullptr != ItemBoxManager_)
	{
		Window->ItemBoxManager_ = ItemBoxManager_;
	}

	// 221018 SJH ADD : ���� ����Ȯ�ο� ����� ����
	MonsterDebugWindow_ = GameEngineGUI::GetInst()->FindGUIWindowConvert<MonsterDebugWindow>("MonsterDebugWindow");
	if (nullptr == MonsterDebugWindow_)
	{
		MonsterDebugWindow_ = GameEngineGUI::GetInst()->CreateGUIWindow<MonsterDebugWindow>("MonsterDebugWindow");
		MonsterDebugWindow_->OnOffChange();
	}
}

void LumiaLevel::CameraAdjustment()
{
	GetMainCameraActor()->GetCamera()->SetFov(50.f);
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 100.f, -200.f });
}

void LumiaLevel::CreateProhibitedSystem()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("EternalReturn");
		dir / "Resources" / "FBX" / "Prohibited";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_AVENUE.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_ALLEY.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_ARCHERY.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_BEACH.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_CEMETERY.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_CHAPEL.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_DOCK.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_FACTORY.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_HOSPITAL.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_HOTEL.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_FOREST.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_POND.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_RESERCH_CENTRE.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_SCHOOL.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_TEMPLE.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Prohibited_UPTOWN.fbx"));
		mesh->CreateRenderingBuffer();
	}

	float4 position = { 0.0f, 25.0f, 0.0f };

	for (int i = 0; i < 16; i++)
	{
		ProhibitedArea* area = CreateActor<ProhibitedArea>();

		Location locaName = static_cast<Location>(i);
		switch (locaName)
		{
		case Location::NONE:
			break;
		case Location::DOCK:
			area->Init(locaName, "Prohibited_DOCK.fbx", position, "�ױ�");
			break;
		case Location::POND:
			area->Init(locaName, "Prohibited_POND.fbx", position, "����");
			break;
		case Location::BEACH:
			area->Init(locaName, "Prohibited_BEACH.fbx", position, "�𷡻���");
			break;
		case Location::UPTOWN:
			area->Init(locaName, "Prohibited_UPTOWN.fbx", position, "��� ���ð�");
			break;
		case Location::ALLEY:
			area->Init(locaName, "Prohibited_ALLEY.fbx", position, "����");
			break;
		case Location::HOTEL:
			area->Init(locaName, "Prohibited_HOTEL.fbx", position, "ȣ��");
			break;
		case Location::AVENUE:
			area->Init(locaName, "Prohibited_AVENUE.fbx", position, "��ȭ��");
			break;
		case Location::HOSPITAL:
			area->Init(locaName, "Prohibited_HOSPITAL.fbx", position, "����");
			break;
		case Location::TEMPLE:
			area->Init(locaName, "Prohibited_TEMPLE.fbx", position, "��");
			break;
		case Location::ARCHERY_RANGE:
			area->Init(locaName, "Prohibited_ARCHERY.fbx", position, "�����");
			break;
		case Location::CEMETERY:
			area->Init(locaName, "Prohibited_CEMETERY.fbx", position, "����");
			break;
		case Location::FOREST:
			area->Init(locaName, "Prohibited_FOREST.fbx", position, "��");
			break;
		case Location::FACTORY:
			area->Init(locaName, "Prohibited_FACTORY.fbx", position, "����");
			break;
		case Location::CHAPEL:
			area->Init(locaName, "Prohibited_CHAPEL.fbx", position, "����");
			break;
		case Location::SCHOOL:
			area->Init(locaName, "Prohibited_SCHOOL.fbx", position, "�б�");
			break;
		case Location::RESERCH_CENTRE:
		{
			area->Init(locaName, "Prohibited_RESERCH_CENTRE.fbx", position, "������");
			break;
		}
		case Location::ESCAPE_DOCK:
			break;
		case Location::MAX:
			break;
		default:
			break;
		}

		if (area->GetLocation() == Location::RESERCH_CENTRE)
		{
			area->SetProhibited(true);
		}
		else
		{
			area->SetProhibited(false);
		}

		prohitivedAreaList_.push_back(area);
	}

	{
		// ��ȭ�� ->
		AreaChangeTrigger* avenue_Forest = CreateActor<AreaChangeTrigger>();
		avenue_Forest->SetLocations(Location::AVENUE, "��ȭ��", Location::FOREST, "��");
		avenue_Forest->SetCollision({ -4817.0f,  0.0f, 6239.0f }, { 1500.0f , 100.0f, 1.0f }, { -4811.0f,  0.0f, 6148.0f }); // -4791, 6194, z 20����

		AreaChangeTrigger* avenue_School = CreateActor<AreaChangeTrigger>();
		avenue_School->SetLocations(Location::AVENUE, "��ȭ��", Location::SCHOOL, "�б�");
		avenue_School->SetCollision({ -5407.0f,  30.0f, 9889.0f }, { 50.0f , 100.0f, 300.0f }, { -5602.0f, 30.0f, 9900.0f }); // -5537, 9909, 300 20

		AreaChangeTrigger* avenue_School1 = CreateActor<AreaChangeTrigger>();
		avenue_School1->SetLocations(Location::AVENUE, "��ȭ��", Location::SCHOOL, "�б�");
		avenue_School1->SetCollision({ -5382.0f,  30.0f, 7764.0f }, { 50.0f , 100.0f, 300.0f }, { -5479.0f, 30.0f, 7743.0f }); // -5537, 9909, 300 20

		AreaChangeTrigger* avenue_Alley0 = CreateActor<AreaChangeTrigger>();
		avenue_Alley0->SetLocations(Location::AVENUE, "��ȭ��", Location::ALLEY, "����");
		avenue_Alley0->SetCollision({ -7859.0f,  0.0f, 11188.0f }, { 1.0f , 100.0f, 1500.0f }, { -7894.0f, 0.0f, 11182.0f });

		AreaChangeTrigger* avenue_Alley1 = CreateActor<AreaChangeTrigger>();
		avenue_Alley1->SetLocations(Location::AVENUE, "��ȭ��", Location::ALLEY, "����");
		avenue_Alley1->SetCollision({ -7697.0f,  0.0f, 14594.0f }, { 1.0f , 100.0f, 800.0f }, { -7745.0f, 0.0f, 14581.0f });

		AreaChangeTrigger* avenue_Alley2 = CreateActor<AreaChangeTrigger>();
		avenue_Alley2->SetLocations(Location::AVENUE, "��ȭ��", Location::ALLEY, "����");
		avenue_Alley2->SetCollision({ -4795.0f,  0.0f, 18029.0f }, { 1000.0f , 100.0f, 1.0f }, { -4802.0f, 0.0f, 18110.0f });

		AreaChangeTrigger* avenue_Temple = CreateActor<AreaChangeTrigger>();
		avenue_Temple->SetLocations(Location::AVENUE, "��ȭ��", Location::TEMPLE, "��");
		avenue_Temple->SetCollision({ -324.0f,  0.0f, 16756.0f }, { 800.0f , 100.0f, 10.0f }, { -317.0f, 0.0f, 16842.0f }); // -349, 16842 z �� 10 ��

		AreaChangeTrigger* avenue_Pond = CreateActor<AreaChangeTrigger>();
		avenue_Pond->SetLocations(Location::AVENUE, "��ȭ��", Location::POND, "����");
		avenue_Pond->SetCollision({ 362.0f,  0.0f, 15892.0f }, { 10.0f , 100.0f, 300.0f }, { 417.0f, 0.0f, 15888.0f }); // ��ġ 397, 15883 ũ�� 300 10 ��

		AreaChangeTrigger* avenue_Pond1 = CreateActor<AreaChangeTrigger>();
		avenue_Pond1->SetLocations(Location::AVENUE, "��ȭ��", Location::POND, "����");
		avenue_Pond1->SetCollision({ 368.0f,  0.0f, 13734.0f }, { 10.0f , 100.0f, 500.0f }, { 422.0f, 0.0f, 13720.0f }); // ��ġ 439 13677 ũ�� 300 10 ��

		AreaChangeTrigger* avenue_Pond2 = CreateActor<AreaChangeTrigger>();
		avenue_Pond2->SetLocations(Location::AVENUE, "��ȭ��", Location::POND, "����");
		avenue_Pond2->SetCollision({ -134.0f,  0.0f, 11177.0f }, { 10.0f , 100.0f, 300.0f }, { -57.0f, 0.0f, 11178.0f }); // ��

		AreaChangeTrigger* avenue_Pond3 = CreateActor<AreaChangeTrigger>();
		avenue_Pond3->SetLocations(Location::AVENUE, "��ȭ��", Location::POND, "����");
		avenue_Pond3->SetCollision({ -153.0f,  0.0f, 9545.0f }, { 1.0f , 100.0f, 1500.0f }, { -75.0f, 0.0f, 9539.0f });

		AreaChangeTrigger* avenue_Research = CreateActor<AreaChangeTrigger>();
		avenue_Research->SetLocations(Location::AVENUE, "��ȭ��", Location::RESERCH_CENTRE, "������");
		avenue_Research->SetCollision({ -884.0f,  0.0f, 8306.0f }, { 500.0f , 300.0f, 10.0f }, { -883.0f, 0.0f, 8250.0f }); // -891 8269 z10 ��
	}

	{
		// �� ->
		AreaChangeTrigger* forest_school = CreateActor<AreaChangeTrigger>();
		forest_school->SetLocations(Location::FOREST, "��", Location::SCHOOL, "�б�");
		forest_school->SetCollision({ -5385.0f,  0.0f, 5659.0f }, { 10.0f , 100.0f, 350.0f }, { -5529.0f, 0.0f, 5684.0f });

		AreaChangeTrigger* forest_school1 = CreateActor<AreaChangeTrigger>();
		forest_school1->SetLocations(Location::FOREST, "��", Location::SCHOOL, "�б�");
		forest_school1->SetCollision({ -5326.0f,  0.0f, 3601.0f }, { 10.0f , 100.0f, 1200.0f }, { -5476.0f, 0.0f, 3596.0f });

		AreaChangeTrigger* forest_hotel = CreateActor<AreaChangeTrigger>();
		forest_hotel->SetLocations(Location::FOREST, "��", Location::HOTEL, "ȣ��");
		forest_hotel->SetCollision({ -5344.0f,  0.0f, 1219.0f }, { 10.0f , 100.0f, 350.0f }, { -5487.0f, 0.0f, 1182.0f });

		AreaChangeTrigger* forest_hotel1 = CreateActor<AreaChangeTrigger>();
		forest_hotel1->SetLocations(Location::FOREST, "��", Location::HOTEL, "ȣ��");
		forest_hotel1->SetCollision({ -5310.0f,  0.0f, -825.0f }, { 10.0f , 100.0f, 1500.0f }, { -5483.0f, 0.0f, -817.0f });

		AreaChangeTrigger* forest_beach = CreateActor<AreaChangeTrigger>();
		forest_beach->SetLocations(Location::FOREST, "��", Location::BEACH, "�𷡻���");
		forest_beach->SetCollision({ -4785.0f,  0.0f, -2099.0f }, { 1500.0f , 100.0f, 10.0f }, { -4795.0f, 0.0f, -2296.0f });

		AreaChangeTrigger* forest_beach1 = CreateActor<AreaChangeTrigger>();
		forest_beach1->SetLocations(Location::FOREST, "��", Location::BEACH, "�𷡻���");
		forest_beach1->SetCollision({ -2498.0f,  0.0f, -2096.0f }, { 500.0f , 100.0f, 10.0f }, { -2503.0f, 0.0f, -2312.0f });

		AreaChangeTrigger* forest_uptown = CreateActor<AreaChangeTrigger>();
		forest_uptown->SetLocations(Location::FOREST, "��", Location::UPTOWN, "��� ���ð�");
		forest_uptown->SetCollision({ 2114.0f,  0.0f, -2106.0f }, { 500.0f , 100.0f, 10.0f }, { 2108.0f, 0.0f, -2316.0f });

		AreaChangeTrigger* forest_chapel = CreateActor<AreaChangeTrigger>();
		forest_chapel->SetLocations(Location::FOREST, "��", Location::CHAPEL, "����");
		forest_chapel->SetCollision({ 3511.0f,  0.0f, 340.0f }, { 10.0f , 100.0f, 700.0f }, { 3678.0f, 0.0f, 340.0f });
	}

	{
		// �б� 
		AreaChangeTrigger* school_alley = CreateActor<AreaChangeTrigger>();
		school_alley->SetLocations(Location::SCHOOL, "�б�", Location::ALLEY, "����");
		school_alley->SetCollision({ -8342.0f,  0.0f, 10357.0f }, { 800.0f , 100.0f, 10.0f }, { -8339.0f, 0.0f, 10533.0f });

		AreaChangeTrigger* school_alley1 = CreateActor<AreaChangeTrigger>();
		school_alley1->SetLocations(Location::SCHOOL, "�б�", Location::ALLEY, "����");
		school_alley1->SetCollision({ -10366.0f,  0.0f, 10290.0f }, { 1200.0f , 100.0f, 10.0f }, { -10390.0f, 0.0f, 10485.0f });

		AreaChangeTrigger* school_archery = CreateActor<AreaChangeTrigger>();
		school_archery->SetLocations(Location::SCHOOL, "�б�", Location::ARCHERY_RANGE, "�����");
		school_archery->SetCollision({ -11731.0f,  0.0f, 9518.0f }, { 10.0f , 100.0f, 200.0f }, { -11872.0f, 0.0f, 9496.0f });

		AreaChangeTrigger* school_archery1 = CreateActor<AreaChangeTrigger>();
		school_archery1->SetLocations(Location::SCHOOL, "�б�", Location::ARCHERY_RANGE, "�����");
		school_archery1->SetCollision({ -10618.0f,  0.0f, 7205.0f }, { 10.0f , 100.0f, 500.0f }, { -10779.0f, 0.0f, 7200.0f });

		AreaChangeTrigger* school_archery2 = CreateActor<AreaChangeTrigger>();
		school_archery2->SetLocations(Location::SCHOOL, "�б�", Location::ARCHERY_RANGE, "�����");
		school_archery2->SetCollision({ -10610.0f,  0.0f, 4077.0f }, { 10.0f , 100.0f, 500.0f }, { -10773.0f, 0.0f, 4075.0f });

		AreaChangeTrigger* school_hotel = CreateActor<AreaChangeTrigger>();
		school_hotel->SetLocations(Location::SCHOOL, "�б�", Location::HOTEL, "ȣ��");
		school_hotel->SetCollision({ -7813.0f,  0.0f, 3173.0f }, { 2000.0f , 100.0f, 10.0f }, { -7813.0f, 0.0f, 3050.0f });
	}

	{
		// �����
		AreaChangeTrigger* archery_alley = CreateActor<AreaChangeTrigger>();
		archery_alley->SetLocations(Location::ARCHERY_RANGE, "�����", Location::ALLEY, "����");
		archery_alley->SetCollision({ -13126.0f,  0.0f, 10329.0f }, { 800.0f , 100.0f, 10.0f }, { -13126.0f, 0.0f, 10568.0f });

		AreaChangeTrigger* archery_hotel = CreateActor<AreaChangeTrigger>();
		archery_hotel->SetLocations(Location::ARCHERY_RANGE, "�����", Location::HOTEL, "ȣ��");
		archery_hotel->SetCollision({ -11899.0f,  0.0f, 3224.0f }, { 500.0f , 100.0f, 10.0f }, { -11899.0f, 0.0f, 3065.0f });
	}

	{
		// ȣ��
		AreaChangeTrigger* hotel_beach = CreateActor<AreaChangeTrigger>();
		hotel_beach->SetLocations(Location::HOTEL, "ȣ��", Location::BEACH, "�𷡻���");
		hotel_beach->SetCollision({ -7675.0f,  0.0f, -3372.0f }, { 600.0f , 100.0f, 10.0f }, { -7675.0f, 0.0f, -3505.0f });

		AreaChangeTrigger* hotel_beach1 = CreateActor<AreaChangeTrigger>();
		hotel_beach1->SetLocations(Location::HOTEL, "ȣ��", Location::BEACH, "�𷡻���");
		hotel_beach1->SetCollision({ -8875.0f,  0.0f, -3980.0f }, { 1000.0f , 100.0f, 10.0f }, { -8875.0f, 0.0f, -4107.0f });

	}

	{
		// �𷡻���
		AreaChangeTrigger* hotel_uptown = CreateActor<AreaChangeTrigger>();
		hotel_uptown->SetLocations(Location::BEACH, "�𷡻���", Location::UPTOWN, "��� ���ð�");
		hotel_uptown->SetCollision({ -1884.0f,  0.0f, -2879.0f }, { 10.0f , 100.0f, 1200.0f }, { -1715.0f, 0.0f, -2879.0f });

		AreaChangeTrigger* hotel_uptown1 = CreateActor<AreaChangeTrigger>();
		hotel_uptown1->SetLocations(Location::BEACH, "�𷡻���", Location::UPTOWN, "��� ���ð�");
		hotel_uptown1->SetCollision({ -1900.0f,  0.0f, -4677.0f }, { 10.0f , 100.0f, 1200.0f }, { -1738.0f, 0.0f, -4677.0f });
	}

	{
		// ��� ���ð�
		AreaChangeTrigger* uptown_chapel = CreateActor<AreaChangeTrigger>();
		uptown_chapel->SetLocations(Location::UPTOWN, "��� ���ð�", Location::CHAPEL, "����");
		uptown_chapel->SetCollision({ 6271.0f,  0.0f, -1614.0f }, { 500.0f , 100.0f, 10.0f }, { 6271.0f, 0.0f, -1406.0f });
	
		AreaChangeTrigger* uptown_dock = CreateActor<AreaChangeTrigger>();
		uptown_dock->SetLocations(Location::UPTOWN, "��� ���ð�", Location::DOCK, "�ױ�");
		uptown_dock->SetCollision({ 6493.0f,  0.0f, -2530.0f }, { 10.0f , 100.0f, 1900.0f }, { 6686.0f, 0.0f, -2530.0f });
	}

	{
		// �ױ�
		AreaChangeTrigger* dock_chapel = CreateActor<AreaChangeTrigger>();
		dock_chapel->SetLocations(Location::DOCK, "�ױ�", Location::CHAPEL, "����");
		dock_chapel->SetCollision({ 9081.0f,  0.0f, -1586.0f }, { 500.0f , 100.0f, 10.0f }, { 9081.0f, 0.0f, -1424.0f });

		AreaChangeTrigger* dock_factory = CreateActor<AreaChangeTrigger>();
		dock_factory->SetLocations(Location::DOCK, "�ױ�", Location::FACTORY, "����");
		dock_factory->SetCollision({ 12666.0f,  0.0f, 416.0f }, { 1700.0f , 100.0f, 10.0f }, { 12666.0f, 0.0f, 639.0f });
	}

	{
		// ����
		AreaChangeTrigger* chapel_factory = CreateActor<AreaChangeTrigger>();
		chapel_factory->SetLocations(Location::CHAPEL, "����", Location::FACTORY, "����");
		chapel_factory->SetCollision({ 11633.0f,  0.0f, 981.0f }, { 10.0f , 100.0f, 500.0f }, { 11872.0f, 0.0f, 981.0f });

		AreaChangeTrigger* chapel_cemetry = CreateActor<AreaChangeTrigger>();
		chapel_cemetry->SetLocations(Location::CHAPEL, "����", Location::CEMETERY, "����");
		chapel_cemetry->SetCollision({ 4502.0f,  0.0f, 2851.0f }, { 2000.0f , 100.0f, 10.0f }, { 4502.0f, 0.0f, 3018.0f });

		AreaChangeTrigger* chapel_cemetry1 = CreateActor<AreaChangeTrigger>();
		chapel_cemetry1->SetLocations(Location::CHAPEL, "����", Location::CEMETERY, "����");
		chapel_cemetry1->SetCollision({ 6279.0f,  0.0f, 2835.0f }, { 2000.0f , 100.0f, 10.0f }, { 6279.0f, 0.0f, 3028.0f });

		AreaChangeTrigger* chapel_cemetry2 = CreateActor<AreaChangeTrigger>();
		chapel_cemetry2->SetLocations(Location::CHAPEL, "����", Location::CEMETERY, "����");
		chapel_cemetry2->SetCollision({ 9257.0f,  0.0f, 3938.0f }, { 2000.0f , 100.0f, 10.0f }, { 9257.0f, 0.0f, 4015.0f });
	}

	{
		// ����
		AreaChangeTrigger* factory_cemetery = CreateActor<AreaChangeTrigger>();
		factory_cemetery->SetLocations(Location::FACTORY, "����", Location::CEMETERY, "����");
		factory_cemetery->SetCollision({ 11891.0f,  0.0f, 5000.0f }, { 10.0f , 100.0f, 5000.0f }, { 11709.0f, 0.0f, 5000.0f });

		AreaChangeTrigger* factory_hospital = CreateActor<AreaChangeTrigger>();
		factory_hospital->SetLocations(Location::FACTORY, "����", Location::HOSPITAL, "����");
		factory_hospital->SetCollision({ 11864.0f,  0.0f, 9216.0f }, { 10.0f , 100.0f, 1000.0f }, { 11750.0f, 0.0f, 9216.0f });
	}

	{
		// ����
		AreaChangeTrigger* cemetery_hospital = CreateActor<AreaChangeTrigger>();
		cemetery_hospital->SetLocations(Location::CEMETERY, "����", Location::HOSPITAL, "����");
		cemetery_hospital->SetCollision({ 9415.0f,  0.0f, 8699.0f }, { 3000.0f , 100.0f, 10.0f }, { 9415.0f, 0.0f, 8901.0f });
	}

	{
		// ����
		AreaChangeTrigger* hospital_pond = CreateActor<AreaChangeTrigger>();
		hospital_pond->SetLocations(Location::HOSPITAL, "����", Location::POND, "����");
		hospital_pond->SetCollision({ 7891.0f,  0.0f, 9260.0f }, { 10.0f , 100.0f, 600.0f }, { 7701.0f, 0.0f, 9260.0f });
		
		AreaChangeTrigger* hospital_pond1 = CreateActor<AreaChangeTrigger>();
		hospital_pond1->SetLocations(Location::HOSPITAL, "����", Location::POND, "����");
		hospital_pond1->SetCollision({ 7276.0f,  0.0f, 15099.0f }, { 10.0f , 100.0f, 1000.0f }, { 7130.0f, 0.0f, 15099.0f });
	}

	{
		// ����
		AreaChangeTrigger* pond_temple = CreateActor<AreaChangeTrigger>();
		pond_temple->SetLocations(Location::POND, "����", Location::TEMPLE, "��");
		pond_temple->SetCollision({ 1267.0f,  0.0f, 16782.0f }, { 500.0f , 100.0f, 10.0f }, { 1267.0f, 0.0f, 16904.0f });

		AreaChangeTrigger* pond_temple1 = CreateActor<AreaChangeTrigger>();
		pond_temple1->SetLocations(Location::POND, "����", Location::TEMPLE, "��");
		pond_temple1->SetCollision({ 5314.0f,  0.0f, 17625.0f }, { 500.0f , 100.0f, 10.0f }, { 5314.0f, 0.0f, 17742.0f });
	}

	{
		// ��
		AreaChangeTrigger* temple_alley = CreateActor<AreaChangeTrigger>();
		temple_alley->SetLocations(Location::TEMPLE, "��", Location::ALLEY, "����");
		temple_alley->SetCollision({ -4233.0f,  0.0f, 18827.0f }, { 10.0f , 100.0f, 800.0f }, { -4385.0f, 0.0f, 18827.0f });
	}

	{
		// ������
		AreaChangeTrigger* research_forest = CreateActor<AreaChangeTrigger>();
		research_forest->SetLocations(Location::RESERCH_CENTRE, "������", Location::FOREST, "��");
		research_forest->SetCollision({ -3606.0f,  0.0f, 5402.0f }, { 10.0f , 300.0f, 800.0f }, { -3702.0f, 0.0f, 5402.0f });

		AreaChangeTrigger* research_forest1 = CreateActor<AreaChangeTrigger>();
		research_forest1->SetLocations(Location::RESERCH_CENTRE, "������", Location::FOREST, "��");
		research_forest1->SetCollision({ -934.0f,  0.0f, 2673.0f }, { 800.0f , 300.0f, 10.0f }, { -934.0f, 0.0f, 2516.0f });

		AreaChangeTrigger* research_forest2 = CreateActor<AreaChangeTrigger>();
		research_forest2->SetLocations(Location::RESERCH_CENTRE, "������", Location::FOREST, "����");
		research_forest2->SetCollision({ 1845.0f,  0.0f, 5384.0f }, { 10.0f , 300.0f, 400.0f }, { 1985.0f, 0.0f, 5384.0f });
	}
}

void LumiaLevel::GameTimeUpdatePacketSend()
{
	GameTimeController* gm = GameTimeController::GetInstance();

	// ��������ð� ����ȭ�� ���� ��Ŷ����(��������)
	GameTimeSyncPacket SendPacket;
	SendPacket.SetCurrentDay(gm->GetCurrentDay());
	SendPacket.SetDayAndNightType(gm->GetCurrentDayType());
	SendPacket.SetGameTime(gm->GetTotalGameTimeToSec());
	SendPacket.SetRemainTime(gm->GetRemainTimeToSec());
	GameServer::GetInstance()->Send(&SendPacket);
}

void LumiaLevel::CharacterStateUpdatePacketSend(float _deltaTime)
{
	GameServer* ServerSocket = GameServer::GetInstance();
	GameClient* ClientSocket = GameClient::GetInstance();

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	if (-1 != pm->GetMyNumber() && 0 < static_cast<int>(CharacterActorList_.size()))
	{
		Character* currentCharacter = CharacterActorList_[pm->GetMyNumber()];

		// ĳ���� �̵����� ��Ŷ
		CharMovementPacket MovePacket;
		MovePacket.SetTargetIndex(pm->GetMyNumber());
		MovePacket.SetPos(currentCharacter->GetTransform()->GetLocalPosition());
		MovePacket.SetDirection(currentCharacter->GetTransform()->GetLocalRotation());

		if (true == ServerSocket->IsOpened())
		{
			ServerSocket->Send(&MovePacket);
		}
		else if (true == ClientSocket->IsConnected())
		{
			ClientSocket->Send(&MovePacket);
		}

		// ĳ���� �ִϸ��̼ǰ��� ��Ŷ
		CharAnimationPacket AnimPacket;
		AnimPacket.SetTargetIndex(pm->GetMyNumber());
		AnimPacket.SetAnimationName(currentCharacter->GetCurAnimation());
		AnimPacket.SetOverrideAnimation(currentCharacter->GetOverrideAnimationName(), currentCharacter->GetOverrideAnimationBoneName());

		static float aniPacketDelay = 0.0f;

		aniPacketDelay += _deltaTime;

		if (aniPacketDelay >= 0.033f)
		{

			if (true == ServerSocket->IsOpened())
			{
				ServerSocket->Send(&AnimPacket);
			}
			else if (true == ClientSocket->IsConnected())
			{
				ClientSocket->Send(&AnimPacket);
			}
			aniPacketDelay = 0.0f;
		}
	}
}

void LumiaLevel::CharactersTransformUpdate()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (0 < static_cast<int>(CharacterActorList_.size()))
	{
		for (int i = 0; i < CharacterActorList_.size(); i++)
		{
			// ���� ��Ŷ�� ������ ���ŵ� playerinfo ���� ����
			// ������ ĳ���� ���͵��� ��ġ ȸ�� �ִϸ��̼� ���� �������ִ� �ڵ����Դϴ�.
			if (i == pm->GetMyNumber())
			{
				continue;
			}

			// 1019 ������
			if (i == CharacterActorList_.size())
			{
				return;
			}

			PlayerInfo& currentInfo = pm->GetPlayerList()[i];

			CharacterActorList_[i]->GetTransform()->SetLocalPosition(currentInfo.curPos_);
			CharacterActorList_[i]->ChangeAnimation(currentInfo.curAnimation_);
			CharacterActorList_[i]->ChangeOverrideAnimation(currentInfo.overrideAnimationName_, currentInfo.overrideAnimationBoneName_);
			CharacterActorList_[i]->GetTransform()->SetWorldRotationDegree(pm->GetPlayerList()[i].curRotation_);
		}
	}
}

void LumiaLevel::MonsterStateUpdatePacketSend()
{
	// ... 

}

void LumiaLevel::MonstersTransformUpdate()
{
	// ...

}

void LumiaLevel::UpdateCharacterVisibility()
{
	Character* focusedCharacter = nullptr;
	for (Character* c : CharacterActorList_)
	{
		if (c->IsFocused())
		{
			focusedCharacter = c;
			break;
		}
	}

	if (nullptr == focusedCharacter)
	{
		return;
	}

	const std::vector<GameEngineVertex> eyesightVertices = focusedCharacter->GetEyesightVertices();
	const std::vector<UINT> eyesightIndices = focusedCharacter->GetEysightIndices();
	float4 focusedCharacterPosition = focusedCharacter->GetTransform()->GetWorldPosition();

	for (Character* c : CharacterActorList_)
	{
		if (c == focusedCharacter)
		{
			continue;
		}

		c->Hide();
		float4 position = c->GetTransform()->GetWorldPosition();

		if (FT::Char::DEFAULT_HIDE_RANGE < float4::Calc_Len3D(position, focusedCharacterPosition))
		{
			continue;
		}

		position.y = FT::Map::MAX_HEIGHT;
		float temp;

		int indexCount = static_cast<int>(eyesightIndices.size()) / 3;
		for (int i = 0; i < indexCount; ++i)
		{
			float4 v1 = eyesightVertices[eyesightIndices[i * 3]].POSITION;
			float4 v2 = eyesightVertices[eyesightIndices[i * 3 + 1]].POSITION;
			float4 v3 = eyesightVertices[eyesightIndices[i * 3 + 2]].POSITION;

			bool bIntersection = DirectX::TriangleTests::Intersects(position.DirectVector, float4::DOWN.DirectVector, 
				v1.DirectVector, v2.DirectVector, v3.DirectVector, temp);

			if (bIntersection)
			{
				c->Show();
				break;
			}
		}
	}

	for (Monsters* m : MonsterActorList_)
	{
		m->Hide();
		float4 position = m->GetTransform()->GetWorldPosition();

		if (FT::Char::DEFAULT_HIDE_RANGE < float4::Calc_Len3D(position, focusedCharacterPosition))
		{
			continue;
		}

		position.y = FT::Map::MAX_HEIGHT;
		float temp;

		int indexCount = static_cast<int>(eyesightIndices.size()) / 3;
		for (int i = 0; i < indexCount; ++i)
		{
			float4 v1 = eyesightVertices[eyesightIndices[i * 3]].POSITION;
			float4 v2 = eyesightVertices[eyesightIndices[i * 3 + 1]].POSITION;
			float4 v3 = eyesightVertices[eyesightIndices[i * 3 + 2]].POSITION;

			bool bIntersection = DirectX::TriangleTests::Intersects(position.DirectVector, float4::DOWN.DirectVector,
				v1.DirectVector, v2.DirectVector, v3.DirectVector, temp);

			if (bIntersection)
			{
				m->Show();
				break;
			}
		}
	}
}

void LumiaLevel::DebugWindowUpdate()
{
	// DebugAndControlWindow
	if (nullptr != DebugAndControlWindow_ && nullptr != MousePointer::InGameMouse && 0 < static_cast<int>(CharacterActorList_.size()))
	{
		GameTimeController* gm = GameTimeController::GetInstance();
		PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

		// InGameMouse Debug Value
		float4 position = MousePointer::InGameMouse->GetIntersectionYAxisPlane(0, 50000.f);
		DebugAndControlWindow_->AddText("MouseX : " + std::to_string(GameEngineInput::GetInst().GetMousePos().x));
		DebugAndControlWindow_->AddText("MouseY : " + std::to_string(GameEngineInput::GetInst().GetMousePos().y));

		DebugAndControlWindow_->AddText("RayCastX : " + std::to_string(position.x));
		DebugAndControlWindow_->AddText("RayCastZ : " + std::to_string(position.z));

		// Player Debug Value
		float4 playerPosition = CharacterActorList_[pm->GetMyNumber()]->GetTransform()->GetWorldPosition();
		float4 camPos = GetMainCameraActor()->GetTransform()->GetWorldPosition();
		float4 playerToCamera = camPos - playerPosition;
		float4 cameraRotation = GetMainCameraActor()->GetTransform()->GetLocalRotation();
		DebugAndControlWindow_->AddText("Player to Camera vector : " + std::to_string(playerToCamera.x) + ", " + std::to_string(playerToCamera.y) + ", " + std::to_string(playerToCamera.z));
		DebugAndControlWindow_->AddText("CameraRotation : " + std::to_string(cameraRotation.x) + ", " + std::to_string(cameraRotation.y) + ", " + std::to_string(cameraRotation.z));

		for (int i = 0; i < CharacterActorList_.size(); i++)
		{
			DebugAndControlWindow_->AddText("Player " + std::to_string(i) + "curHP(Local) : " + std::to_string(CharacterActorList_[i]->GetStat()->HP));
			DebugAndControlWindow_->AddText("Player " + std::to_string(i) + "curHP(Server) : " + std::to_string(pm->GetPlayerList()[i].stat_->HP));

			DebugAndControlWindow_->AddText("Player " + std::to_string(i) + "curSP(Local) : " + std::to_string(CharacterActorList_[i]->GetStat()->SP));
			DebugAndControlWindow_->AddText("Player " + std::to_string(i) + "curSP(Server) : " + std::to_string(pm->GetPlayerList()[i].stat_->SP));
		}
	}

	// MonsterDebugWindow
	if (nullptr != MonsterDebugWindow_ && 0 < static_cast<int>(MonsterActorList_.size()) && 0 < static_cast<int>(CharacterActorList_.size()))
	{
		int MonsterTypeCount = static_cast<int>(MonsterType::MAX);
		for (int MonsterTypeNum = 0; MonsterTypeNum < MonsterTypeCount; ++MonsterTypeNum)
		{
			std::vector<Monsters*>& vecMonsters = MonsterActorByTypeList_[MonsterTypeNum];
			int MonsterCount = static_cast<int>(vecMonsters.size());
			for (int MonsterNum = 0; MonsterNum < MonsterCount; ++MonsterNum)
			{
				// ���͸�(���͹�ȣ + Ÿ�� + ��ġ)
				int Index = vecMonsters[MonsterNum]->GetIndex();
				float4 CurPos = vecMonsters[MonsterNum]->GetTransform()->GetWorldPosition();
				MonsterType Type = vecMonsters[MonsterNum]->GetMonsterType();
				switch (Type)
				{
				case MonsterType::WOLF:
				{
					MonsterDebugWindow_->AddText("Monster" + std::to_string(Index) + "(WOLF) -> " + "Pos< " + "x " + std::to_string(CurPos.x) + " y " + std::to_string(CurPos.y) + " z " + std::to_string(CurPos.z) + +">");
					break;
				}
				case MonsterType::BEAR:
				{
					MonsterDebugWindow_->AddText("Monster" + std::to_string(Index) + "(BEAR) -> " + "Pos< " + "x " + std::to_string(CurPos.x) + " y " + std::to_string(CurPos.y) + " z " + std::to_string(CurPos.z) + +">");
					break;
				}
				case MonsterType::BAT:
				{
					MonsterDebugWindow_->AddText("Monster" + std::to_string(Index) + "(BAT) -> " + "Pos< " + "x " + std::to_string(CurPos.x) + " y " + std::to_string(CurPos.y) + " z " + std::to_string(CurPos.z) + +">");
					break;
				}
				case MonsterType::DOG:
				{
					MonsterDebugWindow_->AddText("Monster" + std::to_string(Index) + "(DOG) -> " + "Pos< " + "x " + std::to_string(CurPos.x) + " y " + std::to_string(CurPos.y) + " z " + std::to_string(CurPos.z) + +">");
					break;
				}
				case MonsterType::CHICKEN:
				{
					MonsterDebugWindow_->AddText("Monster" + std::to_string(Index) + "(CHICKEN) -> " + "Pos< " + "x " + std::to_string(CurPos.x) + " y " + std::to_string(CurPos.y) + " z " + std::to_string(CurPos.z) + +">");
					break;
				}
				case MonsterType::BOAR:
				{
					MonsterDebugWindow_->AddText("Monster" + std::to_string(Index) + "(BOAR) -> " + "Pos< " + "x " + std::to_string(CurPos.x) + " y " + std::to_string(CurPos.y) + " z " + std::to_string(CurPos.z) + +">");
					break;
				}
				}

				// ����ü��
				float CurHp = vecMonsters[MonsterNum]->GetMonsterStateInfo().HP_;
				MonsterDebugWindow_->AddText("HP: " + std::to_string(CurHp));

				// ���緹��
				int CurLevel = vecMonsters[MonsterNum]->GetMonsterStateInfo().Level_;
				MonsterDebugWindow_->AddText("LV: " + std::to_string(CurLevel));

				// �������
				MonsterStateBasicType CurMainState = vecMonsters[MonsterNum]->GetMonsterBasicStateType();
				switch (CurMainState)
				{
				case MonsterStateBasicType::NONE:
				{
					MonsterDebugWindow_->AddText("MainState: NONE");
					break;
				}
				case MonsterStateBasicType::NORMAL:
				{
					MonsterDebugWindow_->AddText("MainState: NORMAL");
					break;
				}
				case MonsterStateBasicType::CROWDCONTROL:
				{
					MonsterDebugWindow_->AddText("MainState: CROWDCONTROL");
					break;
				}
				case MonsterStateBasicType::ATTACK:
				{
					MonsterDebugWindow_->AddText("MainState: ATTACK");
					break;
				}
				}

				MonsterStateType CurDetailState = vecMonsters[MonsterNum]->GetMonsterDetailStateType();
				switch (CurDetailState)
				{
				case MonsterStateType::NONE:
				{
					MonsterDebugWindow_->AddText("DetailState: NONE");
					break;
				}
				case MonsterStateType::APPEAR:
				{
					MonsterDebugWindow_->AddText("DetailState: APPEAR");
					break;
				}
				case MonsterStateType::REGEN:
				{
					MonsterDebugWindow_->AddText("DetailState: REGEN");
					break;
				}
				case MonsterStateType::IDLE:
				{
					MonsterDebugWindow_->AddText("DetailState: IDLE");
					break;
				}
				case MonsterStateType::HOMINGINSTINCT:
				{
					MonsterDebugWindow_->AddText("DetailState: HOMINGINSTINCT");
					break;
				}
				case MonsterStateType::CHASE:
				{
					MonsterDebugWindow_->AddText("DetailState: CHASE");
					break;
				}
				case MonsterStateType::HIT:
				{
					MonsterDebugWindow_->AddText("DetailState: HIT");
					break;
				}
				case MonsterStateType::DEATH:
				{
					MonsterDebugWindow_->AddText("DetailState: DEATH");
					break;
				}
				case MonsterStateType::DEAD:
				{
					MonsterDebugWindow_->AddText("DetailState: DEAD");
					break;
				}
				case MonsterStateType::STUN:
				{
					MonsterDebugWindow_->AddText("DetailState: STUN");
					break;
				}
				case MonsterStateType::KNOCKBACK:
				{
					MonsterDebugWindow_->AddText("DetailState: KNOCKBACK");
					break;
				}
				case MonsterStateType::WALLSLAM:
				{
					MonsterDebugWindow_->AddText("DetailState: WALLSLAM");
					break;
				}
				case MonsterStateType::ATK01:
				{
					MonsterDebugWindow_->AddText("DetailState: ATK01");
					break;
				}
				case MonsterStateType::ATK02:
				{
					MonsterDebugWindow_->AddText("DetailState: ATK02");
					break;
				}
				case MonsterStateType::SKILLATTACK:
				{
					MonsterDebugWindow_->AddText("DetailState: SKILLATTACK");
					break;
				}
				}

				// Ÿ��
				int TargetIndex = vecMonsters[MonsterNum]->GetTargetIndex();
				if (-1 != TargetIndex && TargetIndex < static_cast<int>(CharacterActorList_.size()))
				{
					MonsterDebugWindow_->AddText("CurTarget: Character" + std::to_string(CharacterActorList_[TargetIndex]->GetIndex()));
				}
				else if (-1 == TargetIndex)
				{
					MonsterDebugWindow_->AddText("CurTarget: NoneTarget");
				}

				// ����
				MonsterDebugWindow_->AddText("");
			}

			// ����
			MonsterDebugWindow_->AddText("");
		}
	}
}

void LumiaLevel::CheckLevelRelatedInputKey()
{
	if (true == GameEngineInput::GetInst().Down("MonsterDebugWindowOnOrOff") && nullptr != MonsterDebugWindow_)
	{
		MonsterDebugWindow_->OnOffChange();
	}

	if (GameEngineInput::Up("N") && nullptr != CurMap_)
	{
		if (nullptr != CurMap_)
		{
			CurMap_->ToggleNavMeshRender();
		}
	}

	// ���� ��/�� ��ȯ
	if (true == GameEngineInput::GetInst().Down("ForcedChangeDayAndNight"))
	{
		GameTimeController::GetInstance()->DayAndNightForecedConversion();
	}
}

LumiaLevel::LumiaLevel()
	: CurMap_(nullptr)
	, ItemBoxManager_(nullptr)
	, MainLight_(nullptr)
	, DebugAndControlWindow_(nullptr)
	, MonsterDebugWindow_(nullptr)
	, ItemListWindow_(nullptr)
	, SurvivorCount_(0)
{
}

LumiaLevel::~LumiaLevel()
{
}

void LumiaLevel::LevelStart()
{
	// Create LumiaLevel Input
	CreateLevelInput();

	// 
	GetMainCamera()->DebugOn();
}

void LumiaLevel::LevelUpdate(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	// ���� ��Ŷ ó��
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->ProcessPacket();
	}

	// Ŭ���̾�Ʈ ��Ŷ ó��
	if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->ProcessPacket();
	}

	// ����������Ʈ�ѷ�(������, ��/����ȯ, ����ù����, ... )
	// ��, �����϶��� ó���ϸ� ����(ȣ��Ʈ)�� ��������� ����(�Ѱ����� �����ϱ�����)
	if (true == GameServer::GetInstance()->IsOpened())
	{
		// ������, ��/����ȯ, ����ù������ ���� Ÿ�Ӱ���
		GameTimeController::GetInstance()->HostUpdate(_DeltaTime);

		// ���ӽð� �ǽð� ��Ŷ(����-Ŭ�� ����ȭ)
		GameTimeUpdatePacketSend();
	}

	// ĳ���� ���� ������Ʈ ��Ŷ ����ó��
	CharacterStateUpdatePacketSend(_DeltaTime);

	// ĳ���� ����� Ʈ������ ������ ����
	CharactersTransformUpdate();

	// ���� ���� ������Ʈ ��Ŷ ����ó��
	MonsterStateUpdatePacketSend();

	// ���� ����� Ʈ������ ������ ����
	MonstersTransformUpdate();

	// ����ĳ���� ���� �þ�ó��
	// -> Rendering On/Off Processing
	UpdateCharacterVisibility();

	// Switching FreeCamMode
	if (true == GameEngineInput::GetInst().Down("O"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (true == GameEngineInput::Down("L"))
	{
		if (GetMainCamera()->IsDebugCheck())
		{
			GetMainCamera()->DebugOff();
		}
		else
		{
			GetMainCamera()->DebugOn();
		}
	}

	// MainCamera Adjustment
	if (false == GetMainCameraActor()->IsFreeCameraMode() && -1 != pm->GetMyNumber())
	{
		float4 playerPosition = CharacterActorList_[pm->GetMyNumber()]->GetTransform()->GetWorldPosition();
		GetMainCameraActor()->GetTransform()->SetWorldPosition(playerPosition + float4(400.f, 1280.f, -600.f));
		GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 60.f, -35.f, 0.0f });

		// 221109 SJH ADD : �����÷��̾ ���� �ٴϴ� ����(��) ���� �߰�
		if (nullptr != MainLight_)
		{
			MainLight_->GetTransform()->SetWorldPosition(playerPosition);
			MainLight_->GetTransform()->AddWorldPosition(MainLight_->GetTransform()->GetWorldBackVector() * 500.0f);
			MainLight_->GetTransform()->SetLocalRotationDegree({ 60.0f, 45.0f, 0.0f });
		}
	}

	// Debug Window Update
	DebugWindowUpdate();

	// Check Level Related InputKey
	CheckLevelRelatedInputKey();
}

void LumiaLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	// 220929 ADD SJH : �׽�Ʈ��(���Ļ�������)
	// ������ ������ ���߰ų� Ŭ���̾�Ʈ�� ������ ��������������¿��� LevelControlWindow������ ������ �����̵��� ���
	if (false == GameServer::GetInstance()->IsOpened() && false == GameClient::GetInstance()->IsConnected())
	{
		this->ClearAll();
		Test_releaseRenderWindow();
		Test_releaseResource();

		MainCameraActor_ = CreateActor<CameraActor>();
		UICameraActor_ = CreateActor<CameraActor>();
		UICameraActor_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	}
}

void LumiaLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	// GameController Initalize
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameTimeController::GetInstance()->HostInitialize();
	}

	// LevelUpSystem Initalize
	LevelUpSystem::GetInstance()->LoadLevelInfomationFile();

	// 220929 ADD SJH : �׽�Ʈ��(���Ļ�������)
	// ������ ������ ���߰ų� Ŭ���̾�Ʈ�� ������ ��������������¿��� LevelControlWindow������ ������ �����̵��� ���
	if (false == GameServer::GetInstance()->IsOpened() && false == GameClient::GetInstance()->IsConnected())
	{
		Test_loadResource();
		Test_initRenderWindow();
		Test_createActor();
		Test_adjustCamera();
		Test_serverCheck();

		ItemBoxWindow* Window = nullptr;

		if (nullptr == GameEngineGUI::GetInst()->FindGUIWindow("ItemBoxWindow"))
		{
			Window = GameEngineGUI::GetInst()->CreateGUIWindow<ItemBoxWindow>("ItemBoxWindow");
		}

		if (nullptr != Window)
		{
			Window->ItemBoxManager_ = ItemBoxManager_;
		}

		// 
	}
	else
	{
		// �⺻ ���� ����
		CreateBasicActor();

		// ��Ʈ��ũ �ڵ鷯 ���
		AddSocketHandle();

		// IMGUI Window Find & Setting
		InitIMGUIWindow();

		// MainCamera Adjustment
		CameraAdjustment();

		CreateProhibitedSystem();
	}
}

#pragma region �׽�Ʈ���Լ�(���Ļ�������)
// LevelControlWindow������ ���� �����̵��Ѱ�� ����ϴ� �Լ�
// ���� ������ ���� �ش� ������ ü�����Ѱ��

void LumiaLevel::Test_loadResource()
{
	// ��, �׺���̼� �޽�, �����۹ڽ�
	{
		GameEngineDirectory tempDir;

		tempDir.MoveParent("EternalReturn");
		tempDir / "Resources" / "FBX" / "Map";

		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx")))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
			Mesh->CreateRenderingBuffer();
		}

		tempDir.MoveParent("FBX");
		tempDir / "UserMesh" / "Map";

		std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
			{
				GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecFile[i].GetFullPath());
			}
		}

		tempDir.MoveParent("UserMesh");
		tempDir.MoveChild("ItemBox");

		vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
			{
				GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecFile[i].GetFullPath());
			}
		}
	}

	// ĳ����
	Rio::LoadResource();
	Hyunwoo::LoadResource();
}

void LumiaLevel::Test_initRenderWindow()
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		Window->ClaerRenderTarget();
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("���� ī�޶� Ÿ��", GetMainCamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("UI ī�޶� Ÿ��", GetUICamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("���� ī�޶� G-Buffer", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size);
		Window->PushRenderTarget("���� ī�޶� ���۵� ����Ʈ", GetMainCamera()->GetCameraDeferredLightTarget(), Size);
		Window->PushRenderTarget("���� ī�޶� ���۵� Ÿ��", GetMainCamera()->GetCameraDeferredTarget(), Size);
	}

	DebugAndControlWindow_ = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
}

void LumiaLevel::Test_createActor()
{
	// �ΰ��� ���콺 ����
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	{
		GameEngineDirectory tempDir;

		tempDir.MoveParent("EternalReturn");
		tempDir / "Resources" / "FBX" / "UserMesh" / "ItemBox" / "ItemBoxInfo";

		ItemBoxManager_ = CreateActor<ItemBoxManager>();
		ItemBoxManager_->UserAllLoad(tempDir);
		ItemBoxManager_->GetTransform()->SetLocalScaling(100.0f);
	}

	CurMap_ = CreateActor<LumiaMap>();

	SKySphereActor* Actor = CreateActor<SKySphereActor>();


	{
		LightActor* Actor;

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(1.f);
		Actor->GetLight()->SetAmbientPower(10.f);
		Actor->GetLight()->SetSpacularLightPow(10.f);
	}

	Test_GenerateCharactor();
}

void LumiaLevel::Test_adjustCamera()
{
	GetMainCameraActor()->GetCamera()->SetFov(50.f);
	GetMainCameraActor()->FreeCameraModeSwitch();
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 100.f, -200.f });
}

void LumiaLevel::Test_serverCheck()
{
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer* server = GameServer::GetInstance();
		server->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
		server->AddPacketHandler(ePacketID::CharAnimationPacket, new CharAnimationPacket);
		server->AddPacketHandler(ePacketID::CharStatPacket, new CharStatPacket);
	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient* client = GameClient::GetInstance();
		client->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
		client->AddPacketHandler(ePacketID::CharAnimationPacket, new CharAnimationPacket);
		client->AddPacketHandler(ePacketID::CharStatPacket, new CharStatPacket);
	}
}

void LumiaLevel::Test_releaseRenderWindow()
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		Window->ClaerRenderTarget();
	}
}

void LumiaLevel::Test_releaseResource()
{
	// loadResource���� �ε��� ���ҽ� ����

	// ��, �׺���̼� �޽�, �����۹ڽ�
	GameEngineFBXMeshManager::GetInst().Delete("Bg_NaviMesh.fbx");
	{
		GameEngineDirectory tempDir;
		tempDir.MoveParent("EternalReturn");
		tempDir / "Resources" / "FBX" / "UserMesh" / "Map";

		std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
			{
				GameEngineFBXMeshManager::GetInst().Delete(vecFile[i].FileName());
			}
		}

		tempDir.MoveParent("UserMesh");
		tempDir.MoveChild("ItemBox");

		vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
			{
				GameEngineFBXMeshManager::GetInst().Delete(vecFile[i].FileName());
			}
		}
	}

	// ĳ����
	Rio::ReleaseResource();
	Hyunwoo::ReleaseResource();

	ItemBoxManager_ = nullptr;
}

void LumiaLevel::Test_GenerateCharactor()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	for (int i = 0; i < 2; i++)
	{
		PlayerInfo newPlayer;
		newPlayer.playerNumber_ = i;
		newPlayer.startPoint_ = 0;
		newPlayer.character_ = 6;
		newPlayer.curAnimation_ = "";
		newPlayer.isReady_ = true;

		newPlayer.curRotation_ = float4::ZERO;
		newPlayer.curPos_ = { -2500.f, 0.0f, 10000.f };

		pm->AddNewPlayer(newPlayer);

		Character* newCharacter = CreateActor<Rio>();
		PlayerInfoManager::GetInstance()->GetPlayerList()[i].stat_ = newCharacter->GetStat();
		newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		newCharacter->SetIndex(i);
		CharacterActorList_.push_back(newCharacter);
	}

	CharacterActorList_[0]->Focus();
	pm->SetPlayerNumber(0);
	pm->SetMainCharacter(CharacterActorList_[0]);
}

#pragma endregion
