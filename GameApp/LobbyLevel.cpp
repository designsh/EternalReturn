#include "Precompile.h"
#include "LobbyLevel.h"
#include "ePacketID.h"
#include "PlayerNumberPacket.h"
#include "CharSelectPacket.h"
#include "GameJoinPacket.h"
#include "GameJoinPacket2.h"
#include "StartPointSelectPacket.h"
#include "ReadyPacket.h"
#include "Enums.h"

#include "LobbyBackground.h"
#include "Lobby_ButtonOne.h"
#include "Lobby_ButtonTwo.h"
#include "Lobby_PortraitBg.h"
#include "TempLobbyRenderer.h"
#include "UI_TestMouse.h"
#include "Lobby_Map.h"
#include "Lobby_StartButton.h"
#include "Lobby_GameStartButton.h"	
#include "CharFullSelectEffect.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "PlayerInfoManager.h"
#include "SetPlayerNumberPacket.h"
#include "LobbyToLumiaPacket.h"
#include "GameStartPacket.h"

#include "LobbyUIController.h"

#include "UserGame.h"

#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngineBase/GameEngineSoundPlayer.h>

LobbyLevel::LobbyLevel()
	: playerCount_(0)
	, myCharacterSelection_(-1)
	, myStartPointSelection_(-1)
	, myIsReady_(false)
	, UIController_(nullptr)
	, GameStartButton_(nullptr)
	, voicePlayTime_(0.0f)
	, isVoicePlayed_(false)
{

}

LobbyLevel::~LobbyLevel()
{

}

void LobbyLevel::LevelStart()
{
	{
		GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
		//GetMainCameraActor()->GetCamera()->SetFov(60.f);
		GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -400.0f));
	}

	{
		state_.CreateStateTemplate<LobbyLevel>("Idle", this, &LobbyLevel::StartIdle, &LobbyLevel::UpdateIdle, &LobbyLevel::EndIdle);
		// Idle : �ƹ� �Է��� ���� ������ ù �����Դϴ�.
		// ȣ��Ʈ ���� | Ŭ���̾�Ʈ�� ������ Ȯ���� �� ĳ���� ���� ������ Select �� �Ѿ�ϴ�.

		state_.CreateStateTemplate<LobbyLevel>("Select", this, &LobbyLevel::StartSelect, &LobbyLevel::UpdateSelect, &LobbyLevel::EndSelect);
		// Select : ���� �濡 �������� ������ ĳ���Ϳ� Ư��, ���� ��ġ�� ���� �� �ִ� �����Դϴ�.
		// ������ �Ϸ� �� ���ϴ��� "ĳ���� ����" ��ư�� ������ Join ���·� �Ѿ�ϴ�.

		state_.CreateStateTemplate<LobbyLevel>("Join", this, &LobbyLevel::StartJoin, &LobbyLevel::UpdateJoin, &LobbyLevel::EndJoin);
		// Join : ĳ���� ������ �������ϰ� �ٸ� �÷��̾ ��ٸ��� �����Դϴ�.

		state_.ChangeState("Idle");

	}

	if (false == GameEngineInput::GetInst().IsKey("1"))
	{
		GameEngineInput::GetInst().CreateKey("1", '1');
		GameEngineInput::GetInst().CreateKey("2", '2');
		GameEngineInput::GetInst().CreateKey("3", '3');
		GameEngineInput::GetInst().CreateKey("4", '4');
		GameEngineInput::GetInst().CreateKey("5", '5');
		GameEngineInput::GetInst().CreateKey("6", '6');
		GameEngineInput::GetInst().CreateKey("Ready", VK_RETURN);
		GameEngineInput::GetInst().CreateKey("LBUTTON", VK_LBUTTON);

		// 220928 DEL SJH
		//GameEngineInput::GetInst().CreateKey("PassLevel", 'P');
	}

}



void LobbyLevel::LevelUpdate(float _DeltaTime)
{
	state_.Update(_DeltaTime);

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	int a = 0;

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		if (nullptr != MousePointer::InGameMouse)
		{
			float4 mousePosition = GameEngineInput::GetInst().GetMousePos();
			float pointX = mousePosition.x - GameEngineWindow::GetInst().GetSize().halffloat4().x;
			float pointY = -1.0f * (mousePosition.y - GameEngineWindow::GetInst().GetSize().halffloat4().y);

			controlWindow->AddText("X : " + std::to_string(pointX));
			controlWindow->AddText("Y : " + std::to_string(pointY));
		}
	}
}

void LobbyLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	FT::BGMPlayer->Stop();
}

void LobbyLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	FT::BGMPlayer->ChangeSound("BSER_BGM_StrategyMap.wav");
	FT::BGMPlayer->Play();

	// �ΰ��� ���콺 ���� - 220927 EIDT SJH
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	if (tempLobbyRenderers_.size() == 0)
	{
		for (size_t i = 0; i < 6; i++)
		{
			TempLobbyRenderer* renderer_ = CreateActor<TempLobbyRenderer>();
			renderer_->SetPos({ (-500.f + 200.f * i), -320.f, 0.f });
			renderer_->SetScale({ 60.f, 75.f });
			renderer_->SetRender(false);
			tempLobbyRenderers_.emplace_back(renderer_);
		}
	}

	{
		UIController_ = CreateActor<LobbyUIController>();
	}
}



// FSM Method
void LobbyLevel::StartIdle()
{

}

void LobbyLevel::UpdateIdle(float _DeltaTime)
{
	GameServer* serverSocket_ = GameServer::GetInstance();
	GameClient* clientSocket_ = GameClient::GetInstance();

	// ������ ���̴� ������Ʈ��
	// �⺻������ �� ���μ����� [ȣ��Ʈ�� �� / Ŭ���̾�Ʈ�� ��] �� ��� �����ϰ�, if ������ �����ؼ� �ڵ带 ¥�ž� �մϴ�.
	// 

	if (true == UIController_->GetHostButton()->MouseCollisionCheck())
	{
		if (true == GameEngineInput::GetInst().Press("LBUTTON"))
		{
			
			UIController_->GetHostButton()->SetImageByIndex(2);
		}
		else
		{
			UIController_->GetHostButton()->SetImageByIndex(1);
		}
	}
	else if (false == UIController_->GetHostButton()->MouseCollisionCheck())
	{
		UIController_->GetHostButton()->SetImageByIndex(0);

	}

	if (true == UIController_->GetClientButton()->MouseCollisionCheck())
	{
		if (true == GameEngineInput::GetInst().Press("LBUTTON"))
		{

			UIController_->GetClientButton()->SetImageByIndex(2);
		}
		else
		{
			UIController_->GetClientButton()->SetImageByIndex(1);
		}
	}
	else if (false == UIController_->GetHostButton()->MouseCollisionCheck())
	{
		UIController_->GetClientButton()->SetImageByIndex(0);

	}

	if (true == UIController_->GetHostButton()->MouseCollisionCheck() && true == GameEngineInput::GetInst().Up("LBUTTON"))
	{
		//  ���μ����� ȣ��Ʈ�� ���� (= ���� ����ڴ�)
		GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_btnClick_v1.wav");
		UIController_->GetHostButton()->SetImageByIndex(3);
		UIController_->GetHostButton()->SetText("ȣ��Ʈ");

		UIController_->GetClientButton()->Off();

		serverSocket_->Initialize();
		serverSocket_->OpenServer();
		//serverSocket_->AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);				// 221010 SJH DEL
		serverSocket_->AddPacketHandler(ePacketID::LobbyToLumiaPacket, new LobbyToLumiaPacket);
		serverSocket_->AddPacketHandler(ePacketID::GameJoinPacket2, new GameJoinPacket2);
		serverSocket_->AddPacketHandler(ePacketID::SetPlayerNumberPacket, new SetPlayerNumberPacket);
		serverSocket_->AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		serverSocket_->AddPacketHandler(ePacketID::StartPointSelectPacket, new StartPointSelectPacket);
		serverSocket_->AddPacketHandler(ePacketID::ReadyPacket, new ReadyPacket);
		serverSocket_->AddPacketHandler(ePacketID::StartPacket, new GameStartPacket);							// 220928 SJH ADD

		GameEngineDebug::OutPutDebugString("ȣ��Ʈ�μ� ���� ����ϴ�.\n");

		std::string nicknameTemp = PlayerInfoManager::GetInstance()->GetNickname();
		PlayerInfoManager::GetInstance()->AddNewPlayer({ 0, -1, -1, -1, 0, nicknameTemp});
		
		// AddNewPlayer() �� �Ķ���ʹ� �� PlayerInfo �� ������ �Ķ���ͷ�,
		// ������� {�÷��̾� ��ȣ, ĳ����, ���� ����, �غ� ����} �� �̴ϼȶ����� ���Դϴ�.
	
		PlayerInfoManager::GetInstance()->SetPlayerNumber(0);
		// �ش� �÷��̾� ��ȣ�� �� ���μ����� ��� �����ε� ���� �����մϴ�.

		// 220928 SJH ADD
		// �ش� API�� ȣ��Ʈ������ �����ϹǷ� ���ӽ��� ��ư ����
		// ��, �Ѱ��� �����ؾ��ϹǷ� ���� üũ
		if (nullptr == Lobby_GameStartButton::Button)
		{
			GameStartButton_ = CreateActor<Lobby_GameStartButton>();
		}

		state_.ChangeState("Select");
		return;
	}

	// ���μ����� Ŭ���̾�Ʈ�� ���� (= �濡 �����ϰڴ�)
	if (true == UIController_->GetClientButton()->MouseCollisionCheck() && true == GameEngineInput::GetInst().Down("LBUTTON"))
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_btnClick_v1.wav");
		UIController_->GetClientButton()->SetImageByIndex(3);
		UIController_->GetClientButton()->SetText("Ŭ���̾�Ʈ");

		UIController_->GetHostButton()->Off();

		clientSocket_->Initialize();
		clientSocket_->Connect("127.0.0.1");
		//clientSocket_->Connect("123.142.84.195"); // 123.142.84.195
		//clientSocket_->Connect("121.129.74.115");
		//clientSocket_->AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);				// 221010 SJH DEL
		clientSocket_->AddPacketHandler(ePacketID::LobbyToLumiaPacket, new LobbyToLumiaPacket);
		clientSocket_->AddPacketHandler(ePacketID::GameJoinPacket2, new GameJoinPacket2);
		clientSocket_->AddPacketHandler(ePacketID::SetPlayerNumberPacket, new SetPlayerNumberPacket);
		clientSocket_->AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		clientSocket_->AddPacketHandler(ePacketID::StartPointSelectPacket, new StartPointSelectPacket);
		clientSocket_->AddPacketHandler(ePacketID::ReadyPacket, new ReadyPacket);
		clientSocket_->AddPacketHandler(ePacketID::StartPacket, new GameStartPacket);							// 220928 SJH ADD

		GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ�μ� �濡 �����մϴ�.\n");
		
		// �ٷ� �÷��̾� ������ �����ϴ� ȣ��Ʈ�ʹ� �޸�, Ŭ���̾�Ʈ�� ���� ��û ��Ŷ�� �����ϴ�.
		// ��Ŷ�� ������ �˸��� �÷��̾� ������ ������ ȣ��Ʈ�� �ٽ� ������ ������ ��ε�ĳ���� �� �ݴϴ�.
		GameJoinPacket2 packet;

		std::string nicknameTemp = PlayerInfoManager::GetInstance()->GetNickname();
		packet.SetPlayerInfo({ -1, -1, -1, -1, 0, nicknameTemp });
		
		packet.SetListSize(static_cast<int>(PlayerInfoManager::GetInstance()->GetPlayerList().size()));

		clientSocket_->Send(&packet);

		state_.ChangeState("Select");

		return;
	}


}

void LobbyLevel::EndIdle()
{


}

void LobbyLevel::StartSelect()
{
	if (true == GameClient::GetInstance()->IsConnected())
	{

	}

	if (true == GameServer::GetInstance()->IsOpened())
	{
		playerCount_ = static_cast<int>(PlayerInfoManager::GetInstance()->GetPlayerList().size());
	}
	
	UIController_->GetMapUI()->ArrangeCounter();
}

void LobbyLevel::UpdateSelect(float _DeltaTime)
{


	GameServer* serverSocket_ = GameServer::GetInstance();
	GameClient* clientSocket_ = GameClient::GetInstance();
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();


	// ȣ��Ʈ ������ ��
	if (serverSocket_->IsOpened())
	{
		serverSocket_->ProcessPacket();

		// ĳ���� ���ý� ��Ŷ ������ �ڵ���
		for (int x = 0; x < (int)JobType::DUMMY; x++)
		{
			//PortraitBg[x]->GetCollision()->Collision(eCollisionGroup::MousePointer)
			
			if (UIController_->GetPortraitVector(x)->GetCollision()->Collision(eCollisionGroup::MousePointer))
			{
				if (GameEngineInput::GetInst().Down("LBUTTON"))
				{
					GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_btnClick_v1.wav");

					UIController_->GetPortraitVector(x)->SelectOn();
					CharSelectPacket packet;
					packet.SetTargetIndex(pm->GetMyNumber()); // ���� �÷��̾� ��ȣ�� �˷��ش�
					packet.SetCharacter(static_cast<int>(UIController_->GetPortraitVector(x)->GetChar())); // ���� ĳ���� ������ �˷��ش�
					//packet.SetStartPoint(static_cast<int>(Location::DOCK)); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
					serverSocket_->Send(&packet);

					pm->GetPlayerList()[pm->GetMyNumber()].character_ = static_cast<int>(UIController_->GetPortraitVector(x)->GetChar());
					

					for (int o = 0; o < (int)JobType::DUMMY; o++)
					{
						if (o == x)
						{
							continue;
						}
						else
						{
							UIController_->GetPortraitVector(o)->SelectOff();
						}

					}
				}
			}

			if (UIController_->GetMapUI()->GetCollision()->Collision(eCollisionGroup::MousePointer) && GameEngineInput::GetInst().Down("LBUTTON"))
			{
				UIController_->GetMapUI()->GetSelectLocation(GameEngineInput::GetInst().GetMouse3DPos());
				UIController_->GetMapUI()->ResizeMyPin();
				StartPointSelectPacket packet;
				packet.SetStartPoint(UIController_->GetMapUI()->GetNodeList()[pm->GetMyNumber()]->GetSelectedLocation()); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
				pm->GetPlayerList()[pm->GetMyNumber()].startPoint_ = UIController_->GetMapUI()->GetNodeList()[pm->GetMyNumber()]->GetSelectedLocation();
				serverSocket_->Send(&packet);
			}
		}

		if (true == UIController_->GetStartButton()->MouseCollisionCheck() && GameEngineInput::GetInst().Down("LBUTTON"))
		{
			if (-1 == pm->GetPlayerList()[pm->GetMyNumber()].character_)
			{
				GameEngineDebug::OutPutDebugString("ĳ���͸� �������ּ���!!\n");
				return;
			}


			GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_matchClick2.wav");
			pm->GetPlayerList()[pm->GetMyNumber()].isReady_ = true;

			ReadyPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetReadyStatus(1);
			serverSocket_->Send(&packet);
			GameEngineDebug::OutPutDebugString("ȣ��Ʈ�� ���� �ھҽ��ϴ�.\n");
			pm->GetPlayerList()[pm->GetMyNumber()].isReady_ = 1;
			state_.ChangeState("Join");
		}
	}
	else if (clientSocket_->IsConnected())	// Ŭ���̾�Ʈ�� ��
	{
		clientSocket_->ProcessPacket();
		playerCount_ = static_cast<int>(pm->GetPlayerList().size());

		// ĳ���� ���ý� ��Ŷ ������ �ڵ���
		for (int x = 0; x < (int)JobType::DUMMY; x++)
		{
			if (UIController_->GetPortraitVector(x)->GetCollision()->Collision(eCollisionGroup::MousePointer))
			{
				if (GameEngineInput::GetInst().Down("LBUTTON"))
				{

					GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_btnClick_v1.wav");

					UIController_->GetPortraitVector(x)->SelectOn();
					CharSelectPacket packet;
					packet.SetTargetIndex(pm->GetMyNumber()); // ���� �÷��̾� ��ȣ�� �˷��ش�
					packet.SetCharacter(static_cast<int>(UIController_->GetPortraitVector(x)->GetChar())); // ���� ĳ���� ������ �˷��ش�
					//packet.SetStartPoint(static_cast<int>(Location::DOCK)); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
					clientSocket_->Send(&packet);

					for (int o = 0; o < (int)JobType::DUMMY; o++)
					{
						if (o == x)
						{
							continue;
						}
						else
						{
							UIController_->GetPortraitVector(o)->SelectOff();
						}

					}
				}


			}

			if (UIController_->GetMapUI()->GetCollision()->Collision(eCollisionGroup::MousePointer) && GameEngineInput::GetInst().Down("LBUTTON"))
			{
				UIController_->GetMapUI()->GetSelectLocation(GameEngineInput::GetInst().GetMouse3DPos());
				UIController_->GetMapUI()->ResizeMyPin();
				StartPointSelectPacket packet;
				packet.SetTargetIndex(pm->GetMyNumber());
				packet.SetStartPoint(UIController_->GetMapUI()->GetNodeList()[pm->GetMyNumber()]->GetSelectedLocation()); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
				clientSocket_->Send(&packet);
			}
		}


		if (true == UIController_->GetStartButton()->MouseCollisionCheck() && GameEngineInput::GetInst().Down("LBUTTON"))
		{
			if (-1 == pm->GetPlayerList()[pm->GetMyNumber()].character_)
			{
				GameEngineDebug::OutPutDebugString("ĳ���͸� �������ּ���!!\n");
				return;
			}

			GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_matchClick2.wav");
			pm->GetPlayerList()[pm->GetMyNumber()].isReady_ = true;

			ReadyPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetReadyStatus(true);
			clientSocket_->Send(&packet);
			state_.ChangeState("Join");
		}

	}


	// �������� �޶��� ĳ���� ������ ������ �������� �ٲ� �ش�.
	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		tempLobbyRenderers_[i]->SetRender(true);

		if (i == 0)
		{
			tempLobbyRenderers_[i]->SetText("ȣ��Ʈ\n" + pm->GetPlayerList()[i].playerNickname_);
		}
		else
		{
			tempLobbyRenderers_[i]->SetText("�÷��̾�" + to_string(i) + "\n" + pm->GetPlayerList()[i].playerNickname_);
		}

		switch (static_cast<JobType>(pm->GetPlayerList()[i].character_))
		{
		case JobType::NONE:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Anon.png");
			break;
		case JobType::YUKI:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Yuki.png");
			break;
		case JobType::HYUNWOO:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Hyunwoo.png");
			break;
		case JobType::JACKIE:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Jackie.png");
			break;
		case JobType::RIO:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Rio.png");
			break;
		case JobType::AYA:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Aya.png");
			break;
		case JobType::MAX:
			break;
		default:
			break;
		}
	}


		
}

void LobbyLevel::EndSelect()
{
}

void LobbyLevel::StartJoin()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	UIController_->GetCharFullEffect()->SetCharImage(static_cast<JobType>(pm->GetMyPlayer().character_));
	UIController_->GetCharFullEffect()->PlayAwake();


}

void LobbyLevel::UpdateJoin(float _DeltaTime)
{
	// �ٸ� ����� �غ� ��ٸ��ų�, ��� ��ư�� ���� �ٽ� ĳ���� �������� ���ư��� �ܰ��Դϴ�.
	// ���� �������� ���� ȣ��Ʈ�� ��ü ���� ��Ȳ�� Ȯ���ϰ� ������ �����մϴ�.
	GameServer* serverSocket_ = GameServer::GetInstance();
	GameClient* clientSocket_ = GameClient::GetInstance();


		if (true == serverSocket_->IsOpened())
		{
			serverSocket_->ProcessPacket();

			// 220928 DEL SJH
			//if (true == GameEngineInput::GetInst().Down("PassLevel"))
			//{
			//	LobbyToLumiaPacket packet;
			//	serverSocket_->Send(&packet);

			//	UserGame::LevelChange("LumiaLevel");

			//}

		}
		else
		{
			clientSocket_->ProcessPacket();
		}


	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		tempLobbyRenderers_[i]->SetRender(true);
		if (i == 0)
		{
			tempLobbyRenderers_[i]->SetText("ȣ��Ʈ\n" + pm->GetPlayerList()[i].playerNickname_);
		}
		else
		{
			tempLobbyRenderers_[i]->SetText("�÷��̾�" + to_string(i) + "\n" + pm->GetPlayerList()[i].playerNickname_);
		}
		switch (static_cast<JobType>(pm->GetPlayerList()[i].character_))
		{
		case JobType::NONE:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Anon.png");
			break;
		case JobType::YUKI:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Yuki.png");
			break;
		//case JobType::FIORA:
		//	tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Anon.png");
		//	break;
		//case JobType::ZAHIR:
		//	tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Anon.png");
		//	break;
		//case JobType::NADINE:
		//	tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Nadine.png");
		//	break;
		case JobType::HYUNWOO:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Hyunwoo.png");
			break;
		case JobType::JACKIE:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Jackie.png");
			break;
		case JobType::RIO:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Rio.png");
			break;
		case JobType::AYA:
			tempLobbyRenderers_[i]->SetImage("Down_Lobby_Portrait_Aya.png");
			break;
		case JobType::MAX:
			break;
		default:
			break;
		}
	}

	if (false == isVoicePlayed_)
	{
		voicePlayTime_ += _DeltaTime;

		if (1.5f <= voicePlayTime_)
		{
			PlaySelectedCharVoice(static_cast<JobType>(pm->GetMyPlayer().character_));
			voicePlayTime_ = 0.0f;
			isVoicePlayed_ = true;
		}
	}

}

void LobbyLevel::EndJoin()
{


}

void LobbyLevel::Check_PortraitCollision()
{




}

void LobbyLevel::PlaySelectedCharVoice(JobType _type)
{
	switch (_type)
	{
	case JobType::NONE:
		break;
	case JobType::YUKI:
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Yuki_selected_1_ko.wav");
		break;
	//case JobType::FIORA:
	//	break;
	//case JobType::ZAHIR:
	//	break;
	//case JobType::NADINE:
	//	break;
	case JobType::HYUNWOO:
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Hyunwoo_selected_1_ko.wav");
		break;
	case JobType::JACKIE:
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Jackie_selected_1_ko.wav");
		break;
	case JobType::RIO:
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_selected_1_ko.wav");
		break;
	case JobType::AYA:
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Aya_selected_1_ko.wav");
		break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}



}
