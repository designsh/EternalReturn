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
	// ���� �濡 ������ ��� �÷��̾��� �غ���°� �غ�Ϸ�����̰�, ȣ��Ʈ���� 2���̻� ������ ���̶�� ȣ��Ʈ���� �ε������� ü����
	PlayerInfoManager* InfoManager = PlayerInfoManager::GetInstance();
	//if (true == InfoManager->AllPlayerReadyStateCheck() && 2 <= static_cast<int>(InfoManager->GetPlayerList().size()))			// 220928 �ӽ��ּ�ó��(���� ��뿹��)
	if (true == InfoManager->AllPlayerReadyStateCheck())																			// �����׽�Ʈ�� ���� �ӽû��
	{
		// ȣ��Ʈ(����)�������� �۽�
		if (true == GameServer::GetInstance()->IsOpened())
		{
			// ���� Ŭ���̾�Ʈ���� ���ӽ��� ��Ŷ ����
			GameStartPacket Packet;
			GameServer::GetInstance()->Send(&Packet);

			// �׽�Ʈ���ڵ�(���Ļ�������) - 220929 SJH ADD
			// ����(ȣ��Ʈ) �÷��̾ �����ϴ� ��� ���� �÷��̾� ����
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

			// ����(ȣ��Ʈ) �ε������� ü����
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

	// �⺻ ������ ����
	Renderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UI));
	//Renderer_->GetTransform()->SetWorldScaling(float4(281.0f, 47.0f));
	//Renderer_->GetTransform()->SetWorldPosition(float4(WindowSizeHarf.x - 170.f, -WindowSizeHarf.y + 120.f));
	Renderer_->GetTransform()->SetWorldScaling(float4(190.0f, 89.0f));
	Renderer_->GetTransform()->SetWorldPosition(float4(WindowSizeHarf.x - 100.f, -WindowSizeHarf.y + 45.f));

	// ��ư �ؽ��� ���
	SetButtonState(ButtonState::Normal, "Lobby_GameStart_Btn.png");
	SetButtonState(ButtonState::MouseOver, "Lobby_GameStart_Btn.png");
	SetButtonState(ButtonState::Click, "Lobby_GameStart_Btn.png");
	SetButtonState(ButtonState::Disabled, "Lobby_GameStart_Btn.png");
	Renderer_->SetChangeAnimation("Normal");

	// ��ư �ؽ�Ʈ ���
	//Renderer_->TextSetting("malgunbd", "���� ����", 20.f, FW1_CENTER | FW1_VCENTER, float4::WHITE);

	//// ��ư ���º� ���� ���
	//MouseOverSoundName_ = "";
	//MouseClickSoundName_ = "";
	//ButtonSound_ = GameEngineSoundManager::GetInst().CreateSoundPlayer();

	// �⺻ �浹ü ����
	Collider_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(UIRenderOrder::UICOL));
	Collider_->GetTransform()->SetWorldScaling(Renderer_->GetTransform()->GetWorldScaling());
	Collider_->GetTransform()->SetWorldPosition(Renderer_->GetTransform()->GetWorldPosition());
	Collider_->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

	// ��ư ���� �ʱ�ȭ
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
