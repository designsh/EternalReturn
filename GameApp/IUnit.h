#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 유닛베이스
// 용도 : 
// 설명 : 
class IUnit : public GameEngineActor
{
public:
	virtual int GetIndex() = 0;
	//virtual void Damage(float _amount ) = 0;
	virtual void Damage(float _Amount, IUnit* _Target) = 0;			// 221017 SJH ADD : 몬스터는 타겟필요
	virtual void Knockback(float _knockbackTime, float4 _knockbackSpeed) = 0;
	virtual void WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime) = 0;
	virtual void Slow(float _slowTime, float _slowRatio) = 0;
	virtual void Stun(float _stunTime) = 0;

public:
	UnitType UnitType_;
};

