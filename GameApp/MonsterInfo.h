#pragma once
#include "Enums.h"

struct MonsterInfo
{
public: // 생성정보
	int Index_;										// 몬스터 생성 인덱스(탐색용)
	Location RegionType_;							// 몬스터 스폰 지역 타입(탐색용)
	MonsterType MonsterType_;						// 몬스터 타입
	bool IsGroup_;									// 그룹생성

public: // 초기화정보
	float4 SpawnPosition_;							// 스폰 위치
};

struct MonsterSpawnInfo
{
public:
	std::string area;
	MonsterType type;
	float4 spawnPosition;
};