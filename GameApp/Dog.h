#pragma once
#include "Monsters.h"

// 분류 : 몬스터
// 용도 : 
// 설명 : 들개(몬스터)의 고유 기능을 제공
class Dog : public Monsters
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
	Dog();
	~Dog();

protected:		// delete constructer
	Dog(const Dog& _other) = delete;
	Dog(Dog&& _other) noexcept = delete;

private:		//delete operator
	Dog& operator=(const Dog& _other) = delete;
	Dog& operator=(const Dog&& _other) = delete;

public:
protected:
private:
};

