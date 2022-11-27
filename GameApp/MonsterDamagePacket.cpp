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
		GameEngineDebug::MsgBoxError("현재게임의 현재레벨을 찾을수없습니다!!!!!");
		return;
	}

	// 예외처리
	if (0 > Index_ || Index_ >= static_cast<int>(PlayLevel->GetMonsterActorList().size()))
	{
		GameEngineDebug::MsgBoxError("잘못된 몬스터에 접근을 시도하였습니다!!!!!");
		return;
	}

	// 수신시 타겟에게 데미지를 입힘
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
