#pragma once
#include <GameEngine/GameEnginePacketBase.h>
#include "CharacterStat.h"
// 분류 : 
// 분류 : 
// 용도 : 
// 설명 : 
class CharStatPacket : public GameEnginePacketBase
{
public:
	CharStatPacket(); // default constructer 디폴트 생성자
	~CharStatPacket(); // default destructer 디폴트 소멸자
	CharStatPacket(const CharStatPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	CharStatPacket(CharStatPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	CharStatPacket& operator=(const CharStatPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	CharStatPacket& operator=(const CharStatPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetTargetIndex(int _index);
	void SetStat(CharacterStat _status);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int targetIndex_;
	CharacterStat stat_;

};

