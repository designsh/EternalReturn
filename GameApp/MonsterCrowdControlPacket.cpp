#include "Precompile.h"
#include "MonsterCrowdControlPacket.h"

#include <GameEngine/GameEngineCore.h>

#include "ePacketID.h"
#include "Enums.h"

#include "LumiaLevel.h"
#include "Character.h"
#include "Monsters.h"

void MonsterCrowdControlPacket::userSerialize()
{
	serializer_ << Index_;
	serializer_ << static_cast<int>(CCType_);
	serializer_ << StunTime_;
	serializer_ << KnockbackTime_;
	serializer_ << KnockbackSpeed_;
}

void MonsterCrowdControlPacket::userDeserialize()
{
	serializer_ >> Index_;

	int CCType = -1;
	serializer_ >> CCType;
	CCType_ = static_cast<eCrowdControlType>(CCType);

	serializer_ >> StunTime_;
	serializer_ >> KnockbackTime_;
	serializer_ >> KnockbackSpeed_;
}

void MonsterCrowdControlPacket::initPacketID()
{
	SetPacketID(static_cast<int>(ePacketID::MonsterCCPacket));
}

GameEnginePacketBase* MonsterCrowdControlPacket::getUserObject()
{
	return new MonsterCrowdControlPacket;
}

void MonsterCrowdControlPacket::execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer)
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
	switch (CCType_)
	{
		case eCrowdControlType::Stun:
		{
			CurMonster->rcvStun(StunTime_);
			break;
		}
		case eCrowdControlType::Knockback:
		{
			CurMonster->rcvKnockback(KnockbackTime_, KnockbackSpeed_);
			break;
		}
		case eCrowdControlType::WallSlam:
		{
			CurMonster->rcvWallSlam(KnockbackTime_, KnockbackSpeed_, StunTime_);
			break;
		}
	}

	if (true == _bServer)
	{
		_network->Send(this);
	}
}

MonsterCrowdControlPacket::MonsterCrowdControlPacket()
	: Index_(-1)
	, CCType_(eCrowdControlType::None)
	, StunTime_(0.0f)
	, KnockbackTime_(0.0f)
	, KnockbackSpeed_(float4::ZERO)
{
}

MonsterCrowdControlPacket::~MonsterCrowdControlPacket()
{
}
