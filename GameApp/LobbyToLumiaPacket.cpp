#include "PreCompile.h"
#include "LobbyToLumiaPacket.h"

#include "ePacketID.h"

#include "UserGame.h"
LobbyToLumiaPacket::LobbyToLumiaPacket() // default constructer ����Ʈ ������
{

}

LobbyToLumiaPacket::~LobbyToLumiaPacket() // default destructer ����Ʈ �Ҹ���
{

}

LobbyToLumiaPacket::LobbyToLumiaPacket(LobbyToLumiaPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}


void LobbyToLumiaPacket::userSerialize()
{

}

void LobbyToLumiaPacket::userDeserialize()
{

}

void LobbyToLumiaPacket::initPacketID()
{
    SetPacketID(ePacketID::LobbyToLumiaPacket);
}

GameEnginePacketBase* LobbyToLumiaPacket::getUserObject()
{
    return new LobbyToLumiaPacket;
}

void LobbyToLumiaPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{

        UserGame::LevelChange("LumiaLevel");

}

