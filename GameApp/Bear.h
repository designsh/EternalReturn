#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : ��(����)�� ���� ����� ����
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
	bool SkillAtk_;										// ��ų���� ó��
	float SkillAtk_Range_;								// ��ų���� �����Ÿ�
	float SkillAtk_CastTimeMax_;						// ��ų���� �����ð� �ִ�ġ(����)
	float SkillAtk_CastTime_;							// ��ų���� �����ð�(����)
	float SkillAtk_StunTime_;							// ��ų���� ���Ͻð�(����)
	float SkillAtk_FixedDamage_;						// ��ų���� ����������(����)

private:
	float4 PivotPos_;									// �������� �ǹ� ������
};

