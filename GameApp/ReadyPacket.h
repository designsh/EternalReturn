#pragma once

#include <GameEngine/GameEnginePacketBase.h>

// 분류 : 
// 용도 : 
// 설명 : 자신의 레디 상태를 서버에게 갱신 요청합니다.
// 캐릭터 선택 완료 후 준비가 됐다고 보내주는 용도입니다.
// 아니면 다시 선택 취소하고 준비가 되지 않았다고 보내주는 용도거나...

class ReadyPacket : public GameEnginePacketBase
{
public:
	ReadyPacket(); // default constructer 디폴트 생성자
	~ReadyPacket(); // default destructer 디폴트 소멸자
	ReadyPacket(const ReadyPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ReadyPacket(ReadyPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	ReadyPacket& operator=(const ReadyPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ReadyPacket& operator=(const ReadyPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetTargetIndex(int _index);
	void SetReadyStatus(int _isReady);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int targetIndex_;
	int isReady_;
};

