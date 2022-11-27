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
	//====================================== 스텟의 최대치(고정)
	serializer_ << StatInfo_.LevelMin_;
	serializer_ << StatInfo_.LevelMax_;
	serializer_ << StatInfo_.HomingInstinctValueMax_;
	serializer_ << StatInfo_.RegenTimeMax_;
	serializer_ << StatInfo_.SkillCoolDownMax_;

	//====================================== 레벨당 증가량(고정)
	serializer_ << StatInfo_.OffencePowerIncrement_;
	serializer_ << StatInfo_.DefenseIncrement_;
	serializer_ << StatInfo_.MoveSpeedIncrement_;
	serializer_ << StatInfo_.HPIncrement_;

	//====================================== 기본스텟(고정)
	serializer_ << StatInfo_.NestPosition_;
	serializer_ << StatInfo_.AttackSpeed_;
	serializer_ << StatInfo_.AttackRange_;
	serializer_ << StatInfo_.DetectRange_;

	//====================================== 기본스텟(갱신)
	serializer_ << StatInfo_.Level_;
	serializer_ << StatInfo_.OffencePower_;
	serializer_ << StatInfo_.Defense_;
	serializer_ << StatInfo_.HPMax_;
	serializer_ << StatInfo_.HP_;
	serializer_ << StatInfo_.MoveSpeed_;
	serializer_ << StatInfo_.HomingInstinctValue_;
	serializer_ << StatInfo_.RegenTime_;

	//====================================== 스킬관련
	serializer_ << StatInfo_.IsSkill_;
	serializer_ << StatInfo_.SkillCoolDown_;

	//====================================== 아이템관련
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
	//====================================== 스텟의 최대치(고정)
	serializer_ >> StatInfo_.LevelMin_;
	serializer_ >> StatInfo_.LevelMax_;
	serializer_ >> StatInfo_.HomingInstinctValueMax_;
	serializer_ >> StatInfo_.RegenTimeMax_;
	serializer_ >> StatInfo_.SkillCoolDownMax_;

	//====================================== 레벨당 증가량(고정)
	serializer_ >> StatInfo_.OffencePowerIncrement_;
	serializer_ >> StatInfo_.DefenseIncrement_;
	serializer_ >> StatInfo_.MoveSpeedIncrement_;
	serializer_ >> StatInfo_.HPIncrement_;

	//====================================== 기본스텟(고정)
	serializer_ >> StatInfo_.NestPosition_;
	serializer_ >> StatInfo_.AttackSpeed_;
	serializer_ >> StatInfo_.AttackRange_;
	serializer_ >> StatInfo_.DetectRange_;

	//====================================== 기본스텟(갱신)
	serializer_ >> StatInfo_.Level_;
	serializer_ >> StatInfo_.OffencePower_;
	serializer_ >> StatInfo_.Defense_;
	serializer_ >> StatInfo_.HPMax_;
	serializer_ >> StatInfo_.HP_;
	serializer_ >> StatInfo_.MoveSpeed_;
	serializer_ >> StatInfo_.HomingInstinctValue_;
	serializer_ >> StatInfo_.RegenTime_;

	//====================================== 스킬관련
	serializer_ >> StatInfo_.IsSkill_;
	serializer_ >> StatInfo_.SkillCoolDown_;

	//====================================== 아이템관련
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
		GameEngineDebug::MsgBoxError("현재게임의 현재레벨을 찾을수없습니다!!!!!");
		return;
	}

	// Index_ == -1 이라면 현재 맵에 배치된 해당 타입의 몬스터에게 일괄처리
	// 단, 서버에서만 일괄처리명령을 내리므로 클라일때만 처리
	if (-1 == Index_ && false == _bServer)
	{
		// 루미아레벨에 배치된 몬스터들을 일괄적으로 상태전환
		switch (StateType_)
		{
			case MonsterStateType::APPEAR: // 첫등장상태전환
			{
				PlayLevel->GuestMonsterFirstAppearBatchProcessing(Type_);
				break;
			}
		}
	}
	// 아니라면 특정인덱스를 가지는 몬스터의 처리
	else if(-1 != Index_)
	{
		// 예외처리
		if (Index_ >= static_cast<int>(PlayLevel->GetMonsterActorList().size()))
		{
			GameEngineDebug::MsgBoxError("잘못된 몬스터의 상태전환을 시도하였습니다!!!!!");
			return;
		}

		// 해당 상태로 전환
		Monsters* CurMonster = PlayLevel->GetSpecificMonster(Index_);
		if (nullptr == CurMonster)
		{
			GameEngineDebug::MsgBoxError("여기들어오면 이상한거임!!!!");
			return;
		}

		switch (StateType_)
		{
			case MonsterStateType::ATK01: // 공격01상태전환
			{
				CurMonster->rcvAttack01(StatInfo_);
				break;
			}
			case MonsterStateType::ATK02: // 공격02상태전환
			{
				CurMonster->rcvAttack02(StatInfo_);
				break;
			}
			case MonsterStateType::HIT:	// 피격상태전환
			{
				CurMonster->rcvDamage(StatInfo_, TargetIndex_);
				break;
			}
			case MonsterStateType::DEATH: // 사망중상태전환
			{
				CurMonster->rcvDeath(StatInfo_);
				break;
			}
			case MonsterStateType::DEAD: // 완전사망상태전환
			{
				CurMonster->rcvDead(StatInfo_);
				break;
			}
			case MonsterStateType::REGEN: // 리젠상태전환
			{
				CurMonster->rcvRegen(StatInfo_);
				break;
			}
			case MonsterStateType::HOMINGINSTINCT: // 귀환상태전환
			{
				CurMonster->rcvHomingInstinct(StatInfo_);
				break;
			}
		}

		// 서버일때
		if (true == _bServer)
		{
			// 동기화 패킷 처리
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
