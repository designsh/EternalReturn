#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// 분류 : 
// 용도 : 
// 설명 : 
class StartPointSelectPacket : public GameEnginePacketBase
{
public:
	StartPointSelectPacket(); // default constructer 디폴트 생성자
	~StartPointSelectPacket(); // default destructer 디폴트 소멸자
	StartPointSelectPacket(const StartPointSelectPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StartPointSelectPacket(StartPointSelectPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	StartPointSelectPacket& operator=(const StartPointSelectPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StartPointSelectPacket& operator=(const StartPointSelectPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetStartPoint(int _startPoint);
	void SetTargetIndex(int _index);

	int GetStartPoint()
	{
		return startPoint_;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int startPoint_;
	int targetIndex_;
};

