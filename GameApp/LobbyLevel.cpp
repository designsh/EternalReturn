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
		// Idle : 아무 입력이 없는 대기실의 첫 상태입니다.
		// 호스트 개설 | 클라이언트로 입장이 확정된 후 캐릭터 선택 상태인 Select 로 넘어갑니다.

		state_.CreateStateTemplate<LobbyLevel>("Select", this, &LobbyLevel::StartSelect, &LobbyLevel::UpdateSelect, &LobbyLevel::EndSelect);
		// Select : 게임 방에 정식으로 입장해 캐릭터와 특성, 시작 위치를 정할 수 있는 상태입니다.
		// 선택을 완료 후 좌하단의 "캐릭터 선택" 버튼을 누르면 Join 상태로 넘어갑니다.

		state_.CreateStateTemplate<LobbyLevel>("Join", this, &LobbyLevel::StartJoin, &LobbyLevel::UpdateJoin, &LobbyLevel::EndJoin);
		// Join : 캐릭터 선택을 마무리하고 다른 플레이어를 기다리는 상태입니다.

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

	// 인게임 마우스 생성 - 220927 EIDT SJH
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

	// 서버가 쓰이는 업데이트는
	// 기본적으로 현 프로세스가 [호스트일 때 / 클라이언트일 때] 를 모두 상정하고, if 문으로 구분해서 코드를 짜셔야 합니다.
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
		//  프로세스를 호스트로 지정 (= 방을 만들겠다)
		GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_btnClick_v1.wav");
		UIController_->GetHostButton()->SetImageByIndex(3);
		UIController_->GetHostButton()->SetText("호스트");

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

		GameEngineDebug::OutPutDebugString("호스트로서 방을 만듭니다.\n");

		std::string nicknameTemp = PlayerInfoManager::GetInstance()->GetNickname();
		PlayerInfoManager::GetInstance()->AddNewPlayer({ 0, -1, -1, -1, 0, nicknameTemp});
		
		// AddNewPlayer() 의 파라미터는 곧 PlayerInfo 의 생성자 파라미터로,
		// 순서대로 {플레이어 번호, 캐릭터, 시작 지역, 준비 상태} 의 이니셜라이즈 값입니다.
	
		PlayerInfoManager::GetInstance()->SetPlayerNumber(0);
		// 해당 플레이어 번호는 현 프로세스의 멤버 변수로도 따로 저장합니다.

		// 220928 SJH ADD
		// 해당 API는 호스트역할을 수행하므로 게임시작 버튼 생성
		// 단, 한개만 생성해야하므로 조건 체크
		if (nullptr == Lobby_GameStartButton::Button)
		{
			GameStartButton_ = CreateActor<Lobby_GameStartButton>();
		}

		state_.ChangeState("Select");
		return;
	}

	// 프로세스를 클라이언트로 지정 (= 방에 참여하겠다)
	if (true == UIController_->GetClientButton()->MouseCollisionCheck() && true == GameEngineInput::GetInst().Down("LBUTTON"))
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_btnClick_v1.wav");
		UIController_->GetClientButton()->SetImageByIndex(3);
		UIController_->GetClientButton()->SetText("클라이언트");

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

		GameEngineDebug::OutPutDebugString("클라이언트로서 방에 참여합니다.\n");
		
		// 바로 플레이어 정보를 저장하는 호스트와는 달리, 클라이언트는 따로 요청 패킷을 보냅니다.
		// 패킷을 보내면 알맞은 플레이어 순번을 지정해 호스트가 다시 지정된 순번을 브로드캐스팅 해 줍니다.
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


	// 호스트 서버일 시
	if (serverSocket_->IsOpened())
	{
		serverSocket_->ProcessPacket();

		// 캐릭터 선택시 패킷 보내는 코드블록
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
					packet.SetTargetIndex(pm->GetMyNumber()); // 나의 플레이어 번호를 알려준다
					packet.SetCharacter(static_cast<int>(UIController_->GetPortraitVector(x)->GetChar())); // 나의 캐릭터 선택을 알려준다
					//packet.SetStartPoint(static_cast<int>(Location::DOCK)); // 나의 스타팅 포인트 지역을 알려준다
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
				packet.SetStartPoint(UIController_->GetMapUI()->GetNodeList()[pm->GetMyNumber()]->GetSelectedLocation()); // 나의 스타팅 포인트 지역을 알려준다
				pm->GetPlayerList()[pm->GetMyNumber()].startPoint_ = UIController_->GetMapUI()->GetNodeList()[pm->GetMyNumber()]->GetSelectedLocation();
				serverSocket_->Send(&packet);
			}
		}

		if (true == UIController_->GetStartButton()->MouseCollisionCheck() && GameEngineInput::GetInst().Down("LBUTTON"))
		{
			if (-1 == pm->GetPlayerList()[pm->GetMyNumber()].character_)
			{
				GameEngineDebug::OutPutDebugString("캐릭터를 선택해주세요!!\n");
				return;
			}


			GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_matchClick2.wav");
			pm->GetPlayerList()[pm->GetMyNumber()].isReady_ = true;

			ReadyPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetReadyStatus(1);
			serverSocket_->Send(&packet);
			GameEngineDebug::OutPutDebugString("호스트가 레디를 박았습니다.\n");
			pm->GetPlayerList()[pm->GetMyNumber()].isReady_ = 1;
			state_.ChangeState("Join");
		}
	}
	else if (clientSocket_->IsConnected())	// 클라이언트일 시
	{
		clientSocket_->ProcessPacket();
		playerCount_ = static_cast<int>(pm->GetPlayerList().size());

		// 캐릭터 선택시 패킷 보내는 코드블록
		for (int x = 0; x < (int)JobType::DUMMY; x++)
		{
			if (UIController_->GetPortraitVector(x)->GetCollision()->Collision(eCollisionGroup::MousePointer))
			{
				if (GameEngineInput::GetInst().Down("LBUTTON"))
				{

					GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_btnClick_v1.wav");

					UIController_->GetPortraitVector(x)->SelectOn();
					CharSelectPacket packet;
					packet.SetTargetIndex(pm->GetMyNumber()); // 나의 플레이어 번호를 알려준다
					packet.SetCharacter(static_cast<int>(UIController_->GetPortraitVector(x)->GetChar())); // 나의 캐릭터 선택을 알려준다
					//packet.SetStartPoint(static_cast<int>(Location::DOCK)); // 나의 스타팅 포인트 지역을 알려준다
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
				packet.SetStartPoint(UIController_->GetMapUI()->GetNodeList()[pm->GetMyNumber()]->GetSelectedLocation()); // 나의 스타팅 포인트 지역을 알려준다
				clientSocket_->Send(&packet);
			}
		}


		if (true == UIController_->GetStartButton()->MouseCollisionCheck() && GameEngineInput::GetInst().Down("LBUTTON"))
		{
			if (-1 == pm->GetPlayerList()[pm->GetMyNumber()].character_)
			{
				GameEngineDebug::OutPutDebugString("캐릭터를 선택해주세요!!\n");
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


	// 유저들의 달라진 캐릭터 선택을 인지해 렌더링을 바꿔 준다.
	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		tempLobbyRenderers_[i]->SetRender(true);

		if (i == 0)
		{
			tempLobbyRenderers_[i]->SetText("호스트\n" + pm->GetPlayerList()[i].playerNickname_);
		}
		else
		{
			tempLobbyRenderers_[i]->SetText("플레이어" + to_string(i) + "\n" + pm->GetPlayerList()[i].playerNickname_);
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
	// 다른 상대의 준비를 기다리거나, 취소 버튼을 눌러 다시 캐릭터 선택으로 돌아가는 단계입니다.
	// 최종 결제권을 가진 호스트가 전체 레디 상황을 확인하고 게임을 시작합니다.
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
			tempLobbyRenderers_[i]->SetText("호스트\n" + pm->GetPlayerList()[i].playerNickname_);
		}
		else
		{
			tempLobbyRenderers_[i]->SetText("플레이어" + to_string(i) + "\n" + pm->GetPlayerList()[i].playerNickname_);
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
