#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// �з� : ��Ŷ
// �뵵 : 
// ���� : ���ӷε��Ϸ� ��Ŷ
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
	int UniqueID_;						// ��������ID
	int LoadingEndFlag_;				// 1: �ε��Ϸ�, 0: �ε���
};

