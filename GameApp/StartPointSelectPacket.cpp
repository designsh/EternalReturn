#include "PreCompile.h"
#include "StartPointSelectPacket.h"

#include "ePacketID.h"

#include "PlayerInfoManager.h"

StartPointSelectPacket::StartPointSelectPacket() // default constructer ����Ʈ ������
{

}

StartPointSelectPacket::~StartPointSelectPacket() // default destructer ����Ʈ �Ҹ���
{

}

StartPointSelectPacket::StartPointSelectPacket(StartPointSelectPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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

