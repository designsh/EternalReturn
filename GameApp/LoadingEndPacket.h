#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// 분류 : 패킷
// 용도 : 
// 설명 : 게임로딩완료 패킷
class LoadingEndPacket : public GameEnginePacketBase
{
public:
	inline void SetUniqueID(int _UniqueID)
	{
		UniqueID_ = _UniqueID;
	}

	inline void SetLoadingFlag(int _LoadingEndFlag)
	{
		LoadingEndFlag_ = _LoadingEndFlag;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
public:
	LoadingEndPacket();
	~LoadingEndPacket();

protected:
	LoadingEndPacket(const LoadingEndPacket& _other) = delete;
	LoadingEndPacket(LoadingEndPacket&& _other) noexcept = delete;

private:
	LoadingEndPacket& operator=(const LoadingEndPacket& _other) = delete;
	LoadingEndPacket& operator=(const LoadingEndPacket&& _other) = delete;

public:
protected:
private:
	int UniqueID_;						// 고유연결ID
	int LoadingEndFlag_;				// 1: 로딩완료, 0: 로딩중
};

