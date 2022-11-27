#include "PreCompile.h"
#include "ReadyPacket.h"
#include "ePacketID.h"

#include "PlayerInfoManager.h"

ReadyPacket::ReadyPacket() // default constructer 디폴트 생성자
{

}

ReadyPacket::~ReadyPacket() // default destructer 디폴트 소멸자
{

}

ReadyPacket::ReadyPacket(ReadyPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}


void ReadyPacket::userSerialize()
{
    serializer_ << targetIndex_;
    serializer_ << isReady_;
}

void ReadyPacket::userDeserialize()
{
    serializer_ >> targetIndex_;
    serializer_ >> isReady_;
}

void ReadyPacket::initPacketID()
{
    SetPacketID(ePacketID::ReadyPacket);
}

GameEnginePacketBase* ReadyPacket::getUserObject()
{
    return new ReadyPacket;
}

void ReadyPacket::SetTargetIndex(int _index)
{
    targetIndex_ = _index;
}

void ReadyPacket::SetReadyStatus(int _isReady)
{
    isReady_ = _isReady;
}

void ReadyPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    PlayerInfoManager::GetInstance()->GetPlayerList()[targetIndex_].isReady_ = static_cast<bool>(isReady_);

    if (_bServer)
    {
        GameEngineDebug::OutPutDebugString("플레이어 " + std::to_string(targetIndex_) + " 가 레디를 박았습니다.\n");
        _network->Send(this);
    }
}