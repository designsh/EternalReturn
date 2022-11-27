#pragma once

#include <GameEngine/GameEnginePacketBase.h>

class PacketSoundPlay : public GameEnginePacketBase
{
public:
	PacketSoundPlay(); 
	~PacketSoundPlay();
	PacketSoundPlay(const PacketSoundPlay& _other) = delete; 
	PacketSoundPlay(PacketSoundPlay&& _other) = delete; 
	PacketSoundPlay& operator=(const PacketSoundPlay& _other) = delete;
	PacketSoundPlay& operator=(const PacketSoundPlay&& _other) = delete;

public:
	void SetSound(const std::string& _soundName, const float4& _position, bool _bPlayAll = false);

protected:
	virtual void initPacketID() override;
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) override;


private:
	std::string soundName_;
	float4 position_;
	bool bPlayAll_;
};

