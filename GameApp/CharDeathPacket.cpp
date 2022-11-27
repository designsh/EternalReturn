#include "PreCompile.h"
#include "CharDeathPacket.h"
#include "LumiaLevel.h"
#include "Enums.h"
#include "Character.h"
#include "ePacketID.h"
#include <GameEngine/GameEngineCore.h>
#include "PlayerUIController.h"

CharDeathPacket::CharDeathPacket() // default constructer ����Ʈ ������
	: targetIndex_(-1)
{

}

CharDeathPacket::~CharDeathPacket() // default destructer ����Ʈ �Ҹ���
{

}

CharDeathPacket::CharDeathPacket(CharDeathPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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
		GameEngineDebug::MsgBoxError("����Ʈ ��Ŷ execute ���� ������ �������� �� �����߽��ϴ�.");
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

