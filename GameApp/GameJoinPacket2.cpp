#include "PreCompile.h"
#include "GameJoinPacket2.h"
#include "ePacketID.h"
#include "LobbyLevel.h"

#include "PlayerInfoManager.h"
#include "SetPlayerNumberPacket.h"
#include <GameEngine/GameEngineCore.h>

// ��ǻ� GameEngineServer ���� ���� ���� / Ŭ���̾�Ʈ Accept �� �� ��ȣ�� �ű�� �� �´ٰ� �����Ѵ�.

GameJoinPacket2::GameJoinPacket2() // default constructer ����Ʈ ������
    : info_({-1, -1, -1, 0})
    , listSize_(-1)
{
}

GameJoinPacket2::~GameJoinPacket2() // default destructer ����Ʈ �Ҹ���
{

}

void GameJoinPacket2::SetPlayerInfo(PlayerInfo _info)
{
    info_ = _info;
}

void GameJoinPacket2::userSerialize()
{
    serializer_ << info_.character_;
    serializer_ << info_.isReady_;
    serializer_ << info_.playerNumber_;
    serializer_ << info_.startPoint_;
    serializer_ << info_.playerNickname_;
    serializer_ << listSize_;
}

void GameJoinPacket2::userDeserialize()
{
    serializer_ >> info_.character_;
    serializer_ >> info_.isReady_;
    serializer_ >> info_.playerNumber_;
    serializer_ >> info_.startPoint_;
    serializer_ >> info_.playerNickname_;
    serializer_ >> listSize_;
}

void GameJoinPacket2::initPacketID()
{
    SetPacketID(ePacketID::GameJoinPacket2);
}

GameEnginePacketBase* GameJoinPacket2::getUserObject()
{
    return new GameJoinPacket2;
}

void GameJoinPacket2::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

    if ("LobbyLevel" == GameEngineCore::CurrentLevel()->GetName())
    {
        if (_bServer)
        {
            // Ŭ���̾�Ʈ�� ���� ����Ʈ ����� 0 �̸� ���� �������� �����Ѵ�.
            if (listSize_ == 0)
            {
                for (size_t i = 0; i < pm->GetPlayerList().size(); i++)
                {
                    GameJoinPacket2 p;
                    p.SetPlayerInfo(pm->GetPlayerList()[i]);
                    _network->Send(_sender, &p);
                }
            }
            else if (listSize_ < 0)
            {
                GameEngineDebug::OutPutDebugString("�߸��� ����Ʈ ����� ���۹޾ҽ��ϴ�.");
                return;
            }

            // �̰� ������ Ŭ���̾�Ʈ�� ������ �� ���̴�.
            info_.playerNumber_ = static_cast<int>(pm->GetPlayerList().size());
            info_.character_ = -1;
            info_.startPoint_ = -1;
            info_.isReady_ = 0;


            // ���ο� Ŭ���̾�Ʈ ������ ������ Ŭ���̾�Ʈ�鿡�� �߰��Ѵ�.
            pm->AddNewPlayer(info_);
            serializer_.OffsetReset();
            _network->Send(this);

            // ������ Ŭ���̾�Ʈ ���ο��Դ� �÷��̾��Ʈ�� �ε����� �ο��Ѵ�.
            SetPlayerNumberPacket packet;
            packet.SetNumber(info_.playerNumber_);
            _network->Send(_sender, &packet);

        }
        else // client
        {
            pm->AddNewPlayer(info_);
        }
    }

   
}

