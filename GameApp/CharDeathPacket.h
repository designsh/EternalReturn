#pragma once
#include <GameEngine/GameEnginePacketBase.h>
// 분류 : 



class CharDeathPacket : public GameEnginePacketBase
{
public:
	CharDeathPacket(); // default constructer 디폴트 생성자
	~CharDeathPacket(); // default destructer 디폴트 소멸자
	CharDeathPacket(const CharDeathPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	CharDeathPacket(CharDeathPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	CharDeathPacket& operator=(const CharDeathPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	CharDeathPacket& operator=(const CharDeathPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetTargetIndex(int _index);
	void SetFraggerType(int _index);
	void SetFraggerIndex(int _index);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int targetIndex_;
	int fraggerType_;
	int fraggerIndex_;
};

