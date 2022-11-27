#include "Precompile.h"
#include "GameStartPacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include "UserGame.h"

void GameStartPacket::userSerialize()
{
    //serializer_ << MapName_;
}

void GameStartPacket::userDeserialize()
{
    //serializer_ >> MapName_;
}

void GameStartPacket::initPacketID()
{
    SetPacketID(ePacketID::StartPacket);
}

GameEnginePacketBase* GameStartPacket::getUserObject()
{
    return new GameStartPacket;
}

void GameStartPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    // ����(ȣ��Ʈ)�������� �۽��ϹǷ� Ŭ���̾�Ʈ(�Խ�Ʈ)�� ����ó��
    if (false == _bServer)
    {
        // �ε������� ü����
        UserGame::LevelChange("LoadingLevel");
    }
}

GameStartPacket::GameStartPacket()
{
}

GameStartPacket::~GameStartPacket()
{
}
