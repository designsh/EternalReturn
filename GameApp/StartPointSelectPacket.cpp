#include "PreCompile.h"
#include "StartPointSelectPacket.h"

#include "ePacketID.h"

#include "PlayerInfoManager.h"

StartPointSelectPacket::StartPointSelectPacket() // default constructer 디폴트 생성자
{

}

StartPointSelectPacket::~StartPointSelectPacket() // default destructer 디폴트 소멸자
{

}

StartPointSelectPacket::StartPointSelectPacket(StartPointSelectPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void StartPointSelectPacket::SetStartPoint(int _startPoint)
{
    startPoint_ = _startPoint;
}

void StartPointSelectPacket::SetTargetIndex(int _index)
{
	targetIndex_ = _index;
}

void StartPointSelectPacket::userSerialize()
{
	serializer_ << targetIndex_;
	serializer_ << startPoint_;
}

void StartPointSelectPacket::userDeserialize()
{
	serializer_ >> targetIndex_;
	serializer_ >> startPoint_;
}

void StartPointSelectPacket::initPacketID()
{
	SetPacketID(ePacketID::StartPointSelectPacket);
}

GameEnginePacketBase* StartPointSelectPacket::getUserObject()
{
    return new StartPointSelectPacket;
}

void StartPointSelectPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	PlayerInfoManager::GetInstance()->GetPlayerList()[targetIndex_].startPoint_ = startPoint_;

	if (_bServer)
	{
		_network->Send(this);
	}
}

