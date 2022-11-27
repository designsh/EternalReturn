#pragma once
#include <GameEngine/GameEnginePacketBase.h>
// �з� : 
// �뵵 : 
// ���� : 
class CharMovementPacket : public GameEnginePacketBase
{
public:
	CharMovementPacket(); // default constructer ����Ʈ ������
	~CharMovementPacket(); // default destructer ����Ʈ �Ҹ���
	CharMovementPacket(const CharMovementPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharMovementPacket(CharMovementPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	CharMovementPacket& operator=(const CharMovementPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharMovementPacket& operator=(const CharMovementPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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
	// ������ ���ҷ� �־�� �ϳ�?
	// 1. � ĳ���͸� �����߳�?
	// 2. � ������ ���� �������� �����߳�?
	int targetIndex_;
	float4 pos_;
	float4 rotation_;
	// ������ �س����� ���� enum �̵� ���ǵ� ������ �ٲ� ��.

};

