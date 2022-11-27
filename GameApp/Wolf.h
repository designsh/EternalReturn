#pragma once
#include "Monsters.h"

// 분류 : 몬스터
// 용도 : 
// 설명 : 늑대(몬스터)의 고유 특성을 정의하는 클래스
class Wolf : public Monsters
{
public:
	static bool ResourceLoadFlag;

public:
	void MusterSkill(Character* _Target);								// 소집스킬발동

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
	Wolf();
	~Wolf();

protected:
	Wolf(const Wolf& _other) = delete;
	Wolf(Wolf&& _other) = delete;

private:
	Wolf& operator=(const Wolf& _other) = delete;
	Wolf& operator=(const Wolf&& _other) = delete;

public:
protected:
private:
	bool SkillAtk_;										// 스킬시전완료 Flag
	float SkillAtk_Range_;								// 스킬공격 사정거리
	float SkillAtk_CastTime_;							// 스킬공격 시전시간
};

