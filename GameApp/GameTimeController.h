#pragma once
#include "Enums.h"
#include <GameEngineBase/GameEngineRandom.h>
// ������ ��/��ð�
struct DailyTime
{
	float DayTime_;					// ���ð�
	float NightTime_;				// ��ð�
};

// ���� ù���忡 �ʿ��� ����
struct MonsterFirstAppearInfo
{
	bool IsAppear_;					// ù����Ϸ� Flag(true: ù����Ϸ�)
	float AppearTime_;				// ù����ð�(���ð�)
};

// �з� : ��Ʈ�ѷ�
// �뵵 : 
// ���� : ���ӳ� �ð����� ��� �ʿ��� ��� ���� �̺�Ʈó�� ����� �����ϸ� �����ϴ� �̱��� ��ü
class Monsters;
class GameTimeController
{
public: // Static Value & Function
	static GameTimeController* GetInstance();

public: // Inline Get Function
	inline DayAndNightType GetCurrentDayType() const
	{
		return CurDayOrNight_;
	}

	inline int GetCurrentDay() const
	{
		return CurDay_;
	}

public: // Inline Set Function

public: // TotalGameTime Related Get Function
	tm GetTotalGameTimeToMinute();																		// �� ��������ð��� ��/�ʷ� ����Ͽ� ��ȯ(tm ����ü�� ��ȯ)
	tm GetTotalGameTimeToHour();																		// �� ��������ð��� ��/��/�ʷ� ����Ͽ� ��ȯ(tm ����ü�� ��ȯ)
	float GetTotalGameTimeToSec();																		// �� ��������ð��� �ʷ� ��ȯ(�״�� ��ȯ)

public: // Time remaining Related Get Function
	tm GetRemainTimeToMinute();																			// ��/�� ��ȯ���� �����ð��� ��/�ʷ� ����Ͽ� ��ȯ(tm ����ü�� ��ȯ)
	tm GetRemainTimeToHour();																			// ��/�� ��ȯ���� �����ð��� ��/��/�ʷ� ����Ͽ� ��ȯ(tm ����ü�� ��ȯ)
	float GetRemainTimeToSec();																			// ��/�� ��ȯ���� �����ð��� �ʷ� ��ȯ(�״�� ��ȯ)
	
public: // Public Set Function

public: // Public Function(Host)
	void HostInitialize();																				// Player Level Update()���� �ش��Լ��� ȣ��: Host Controller Initialize
	void HostUpdate(float _DeltaTime);																	// Player Level Update()���� �ش��Լ��� ȣ��: Host Controller Update

public: // Public Function(Guest)
	void GuestUpdate(float _GameTime, DayAndNightType _Type, int _CurDay, float _DayAndNightTime);		// GameTimeSyncPacket ���Ž� ȣ�� : Guest Controller Update

public: // Public Day/Night Forced Conversion Function
	void DayAndNightForecedConversion();																// ��/�� ������ȯ �Լ�

protected: // Protected Function
private: // Private Fcuntion

private: // Private Function : GameTime Related Function
	void CreateDailytimes();																			// ������ ������ �ð����
	void CalcGameTime(float _DeltaTime);																// ��������ð��� ���� ��/�� ��ȯ, ���� ��ȯ �� ó��

private: // Private Function : Monster Related Function
	void CreateMonsterFirstAppearInfo();																// ����Ÿ�Ժ� ù���忡 �ʿ��� ���� ����
	void UpdateMonsterFirstAppearTime(float _DeltaTime);												// ����Ÿ�Ժ� ù���忡 �ʿ��� ���ǿ� ���� ù����Ÿ�� üũ
	void FirstAppearMonsters(MonsterType _MonsterType);													// Ư���ð��� ���� ���ʵ����ؾ��ϴ� ���� Ÿ���� �ѹ��� ����ó��


private: // ���� ���� ����
	void MakeNextProhibitedArea();																	// �������� �� ���� ����
	void MakeNextProhibitedArea(Location _location0, Location _location1);							// Ư�� ���� �� ���� ����
	void SetProhibitedArea();																			// ����� ���� ���������� ������ ������������ ����� ����


public:
	GameTimeController();
	~GameTimeController();

protected:
	GameTimeController(const GameTimeController& _other) = delete;
	GameTimeController(GameTimeController&& _other) = delete;

private:
	GameTimeController& operator=(const GameTimeController& _other) = delete;
	GameTimeController& operator=(const GameTimeController&& _other) = delete;

public:
protected:
private: // Day/Night Forced Conversion Related Value
	bool IsForcedConversion_;																			// ������ȯ ���� Flag

private:
	float DayAndNightTime_;																				// �������� �ٲ�� ����(��) :��/�� ��ȯ���� �����ð�(�ʱ�ȭ����: ��/����ȯ��)
	float CurGameTime_;																					// ���� ��������ð�(����) : �������� ����Ǵ½����� �ʱ�ȭ
	int PrevDay_;																						// ���� ����
	int CurDay_;																						// ���� ���� : ��->������ �Ѿ�� ������ ����
	DayAndNightType CurDayOrNight_;																		// ���� ���ΰ� ���ΰ� Ÿ��

private: // ���ӽð� ����
	std::vector<DailyTime> Dailytimes_;																	// ������ ��/��ð�

private: // ����ù���� ����
	bool AllMonsterAppearEnd_;																			// ��� ����Ÿ�Կ� �����Ͽ� ù���� ���� ��ȯ�Ϸ�� On(true: ������ù����Ϸ�)
	MonsterFirstAppearInfo MonsterFirstAppearList_[static_cast<int>(MonsterType::MAX)];					// ���� ���� �������(ù���忡�� �����ϸ� ���� ���ͻ�� �� ������ ������ ���Ϳ��� ����)

private: // �׽�Ʈ��
	float TestTime_;

private:	// �������� ����������
	GameEngineRandom randomGenerator_;
	int nextProhibitArea0_;
	int nextProhibitArea1_;

	bool isReserved_;
	bool isFirstAnnounced_;
private: // ���� ���� 

};

