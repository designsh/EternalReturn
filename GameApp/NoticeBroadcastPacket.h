#pragma once

#include <GameEngine/GameEnginePacketBase.h>
// 용도 : 
// 설명 : 
class NoticeBroadcastPacket : public GameEnginePacketBase
{
public:
	NoticeBroadcastPacket(); // default constructer 디폴트 생성자
	~NoticeBroadcastPacket(); // default destructer 디폴트 소멸자
	NoticeBroadcastPacket(const NoticeBroadcastPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	NoticeBroadcastPacket(NoticeBroadcastPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	NoticeBroadcastPacket& operator=(const NoticeBroadcastPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	NoticeBroadcastPacket& operator=(const NoticeBroadcastPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetString(const std::string& _string);
	void SetTimer(float _timer);

protected:
	virtual void initPacketID() override;
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) override;


private:
	std::string string_;
	float timer_;
};

