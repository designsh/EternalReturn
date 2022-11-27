#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : ����(����)�� ���� Ư���� �����ϴ� Ŭ����
class Wolf : public Monsters
{
public:
	static bool ResourceLoadFlag;

public:
	void MusterSkill(Character* _Target);								// ������ų�ߵ�

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
	bool SkillAtk_;										// ��ų�����Ϸ� Flag
	float SkillAtk_Range_;								// ��ų���� �����Ÿ�
	float SkillAtk_CastTime_;							// ��ų���� �����ð�
};

