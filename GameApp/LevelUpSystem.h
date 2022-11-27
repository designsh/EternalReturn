#pragma once
#include "Enums.h"
#include "LevelUPData.h"

// 분류 : 시스템
// 용도 : 
// 설명 : 게임시간에 따른 레벨업 처리기능 제공
class LevelUpSystem
{
public: // Static Value & Function
	static LevelUpSystem* GetInstance();

public: // 레벨업관련 파일 로드
	void LoadLevelInfomationFile();

public: // 일괄처리
	void AllUnitLevelUP();

public:
protected:
private:
public:
	LevelUpSystem();
	~LevelUpSystem();

protected:
	LevelUpSystem(const LevelUpSystem& _other) = delete;
	LevelUpSystem(LevelUpSystem&& _other) = delete;

private:
	LevelUpSystem& operator=(const LevelUpSystem& _other) = delete;
	LevelUpSystem& operator=(const LevelUpSystem&& _other) = delete;

public:
protected:
private:
	std::map<JobType, LevelUPData> CharacterLevelData_;										// 캐릭터들의 레벨업 정보목록
	std::map<MonsterType, LevelUPData> MonsterLevelData_;									// 몬스터들의 레벨업 정보목록
};

