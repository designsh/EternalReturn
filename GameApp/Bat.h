#pragma once
#include "Monsters.h"

// 분류 : 몬스터
// 용도 : 
// 설명 : 박쥐(몬스터)의 고유 특성을 정의하는 클래스
class Bat : public Monsters
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
	Bat();
	~Bat();

protected:
	Bat(const Bat& _other) = delete;
	Bat(Bat&& _other) noexcept = delete;

private:
	Bat& operator=(const Bat& _other) = delete;
	Bat& operator=(const Bat&& _other) = delete;

public:
protected:
private:
};

