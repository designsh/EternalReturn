#include "PreCompile.h"
#include "Monsters.h"

void Monsters::StartNormalState()
{
	// 몬스터상태 기본타입 지정
	CurStateBasicType_ = MonsterStateBasicType::NORMAL;

	// 
}

void Monsters::UpdateNormalState(float _DeltaTime)
{
	NormalState_.Update(_DeltaTime);
}

void Monsters::EndNormalState()
{
}

void Monsters::StartCrowdControlState()
{
	// 몬스터상태 기본타입 지정
	CurStateBasicType_ = MonsterStateBasicType::CROWDCONTROL;

	// 
}

void Monsters::UpdateCrowdControlState(float _DeltaTime)
{
	CrowdControlState_.Update(_DeltaTime);
}

void Monsters::EndCrowdControlState()
{
}

void Monsters::StartAttackState()
{
	// 몬스터상태 기본타입 지정
	CurStateBasicType_ = MonsterStateBasicType::ATTACK;

	// 
}

void Monsters::UpdateAttackState(float _DeltaTime)
{
	AttackState_.Update(_DeltaTime);
}

void Monsters::EndAttackState()
{
}