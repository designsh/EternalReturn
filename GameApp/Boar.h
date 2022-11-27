#pragma once
#include "Monsters.h"

// �з� : ����
// �뵵 : 
// ���� : �����(����)�� ���� ����� ����
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
	bool SkillAtkReady_;								// ��ų�����غ� ó��
	bool SkillAtk_;										// ��ų���� ó��
	float SkillAtk_Range_;								// ��ų���� �����Ÿ�
	float SkillAtk_CastTimeMax_;						// ��ų���� �����ð� �ִ�ġ(����)
	float SkillAtk_CastTime_;							// ��ų���� �����ð�(����)
	float SkillAtk_FixedDamage_;						// ��ų���� ����������(����)
	float SkillAtk_RushSpeed_;							// ��ų���� �����ӵ�(����)

private:
	float4 SkillAtk_DetectTargetPos_;					// ���� ��ų�غ�� ������ Ÿ���� ��ġ
	float4 SkillAtk_DetectTargetDir_;					// ���� ��ų�غ�� ������ Ÿ���� ���� ����

private: // HitBoxRange Renderer Related Value
	GameEngineFBXRenderer* HitBox_Frame_;				// �׵θ�
	GameEngineFBXRenderer* HitBox_Bottom_;				// �ٴ�
};

