#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// 분류 : 패킷
// 용도 : 
// 설명 : GameTime 동기화 패킷(매프레임마다 송신)
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
	int Day_;																		// 일차
	DayAndNightType Type_;															// 현재 타입(낮/밤)
	float GameTime_;																// 총 게임진행시간
	float DayAndNightTime_;															// 낮/밤 전환시까지 남은시간
};

