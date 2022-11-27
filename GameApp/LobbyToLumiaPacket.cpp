#include "PreCompile.h"
#include "LobbyToLumiaPacket.h"

#include "ePacketID.h"

#include "UserGame.h"
LobbyToLumiaPacket::LobbyToLumiaPacket() // default constructer 디폴트 생성자
{

}

LobbyToLumiaPacket::~LobbyToLumiaPacket() // default destructer 디폴트 소멸자
{

}

LobbyToLumiaPacket::LobbyToLumiaPacket(LobbyToLumiaPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
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

