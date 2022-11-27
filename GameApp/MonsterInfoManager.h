#pragma once
#include "MonsterInfo.h"
#include <set>

// 지역별 야생동물참고정보
struct RefInfoByRegion
{
public: // 기본정보관련
	Location RegionType_;																	// 지역타입
	int TotalMonsterCnt_;																	// 지역별 최대몬스터 스폰수

public: // 위치정보관련(스폰위치생성용) - 해당 지역에 해당하는 버텍스를 모두알고있는다.
	std::string FullSpawnFilePath_;															// 해당 지역의 메쉬파일경로
	GameEngineFBXMesh* SpawnPointMesh_;														// 좌표목록메쉬(네비메쉬활용: 스폰위치지정용)

public: // 개체수관련(생성하려는몬스터개체수 지정용)
	int MonsterCount_[static_cast<int>(MonsterType::MAX)];									// 몬스터(야생동물) 타입별 개체수
};

// 분류 : Manager
// 용도 : 
// 설명 : 현재 맵에 배치된 모든 몬스터의 정보를 관리하는 관리자
class MonsterInfoManager
{
public:
	static MonsterInfoManager* GetInstance();
	static bool FirstCreationPacketFlag;
	static bool SecondCreationPacketFlag;

public: // Inline Get Function
	inline int GetCurMonsterListSize()
	{
		return static_cast<int>(AllMonsters_.size());
	}

	inline std::vector<MonsterInfo>& GetAllMonsterListRef()
	{
		return AllMonsters_;
	}

	inline std::vector<MonsterInfo> GetAllMonsterListValue()
	{
		return AllMonsters_;
	}

public: // Inline Set Function

public: // Public MonsterBasicInfo Add & Set Function
	void AddMonsterInfo(MonsterInfo _MonsterInfo);
	void SetMonsterInfos(std::vector<MonsterInfo> _MonsterInfos);

public: // 
	bool CreatMonsterInfomation();															// 현재맵에 배치하려는 몬스터의 기본정보를 생성

public: // 외부호출
	void LoadMonsterInfoFile();																// 클라전용

protected:

private: // 이미 등록된 몬스터정보 존재시 호출
	void LoadMonsterInfoFile(const std::string& _FullPath);									// 지정된경로에 몬스터정보파일을 읽어들여 생성몬스터정보 셋팅

private: // 최초 몬스터정보생성시 호출(몬스터정보파일 미존재)
	void LoadSpawnPointMeshByRegion(const std::string& _FullPath);							// 지역별 네비메쉬 로드
	void CreateReferenceInfomation(Location _Location);										// 지역별 몬스터개체수, ... 정보를 생성
	void SaveCreationCountByRegion(RefInfoByRegion& _ResultInfo);							// 지역별 최대생성몬스터갯수 정보 저장 - 상수때려박은...
	void CreateBasicMonsterInfos();															// 앞서 생성된 정보를 통해 생성하려는 몬스터 기본정보 생성
	void CreateMonsterInfos();															
	void SaveMonsterInfoFile(const std::string& _FullPath);									// 최초 생성한 몬스터정보목록 파일저장
	
private: // Logging & Conversion Function
	std::string LoggingTypeToString(Location _Type);										// Type To String Conversion(Logging) - Korean(English)
	std::string ConversionTypeToString(Location _Type);										// Type To String Conversion - English

public:
	MonsterInfoManager();
	~MonsterInfoManager();

protected:
	MonsterInfoManager(const MonsterInfoManager& _other) = delete;
	MonsterInfoManager(MonsterInfoManager&& _other) noexcept = delete;

private:
	MonsterInfoManager& operator=(const MonsterInfoManager& _other) = delete;
	MonsterInfoManager& operator=(const MonsterInfoManager&& _other) = delete;

public:
	void setMonsterSpawnPoints(GameEngineDirectory _dir);
protected:
	std::vector<MonsterSpawnInfo> GetMonsterSpawnPoints(Location _spawnArea);
private:
	std::vector<MonsterInfo> AllMonsters_;													// 현재 맵에 배치된 모든 몬스터의 정보

private:
	int MaxCreationCount_;																	// 생성하려는 몬스터 최대수(1~MaxCreationCount_까지 랜덤으로 생성갯수지정용)
	RefInfoByRegion RefInfoByRegion_[static_cast<int>(Location::MAX)];						// 지역별 생성하려는 몬스터들의 기본정보(참고용)
	std::set<float4> DesignatedSpawnPos_;													// 이미 지정되어있는 스폰위치목록

	std::vector<std::string> enum_MapName;
	std::vector<std::string> enum_MonsterName;
	std::map<std::string, std::vector<MonsterSpawnInfo>> monsterSpawnPoints_;
};

