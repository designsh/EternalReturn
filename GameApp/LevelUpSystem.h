#pragma once
#include "Enums.h"
#include "LevelUPData.h"

// �з� : �ý���
// �뵵 : 
// ���� : ���ӽð��� ���� ������ ó����� ����
class LevelUpSystem
{
public: // Static Value & Function
	static LevelUpSystem* GetInstance();

public: // ���������� ���� �ε�
	void LoadLevelInfomationFile();

public: // �ϰ�ó��
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
	std::map<JobType, LevelUPData> CharacterLevelData_;										// ĳ���͵��� ������ �������
	std::map<MonsterType, LevelUPData> MonsterLevelData_;									// ���͵��� ������ �������
};

