#pragma once
#include <GameEngine/GameEnginePacketBase.h>

#include "PlayerInfo.h"

class LoadingLumiaPacket : public GameEnginePacketBase
{
public:
	LoadingLumiaPacket(); // default constructer 디폴트 생성자
	~LoadingLumiaPacket(); // default destructer 디폴트 소멸자
	LoadingLumiaPacket(const LoadingLumiaPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	LoadingLumiaPacket(LoadingLumiaPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	LoadingLumiaPacket& operator=(const LoadingLumiaPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	LoadingLumiaPacket& operator=(const LoadingLumiaPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:

};

