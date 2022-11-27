#pragma once
#include "Monsters.h"

// 분류 : 몬스터
// 용도 : 
// 설명 : 멧돼지(몬스터)의 고유 기능을 제공
class GameEngineFBXRenderer;
class Boar : public Monsters
{
public:
	static bool ResourceLoadFlag;

protected:
	void InitalizeStateInfo() override;
	void InitalizeResourceLoad() override;
	void InitalizeRenderAndAnimation() override;
	void InitalizeCollider() override;

protected:
	void StartSkillAttackProcessing() override;
	void UpdateSkillAttackProcessing(float _DeltaTime) override;
	void EndSkillAttackProcessing() override;

private:
	void HitBoxRangeOn();
	void HitBoxRangeOff();

public:
	Boar();
	~Boar();

protected:
	Boar(const Boar& _other) = delete;
	Boar(Boar&& _other) noexcept = delete;

private:
	Boar& operator=(const Boar& _other) = delete;
	Boar& operator=(const Boar&& _other) = delete;

public:
protected:
private:
	bool SkillAtkReady_;								// 스킬공격준비 처리
	bool SkillAtk_;										// 스킬공격 처리
	float SkillAtk_Range_;								// 스킬공격 사정거리
	float SkillAtk_CastTimeMax_;						// 스킬공격 시전시간 최대치(고정)
	float SkillAtk_CastTime_;							// 스킬공격 시전시간(갱신)
	float SkillAtk_FixedDamage_;						// 스킬공격 고정데미지(고정)
	float SkillAtk_RushSpeed_;							// 스킬공격 돌진속도(고정)

private:
	float4 SkillAtk_DetectTargetPos_;					// 최초 스킬준비시 감지된 타겟의 위치
	float4 SkillAtk_DetectTargetDir_;					// 최초 스킬준비시 감지된 타겟을 향한 방향

private: // HitBoxRange Renderer Related Value
	GameEngineFBXRenderer* HitBox_Frame_;				// 테두리
	GameEngineFBXRenderer* HitBox_Bottom_;				// 바닥
};

