#pragma once
#include "Monsters.h"

// 분류 : 몬스터
// 용도 : 
// 설명 : 곰(몬스터)의 고유 기능을 제공
class Bear : public Monsters
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
public:
	Bear();
	~Bear();

protected:
	Bear(const Bear& _other) = delete;
	Bear(Bear&& _other) noexcept = delete;

private:
	Bear& operator=(const Bear& _other) = delete;
	Bear& operator=(const Bear&& _other) = delete;

public:
protected:
private:
	bool SkillAtk_;										// 스킬공격 처리
	float SkillAtk_Range_;								// 스킬공격 사정거리
	float SkillAtk_CastTimeMax_;						// 스킬공격 시전시간 최대치(고정)
	float SkillAtk_CastTime_;							// 스킬공격 시전시간(갱신)
	float SkillAtk_StunTime_;							// 스킬공격 스턴시간(고정)
	float SkillAtk_FixedDamage_;						// 스킬공격 고정데미지(고정)

private:
	float4 PivotPos_;									// 렌더러의 피벗 조정값
};

