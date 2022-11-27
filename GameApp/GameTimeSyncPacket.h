#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// �з� : ��Ŷ
// �뵵 : 
// ���� : GameTime ����ȭ ��Ŷ(�������Ӹ��� �۽�)
class GameTimeSyncPacket : public GameEnginePacketBase
{
public: // Inline Get Function
public: // Inline Set Function
	inline void SetCurrentDay(int _Day)
	{
		Day_ = _Day;
	}

	inline void SetDayAndNightType(DayAndNightType _Type)
	{
		Type_ = _Type;
	}

	inline void SetGameTime(float _GameTime)
	{
		GameTime_ = _GameTime;
	}

	inline void SetRemainTime(float _DayAndNightTime)
	{
		DayAndNightTime_ = _DayAndNightTime;
	}

public: // Public Function
protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:

public:
	GameTimeSyncPacket();
	~GameTimeSyncPacket();

protected:
	GameTimeSyncPacket(const GameTimeSyncPacket& _other) = delete;
	GameTimeSyncPacket(GameTimeSyncPacket&& _other) noexcept = delete;

private:
	GameTimeSyncPacket& operator=(const GameTimeSyncPacket& _other) = delete;
	GameTimeSyncPacket& operator=(const GameTimeSyncPacket&& _other) = delete;

public:
protected:
private:
	int Day_;																		// ����
	DayAndNightType Type_;															// ���� Ÿ��(��/��)
	float GameTime_;																// �� ��������ð�
	float DayAndNightTime_;															// ��/�� ��ȯ�ñ��� �����ð�
};

