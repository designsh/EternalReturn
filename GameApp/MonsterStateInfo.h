#pragma once
#include <vector>
#include "Enums.h"

// �з� : ����
// �뵵 : 
// ���� : ������ ��������
struct MonsterStateInfo
{
//====================================== ������ �ִ�ġ(����)
	int LevelMin_;							// ���� ����
	int LevelMax_;							// �ִ� ����
	float HomingInstinctValueMax_;			// �ִ� �ͼҺ��� ��ġ
	float RegenTimeMax_;					// ����Ÿ��
	float SkillCoolDownMax_;				// �ִ� ��ų��Ÿ��

//====================================== ������ ������(����)
	int OffencePowerIncrement_;				// ������ ���ݷ� ������
	int DefenseIncrement_;					// ������ ���� ������
	float MoveSpeedIncrement_;				// ������ �̵��ӵ� ������
	float HPIncrement_;						// ������ ü�� ������

//====================================== �⺻����(����)
	float4 NestPosition_;					// ������ġ(= ������ġ)
	float AttackSpeed_;						// ���ݼӵ�
	float AttackRange_;						// ���� �þ߰Ÿ�
	float DetectRange_;						// ���� �þ߰Ÿ�

//====================================== �⺻����(����)
	int Level_;								// ����(�ʰ� ������ ����(�߻�����)�ϼ��� ������ ���� ����) - ������ ���ÿ� ����(�������� ������ ������ ��� �⺻ 1��������)
	int OffencePower_;						// ���ݷ�
	int Defense_;							// ����
	float HPMax_;							// �ִ� ü��(�������� ����)
	float HP_;								// ����ü��
	float MoveSpeed_;						// �̵��ӵ�
	float HomingInstinctValue_;				// �ͼҺ��� ��ġ(0.0f���� �۰ų��������� ���ʹ� ������ġ�� �̵��ϸ� ü���� ȸ�� -> Ÿ���� DetectRange_�� ��� �ð��� ���� ����)
	float RegenTime_;						// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ

//====================================== ��ų����
	int IsSkill_;							// ��ų����(0: ��ų����)
	float SkillCoolDown_;					// ��ų����ð�

//====================================== �����۰���
	std::map<ItemName, float> DropItems_;	// std::map<�����۸�, Ȯ��> : �����ʱ�ȭ�� ����
	std::vector<ItemName> CurDropItems_;	// ����� ����� �����۸�� : ����� Items_���� Ȯ���� üũ�Ͽ� ����ۼ� -> �������� ������ �� ��Ŷ����
};
