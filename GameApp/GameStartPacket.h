#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// �з� : ��Ŷ
// �뵵 : 
// ���� : ���ӽ��� ��Ŷ
class GameStartPacket : public GameEnginePacketBase
{
public:

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
public:
	GameStartPacket();
	~GameStartPacket();

protected:
	GameStartPacket(const GameStartPacket& _other) = delete;
	GameStartPacket(GameStartPacket&& _other) noexcept = delete;

private:
	GameStartPacket& operator=(const GameStartPacket& _other) = delete;
	GameStartPacket& operator=(const GameStartPacket&& _other) = delete;

public:
protected:
private:
};

