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
    // 서버(호스트)측에서만 송신하므로 클라이언트(게스트)측 수신처리
    if (false == _bServer)
    {
        // 로딩레벨로 체인지
        UserGame::LevelChange("LoadingLevel");
    }
}

GameStartPacket::GameStartPacket()
{
}

GameStartPacket::~GameStartPacket()
{
}
