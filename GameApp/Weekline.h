#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : ��Ŭ����(����) ���� ����� ����
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

