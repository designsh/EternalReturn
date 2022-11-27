#pragma once
#include "Monsters.h"

// 분류 : 몬스터
// 용도 : 
// 설명 : 위클라인(몬스터) 고유 기능을 제공
class Weekline : public Monsters
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
	Weekline();
	~Weekline();

protected:		// delete constructer
	Weekline(const Weekline& _other) = delete;
	Weekline(Weekline&& _other) noexcept = delete;

private:		//delete operator
	Weekline& operator=(const Weekline& _other) = delete;
	Weekline& operator=(const Weekline&& _other) = delete;

public:
protected:
private:
};

