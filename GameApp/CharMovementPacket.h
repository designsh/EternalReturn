#pragma once
#include <GameEngine/GameEnginePacketBase.h>
// 분류 : 
// 용도 : 
// 설명 : 
class CharMovementPacket : public GameEnginePacketBase
{
public:
	CharMovementPacket(); // default constructer 디폴트 생성자
	~CharMovementPacket(); // default destructer 디폴트 소멸자
	CharMovementPacket(const CharMovementPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	CharMovementPacket(CharMovementPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	CharMovementPacket& operator=(const CharMovementPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	CharMovementPacket& operator=(const CharMovementPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetTargetIndex(int _index);
	void SetPos(float4 _pos);
	void SetDirection(float4 _dir);

	int GetTargetIndex()
	{
		return targetIndex_;
	}

	float4 GetPos()
	{
		return pos_;
	}

	float4 GetDirection()
	{
		return rotation_;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	// 무엇이 원소로 있어야 하나?
	// 1. 어떤 캐릭터를 선택했나?
	// 2. 어떤 지점을 시작 지점으로 선택했나?
	int targetIndex_;
	float4 pos_;
	float4 rotation_;
	// 정수로 해놨지만 이후 enum 이든 합의된 값으로 바꿀 것.

};

