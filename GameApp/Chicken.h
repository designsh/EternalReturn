#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : ��(����)�� ���� ����� ����
class Chicken : public Monsters
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
	Chicken();
	~Chicken();

protected:
	Chicken(const Chicken& _other) = delete;
	Chicken(Chicken&& _other) noexcept = delete;

private:
	Chicken& operator=(const Chicken& _other) = delete;
	Chicken& operator=(const Chicken&& _other) = delete;

public:
protected:
private:
};

