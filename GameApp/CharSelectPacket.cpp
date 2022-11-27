#include "PreCompile.h"
#include "CharSelectPacket.h"

#include "ePacketID.h"

#include "PlayerInfoManager.h"
CharSelectPacket::CharSelectPacket() // default constructer 디폴트 생성자
{

}

CharSelectPacket::~CharSelectPacket() // default destructer 디폴트 소멸자
{

}

CharSelectPacket::CharSelectPacket(CharSelectPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}


void CharSelectPacket::userSerialize()
{
	serializer_ << targetIndex_;
	serializer_ << character_;
}

void CharSelectPacket::userDeserialize()
{
	serializer_ >> targetIndex_;
	serializer_ >> character_;
}

void CharSelectPacket::initPacketID()
{
	SetPacketID(ePacketID::CharSelectPacket);
}

GameEnginePacketBase* CharSelectPacket::getUserObject()
{
	return new CharSelectPacket;
}

void CharSelectPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{

	// 값만 INT 로 반환하고,
	// 이는 클래스에서 ENUM 값으로 캐릭터와 시작 지역을 해석합니다.
	PlayerInfoManager::GetInstance()->GetPlayerList()[targetIndex_].character_ = character_;

	if (_bServer)
	{

		_network->Send(this);
	}
}

void CharSelectPacket::SetTargetIndex(int _index)
{
	targetIndex_ = _index;
}

void CharSelectPacket::SetCharacter(int _character)
{
	character_ = _character;
}

void CharSelectPacket::SetStartPoint(int _startPoint)
{
	startPoint_ = _startPoint;
}
