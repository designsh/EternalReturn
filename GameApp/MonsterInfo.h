#pragma once
#include "Enums.h"

struct MonsterInfo
{
public: // ��������
	int Index_;										// ���� ���� �ε���(Ž����)
	Location RegionType_;							// ���� ���� ���� Ÿ��(Ž����)
	MonsterType MonsterType_;						// ���� Ÿ��
	bool IsGroup_;									// �׷����

public: // �ʱ�ȭ����
	float4 SpawnPosition_;							// ���� ��ġ
};

struct MonsterSpawnInfo
{
public:
	std::string area;
	MonsterType type;
	float4 spawnPosition;
};