#pragma once
#include "MonsterInfo.h"
#include <set>

// ������ �߻�������������
struct RefInfoByRegion
{
public: // �⺻��������
	Location RegionType_;																	// ����Ÿ��
	int TotalMonsterCnt_;																	// ������ �ִ���� ������

public: // ��ġ��������(������ġ������) - �ش� ������ �ش��ϴ� ���ؽ��� ��ξ˰��ִ´�.
	std::string FullSpawnFilePath_;															// �ش� ������ �޽����ϰ��
	GameEngineFBXMesh* SpawnPointMesh_;														// ��ǥ��ϸ޽�(�׺�޽�Ȱ��: ������ġ������)

public: // ��ü������(�����Ϸ��¸��Ͱ�ü�� ������)
	int MonsterCount_[static_cast<int>(MonsterType::MAX)];									// ����(�߻�����) Ÿ�Ժ� ��ü��
};

// �з� : Manager
// �뵵 : 
// ���� : ���� �ʿ� ��ġ�� ��� ������ ������ �����ϴ� ������
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
	bool CreatMonsterInfomation();															// ����ʿ� ��ġ�Ϸ��� ������ �⺻������ ����

public: // �ܺ�ȣ��
	void LoadMonsterInfoFile();																// Ŭ������

protected:

private: // �̹� ��ϵ� �������� ����� ȣ��
	void LoadMonsterInfoFile(const std::string& _FullPath);									// �����Ȱ�ο� �������������� �о�鿩 ������������ ����

private: // ���� �������������� ȣ��(������������ ������)
	void LoadSpawnPointMeshByRegion(const std::string& _FullPath);							// ������ �׺�޽� �ε�
	void CreateReferenceInfomation(Location _Location);										// ������ ���Ͱ�ü��, ... ������ ����
	void SaveCreationCountByRegion(RefInfoByRegion& _ResultInfo);							// ������ �ִ�������Ͱ��� ���� ���� - �����������...
	void CreateBasicMonsterInfos();															// �ռ� ������ ������ ���� �����Ϸ��� ���� �⺻���� ����
	void CreateMonsterInfos();															
	void SaveMonsterInfoFile(const std::string& _FullPath);									// ���� ������ ����������� ��������
	
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
	std::vector<MonsterInfo> AllMonsters_;													// ���� �ʿ� ��ġ�� ��� ������ ����

private:
	int MaxCreationCount_;																	// �����Ϸ��� ���� �ִ��(1~MaxCreationCount_���� �������� ��������������)
	RefInfoByRegion RefInfoByRegion_[static_cast<int>(Location::MAX)];						// ������ �����Ϸ��� ���͵��� �⺻����(�����)
	std::set<float4> DesignatedSpawnPos_;													// �̹� �����Ǿ��ִ� ������ġ���

	std::vector<std::string> enum_MapName;
	std::vector<std::string> enum_MonsterName;
	std::map<std::string, std::vector<MonsterSpawnInfo>> monsterSpawnPoints_;
};

