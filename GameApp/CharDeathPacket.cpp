#include "PreCompile.h"
#include "CharDeathPacket.h"
#include "LumiaLevel.h"
#include "Enums.h"
#include "Character.h"
#include "ePacketID.h"
#include <GameEngine/GameEngineCore.h>
#include "PlayerUIController.h"

CharDeathPacket::CharDeathPacket() // default constructer 디폴트 생성자
	: targetIndex_(-1)
{

}

CharDeathPacket::~CharDeathPacket() // default destructer 디폴트 소멸자
{

}

CharDeathPacket::CharDeathPacket(CharDeathPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void CharDeathPacket::SetTargetIndex(int _index)
{
	targetIndex_ = _index;
}

void CharDeathPacket::SetFraggerType(int _index)
{
	fraggerType_ = _index;
}

void CharDeathPacket::SetFraggerIndex(int _index)
{
	fraggerIndex_ = _index;
}

void CharDeathPacket::userSerialize()
{
	serializer_ << targetIndex_;
	serializer_ << fraggerType_;
	serializer_ << fraggerIndex_;
}

void CharDeathPacket::userDeserialize()
{
	serializer_ >> targetIndex_;
	serializer_ >> fraggerType_;
	serializer_ >> fraggerIndex_;
}

void CharDeathPacket::initPacketID()
{
    SetPacketID(ePacketID::CharDeathPacket);
}

GameEnginePacketBase* CharDeathPacket::getUserObject()
{
	return new CharDeathPacket;
}

void CharDeathPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	
	LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());

	if (nullptr == level)
	{
		GameEngineDebug::MsgBoxError("이팩트 패킷 execute 도중 레벨을 가져오는 데 실패했습니다.");
		return;
	}

	Character* targetChar = level->GetCharacterActorList()[targetIndex_];

	targetChar->SetFraggerIndex(fraggerIndex_);
	targetChar->SetCharacterDeath();

	if (_bServer)
	{
		_network->Send(this);
	}
}

