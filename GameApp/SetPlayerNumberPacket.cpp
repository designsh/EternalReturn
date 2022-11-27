#include "PreCompile.h"
#include "SetPlayerNumberPacket.h"
#include "ePacketID.h"

#include "PlayerInfoManager.h"

SetPlayerNumberPacket::SetPlayerNumberPacket()
{

}

SetPlayerNumberPacket::~SetPlayerNumberPacket()
{

}

void SetPlayerNumberPacket::initPacketID()
{
    SetPacketID(ePacketID::SetPlayerNumberPacket);
}

void SetPlayerNumberPacket::userSerialize()
{
    serializer_ << playerNumber_;
}

void SetPlayerNumberPacket::userDeserialize()
{
    serializer_ >> playerNumber_;
}

GameEnginePacketBase* SetPlayerNumberPacket::getUserObject()
{
    return new SetPlayerNumberPacket;
}

void SetPlayerNumberPacket::execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer)
{
    if (!_bServer)
    {
        PlayerInfoManager::GetInstance()->SetPlayerNumber(playerNumber_);
    }
}
