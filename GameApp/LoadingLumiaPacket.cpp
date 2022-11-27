#include "PreCompile.h"
#include "LoadingLumiaPacket.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineCore.h>

LoadingLumiaPacket::LoadingLumiaPacket() // default constructer 디폴트 생성자
{

}

LoadingLumiaPacket::~LoadingLumiaPacket() // default destructer 디폴트 소멸자
{

}

LoadingLumiaPacket::LoadingLumiaPacket(LoadingLumiaPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void LoadingLumiaPacket::userSerialize()
{
}

void LoadingLumiaPacket::userDeserialize()
{
}

void LoadingLumiaPacket::initPacketID()
{
    SetPacketID(ePacketID::LoadingLumiaPacket);
}

GameEnginePacketBase* LoadingLumiaPacket::getUserObject()
{
    return new LoadingLumiaPacket;
}

void LoadingLumiaPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    
    // LumiaLoadingManager 같은 클래스가 여기서 동작해야 할 것...

    
    if (_bServer)
    {
        _network->Send(this);
    }

}
