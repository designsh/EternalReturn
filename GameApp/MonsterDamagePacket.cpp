#include "Precompile.h"
#include "MonsterDamagePacket.h"

#include <GameEngine/GameEngineCore.h>

#include "ePacketID.h"
#include "Enums.h"

#include "LumiaLevel.h"
#include "Character.h"
#include "Monsters.h"

void MonsterDamagePacket::userSerialize()
{
	serializer_ << Index_;
}

void MonsterDamagePacket::userDeserialize()
{
	serializer_ >> Index_;
}

void MonsterDamagePacket::initPacketID()
{
	SetPacketID(static_cast<int>(ePacketID::MonsterDamagePacket));
}

GameEnginePacketBase* MonsterDamagePacket::getUserObject()
{
	return new MonsterDamagePacket;
}

void MonsterDamagePacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	// Get Current Level(LumiaLevel Downcasting)
	GameEngineLevel* CurrentLevel = GameEngineCore::CurrentLevel();
	LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurrentLevel);
	if (nullptr == PlayLevel)
	{
		GameEngineDebug::MsgBoxError("��������� ���緹���� ã���������ϴ�!!!!!");
		return;
	}

	// ����ó��
	if (0 > Index_ || Index_ >= static_cast<int>(PlayLevel->GetMonsterActorList().size()))
	{
		GameEngineDebug::MsgBoxError("�߸��� ���Ϳ� ������ �õ��Ͽ����ϴ�!!!!!");
		return;
	}

	// ���Ž� Ÿ�ٿ��� �������� ����
	Monsters* CurMonster = PlayLevel->GetSpecificMonster(Index_);
	CurMonster->rcvDamage();

	if (true == _bServer)
	{
		_network->Send(this);
	}
}

MonsterDamagePacket::MonsterDamagePacket()
	: Index_(-1)
{
}

MonsterDamagePacket::~MonsterDamagePacket()
{
}
