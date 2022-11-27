#pragma once

#include <GameEngine/GameEnginePacketBase.h>

class SetPlayerNumberPacket : public GameEnginePacketBase
{
public:
	SetPlayerNumberPacket(); 
	~SetPlayerNumberPacket();

	SetPlayerNumberPacket(const SetPlayerNumberPacket& _other) = delete; 
	SetPlayerNumberPacket(SetPlayerNumberPacket&& _other) = delete; 

	SetPlayerNumberPacket& operator=(const SetPlayerNumberPacket& _other) = delete;
	SetPlayerNumberPacket& operator=(const SetPlayerNumberPacket&& _other) = delete;

public:
	virtual void initPacketID() override;

	virtual void userSerialize() override;

	virtual void userDeserialize() override;

	virtual GameEnginePacketBase* getUserObject() override;

	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) override;

public:
	void SetNumber(int _playerNumber) { playerNumber_ = _playerNumber; }

private:
	int playerNumber_;
};

