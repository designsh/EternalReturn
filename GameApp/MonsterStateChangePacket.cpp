#include "Precompile.h"
#include "MonsterStateChangePacket.h"

#include <GameEngine/GameEngineCore.h>

#include "ePacketID.h"
#include "Enums.h"

#include "LumiaLevel.h"
#include "Character.h"
#include "Monsters.h"

void MonsterStateChangePacket::userSerialize()
{
	// Monster Distinction Related
	serializer_ << Index_;
	serializer_ << static_cast<int>(Type_);
	serializer_ << static_cast<int>(StateType_);

	// Monster Stat Update Related
	//====================================== ������ �ִ�ġ(����)
	serializer_ << StatInfo_.LevelMin_;
	serializer_ << StatInfo_.LevelMax_;
	serializer_ << StatInfo_.HomingInstinctValueMax_;
	serializer_ << StatInfo_.RegenTimeMax_;
	serializer_ << StatInfo_.SkillCoolDownMax_;

	//====================================== ������ ������(����)
	serializer_ << StatInfo_.OffencePowerIncrement_;
	serializer_ << StatInfo_.DefenseIncrement_;
	serializer_ << StatInfo_.MoveSpeedIncrement_;
	serializer_ << StatInfo_.HPIncrement_;

	//====================================== �⺻����(����)
	serializer_ << StatInfo_.NestPosition_;
	serializer_ << StatInfo_.AttackSpeed_;
	serializer_ << StatInfo_.AttackRange_;
	serializer_ << StatInfo_.DetectRange_;

	//====================================== �⺻����(����)
	serializer_ << StatInfo_.Level_;
	serializer_ << StatInfo_.OffencePower_;
	serializer_ << StatInfo_.Defense_;
	serializer_ << StatInfo_.HPMax_;
	serializer_ << StatInfo_.HP_;
	serializer_ << StatInfo_.MoveSpeed_;
	serializer_ << StatInfo_.HomingInstinctValue_;
	serializer_ << StatInfo_.RegenTime_;

	//====================================== ��ų����
	serializer_ << StatInfo_.IsSkill_;
	serializer_ << StatInfo_.SkillCoolDown_;

	//====================================== �����۰���
	int DropItemCount = static_cast<int>(StatInfo_.CurDropItems_.size());
	serializer_ << DropItemCount;
	for (int ItemNum = 0; ItemNum < DropItemCount; ++ItemNum)
	{
		int ItemNameType = static_cast<int>(StatInfo_.CurDropItems_[ItemNum]);
		serializer_ << ItemNameType;
	}

	// Target(Character) Related
	serializer_ << TargetIndex_;
}

void MonsterStateChangePacket::userDeserialize()
{
	// Monster Related
	serializer_ >> Index_;

	int rcvType = 0;
	serializer_ >> rcvType;
	Type_ = static_cast<MonsterType>(rcvType);

	int rcvStateType = 0;
	serializer_ >> rcvStateType;
	StateType_ = static_cast<MonsterStateType>(rcvStateType);

	// Monster Stat Update Related
	//====================================== ������ �ִ�ġ(����)
	serializer_ >> StatInfo_.LevelMin_;
	serializer_ >> StatInfo_.LevelMax_;
	serializer_ >> StatInfo_.HomingInstinctValueMax_;
	serializer_ >> StatInfo_.RegenTimeMax_;
	serializer_ >> StatInfo_.SkillCoolDownMax_;

	//====================================== ������ ������(����)
	serializer_ >> StatInfo_.OffencePowerIncrement_;
	serializer_ >> StatInfo_.DefenseIncrement_;
	serializer_ >> StatInfo_.MoveSpeedIncrement_;
	serializer_ >> StatInfo_.HPIncrement_;

	//====================================== �⺻����(����)
	serializer_ >> StatInfo_.NestPosition_;
	serializer_ >> StatInfo_.AttackSpeed_;
	serializer_ >> StatInfo_.AttackRange_;
	serializer_ >> StatInfo_.DetectRange_;

	//====================================== �⺻����(����)
	serializer_ >> StatInfo_.Level_;
	serializer_ >> StatInfo_.OffencePower_;
	serializer_ >> StatInfo_.Defense_;
	serializer_ >> StatInfo_.HPMax_;
	serializer_ >> StatInfo_.HP_;
	serializer_ >> StatInfo_.MoveSpeed_;
	serializer_ >> StatInfo_.HomingInstinctValue_;
	serializer_ >> StatInfo_.RegenTime_;

	//====================================== ��ų����
	serializer_ >> StatInfo_.IsSkill_;
	serializer_ >> StatInfo_.SkillCoolDown_;

	//====================================== �����۰���
	int DropItemCount = 0;
	serializer_ >> DropItemCount;
	for (int ItemNum = 0; ItemNum < DropItemCount; ++ItemNum)
	{
		int ItemNameType = -1;
		serializer_ >> ItemNameType;
		StatInfo_.CurDropItems_.push_back(static_cast<ItemName>(ItemNameType));
	}

	// Target(Character) Related
	serializer_ >> TargetIndex_;
}

void MonsterStateChangePacket::initPacketID()
{
	SetPacketID(ePacketID::MonsterStatePacket);
}

GameEnginePacketBase* MonsterStateChangePacket::getUserObject()
{
	return new MonsterStateChangePacket;
}

void MonsterStateChangePacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	// Get Current Level(LumiaLevel Downcasting)
	GameEngineLevel* CurrentLevel = GameEngineCore::CurrentLevel();
	LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurrentLevel);
	if (nullptr == PlayLevel)
	{
		GameEngineDebug::MsgBoxError("��������� ���緹���� ã���������ϴ�!!!!!");
		return;
	}

	// Index_ == -1 �̶�� ���� �ʿ� ��ġ�� �ش� Ÿ���� ���Ϳ��� �ϰ�ó��
	// ��, ���������� �ϰ�ó������� �����Ƿ� Ŭ���϶��� ó��
	if (-1 == Index_ && false == _bServer)
	{
		// ��̾Ʒ����� ��ġ�� ���͵��� �ϰ������� ������ȯ
		switch (StateType_)
		{
			case MonsterStateType::APPEAR: // ù���������ȯ
			{
				PlayLevel->GuestMonsterFirstAppearBatchProcessing(Type_);
				break;
			}
		}
	}
	// �ƴ϶�� Ư���ε����� ������ ������ ó��
	else if(-1 != Index_)
	{
		// ����ó��
		if (Index_ >= static_cast<int>(PlayLevel->GetMonsterActorList().size()))
		{
			GameEngineDebug::MsgBoxError("�߸��� ������ ������ȯ�� �õ��Ͽ����ϴ�!!!!!");
			return;
		}

		// �ش� ���·� ��ȯ
		Monsters* CurMonster = PlayLevel->GetSpecificMonster(Index_);
		if (nullptr == CurMonster)
		{
			GameEngineDebug::MsgBoxError("��������� �̻��Ѱ���!!!!");
			return;
		}

		switch (StateType_)
		{
			case MonsterStateType::ATK01: // ����01������ȯ
			{
				CurMonster->rcvAttack01(StatInfo_);
				break;
			}
			case MonsterStateType::ATK02: // ����02������ȯ
			{
				CurMonster->rcvAttack02(StatInfo_);
				break;
			}
			case MonsterStateType::HIT:	// �ǰݻ�����ȯ
			{
				CurMonster->rcvDamage(StatInfo_, TargetIndex_);
				break;
			}
			case MonsterStateType::DEATH: // ����߻�����ȯ
			{
				CurMonster->rcvDeath(StatInfo_);
				break;
			}
			case MonsterStateType::DEAD: // �������������ȯ
			{
				CurMonster->rcvDead(StatInfo_);
				break;
			}
			case MonsterStateType::REGEN: // ����������ȯ
			{
				CurMonster->rcvRegen(StatInfo_);
				break;
			}
			case MonsterStateType::HOMINGINSTINCT: // ��ȯ������ȯ
			{
				CurMonster->rcvHomingInstinct(StatInfo_);
				break;
			}
		}

		// �����϶�
		if (true == _bServer)
		{
			// ����ȭ ��Ŷ ó��
			_network->Send(this);
		}
	}
}

MonsterStateChangePacket::MonsterStateChangePacket()
	: Index_(-1)
	, Type_(MonsterType::NONE)
	, StateType_(MonsterStateType::NONE)
	, StatInfo_{}
	, TargetIndex_(-1)
{
}

MonsterStateChangePacket::~MonsterStateChangePacket()
{
}
