#pragma once

#include <GameEngine/GameEnginePacketBase.h>
// 분류 : 
// 용도 : 
// 설명 : 
class ProhibitedAreaPacket : public GameEnginePacketBase
{
public:
	ProhibitedAreaPacket(); // default constructer 디폴트 생성자
	~ProhibitedAreaPacket(); // default destructer 디폴트 소멸자
	ProhibitedAreaPacket(const ProhibitedAreaPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ProhibitedAreaPacket(ProhibitedAreaPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	ProhibitedAreaPacket& operator=(const ProhibitedAreaPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ProhibitedAreaPacket& operator=(const ProhibitedAreaPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetReserved(int _tf);
	void SetLocation(int _loca0, int _loca1);

protected:
	virtual void initPacketID() override;
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int location0_;
	int location1_;
	int isReserved_;
};

