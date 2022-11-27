#include "PreCompile.h"
#include "Lobby_GameStartButton.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "PlayerInfoManager.h"
#include "UserGame.h"
#include "GameServer.h"
#include "GameClient.h"

#include "GameStartPacket.h"

Lobby_GameStartButton* Lobby_GameStartButton::Button = nullptr;

void Lobby_GameStartButton::ButtonNormalFunction()
{
}

void Lobby_GameStartButton::ButtonClickFunction()
{
	// 현재 방에 입장한 모든 플레이어의 준비상태가 준비완료상태이고, 호스트포함 2명이상 참여한 방이라면 호스트측이 로딩레벨로 체인지
	PlayerInfoManager* InfoManager = PlayerInfoManager::GetInstance();
	//if (true == InfoManager->AllPlayerReadyStateCheck() && 2 <= static_cast<int>(InfoManager->GetPlayerList().size()))			// 220928 임시주석처리(추후 사용예정)
	if (true == InfoManager->AllPlayerReadyStateCheck())																			// 로컬테스트로 인한 임시사용
	{
		// 호스트(서버)측에서만 송신
		if (true == GameServer::GetInstance()->IsOpened())
		{
			// 모듵 클라이언트에게 게임시작 패킷 전송
			GameStartPacket Packet;
			GameServer::GetInstance()->Send(&Packet);

			// 테스트용코드(추후삭제예정) - 220929 SJH ADD
			// 서버(호스트) 플레이어만 존재하는 경우 더미 플레이어 생성
			if (1 == static_cast<int>(InfoManager->GetPlayerList().size()))
			{
				PlayerInfo DummyPlayerInfo = {};
				DummyPlayerInfo.playerNumber_ = 1;
				DummyPlayerInfo.startPoint_ = -1;
				DummyPlayerInfo.character_ = static_cast<int>(JobType::DUMMY);
				DummyPlayerInfo.curAnimation_ = "";
				DummyPlayerInfo.isReady_ = 1;
				DummyPlayerInfo.IsLoading_ = 1;

				DummyPlayerInfo.curRotation_ = float4::ZERO;
				DummyPlayerInfo.curPos_ = float4::ZERO;

				InfoManager->AddNewPlayer(DummyPlayerInfo);
			}

			// 서버(호스트) 로딩레벨로 체인지
			UserGame::LevelChange("LoadingLevel");
		}
	}
}

void Lobby_GameStartButton::ButtonOverFunction()
{
}

void Lobby_GameStartButton::Start()
{
	float4 WindowSize = GameEngineWindow::GetInst().GetSize();
	float4 WindowSizeHarf = WindowSize.halffloat4();

	GameEngineTexture* ButtonDefault = GameEngineTextureManager::GetInst().Find("Lobby_GameStart_Btn.png");
	ButtonDefault->Cut(1, 1);

	// 기본 렌더러 생성
	Renderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UI));
	//Renderer_->GetTransform()->SetWorldScaling(float4(281.0f, 47.0f));
	//Renderer_->GetTransform()->SetWorldPosition(float4(WindowSizeHarf.x - 170.f, -WindowSizeHarf.y + 120.f));
	Renderer_->GetTransform()->SetWorldScaling(float4(190.0f, 89.0f));
	Renderer_->GetTransform()->SetWorldPosition(float4(WindowSizeHarf.x - 100.f, -WindowSizeHarf.y + 45.f));

	// 버튼 텍스쳐 등록
	SetButtonState(ButtonState::Normal, "Lobby_GameStart_Btn.png");
	SetButtonState(ButtonState::MouseOver, "Lobby_GameStart_Btn.png");
	SetButtonState(ButtonState::Click, "Lobby_GameStart_Btn.png");
	SetButtonState(ButtonState::Disabled, "Lobby_GameStart_Btn.png");
	Renderer_->SetChangeAnimation("Normal");

	// 버튼 텍스트 등록
	//Renderer_->TextSetting("malgunbd", "게임 시작", 20.f, FW1_CENTER | FW1_VCENTER, float4::WHITE);

	//// 버튼 상태별 사운드 등록
	//MouseOverSoundName_ = "";
	//MouseClickSoundName_ = "";
	//ButtonSound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();

	// 기본 충돌체 생성
	Collider_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(UIRenderOrder::UICOL));
	Collider_->GetTransform()->SetWorldScaling(Renderer_->GetTransform()->GetWorldScaling());
	Collider_->GetTransform()->SetWorldPosition(Renderer_->GetTransform()->GetWorldPosition());
	Collider_->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

	// 버튼 상태 초기화
	CurButtonState_ = ButtonState::Normal;
}

Lobby_GameStartButton::Lobby_GameStartButton()
{
	if (nullptr == Lobby_GameStartButton::Button)
	{
		Lobby_GameStartButton::Button = this;
	}
}

Lobby_GameStartButton::~Lobby_GameStartButton()
{
}
