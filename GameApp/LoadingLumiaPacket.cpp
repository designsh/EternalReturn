#include "PreCompile.h"
#include "LoadingLumiaPacket.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineCore.h>

LoadingLumiaPacket::LoadingLumiaPacket() // default constructer ����Ʈ ������
{

}

LoadingLumiaPacket::~LoadingLumiaPacket() // default destructer ����Ʈ �Ҹ���
{

}

LoadingLumiaPacket::LoadingLumiaPacket(LoadingLumiaPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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
    
    // LumiaLoadingManager ���� Ŭ������ ���⼭ �����ؾ� �� ��...

    
    if (_bServer)
    {
        _network->Send(this);
    }

}
