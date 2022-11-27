#include "PreCompile.h"
#include "GameJoinPacket.h"
#include "ePacketID.h"
#include "LobbyLevel.h"

#include "PlayerInfo.h"
// ��ǻ� GameEngineServer ���� ���� ���� / Ŭ���̾�Ʈ Accept �� �� ��ȣ�� �ű�� �� �´ٰ� �����Ѵ�.

GameJoinPacket::GameJoinPacket() // default constructer ����Ʈ ������
{
}

GameJoinPacket::~GameJoinPacket() // default destructer ����Ʈ �Ҹ���
{

}

void GameJoinPacket::SetOtherPlayers(std::vector<PlayerInfo> _others)
{
    othersSize_ = static_cast<int>(_others.size());

    for (size_t i = 0; i < _others.size(); i++)
    {
        otherCharacter_.push_back(_others[i].character_);
        otherStartPoint_.push_back(_others[i].startPoint_);
        otherIsReady_.push_back(_others[i].isReady_);
    }
}

void GameJoinPacket::userSerialize()
{
    serializer_ << othersSize_;
    serializer_.WriteVector(otherCharacter_);
    serializer_.WriteVector(otherStartPoint_);
    serializer_.WriteVector(otherIsReady_);
}

void GameJoinPacket::userDeserialize()
{
    serializer_ >> othersSize_;
    serializer_.ReadVector(otherCharacter_);
    serializer_.ReadVector(otherStartPoint_);
    serializer_.ReadVector(otherIsReady_);
}

void GameJoinPacket::initPacketID()
{
    SetPacketID(ePacketID::GameJoinPacket);
}

GameEnginePacketBase* GameJoinPacket::getUserObject()
{
    return new GameJoinPacket;
}

void GameJoinPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    //if (PlayerInfoManager::GetInstance()->GetMyNumber() == 0)
    //{
    //    // ���� �÷��̾� �ѹ��� �������� ���� �ű� Ŭ���̾�Ʈ�� �� ��Ŷ ��󿡼� ���ܵ˴ϴ�.
    //    return;
    //}

    //PlayerInfo info;
    //info.character_ = -1;
    //info.startPoint_ = -1;
    //info.isReady_ = false;
    //_network->serverPlayerList_.push_back(info);
    //// ��ε�ĳ���� �޴� ���� Ŭ���̾�Ʈ�鿡�Դ� �ű� Ŭ���� ���¸� ������ݴϴ�.

    //if (_bServer)
    //{

    //    _network->Send(this);
    //}
}

