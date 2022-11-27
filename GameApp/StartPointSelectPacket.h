#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// �з� : 
// �뵵 : 
// ���� : 
class StartPointSelectPacket : public GameEnginePacketBase
{
public:
	StartPointSelectPacket(); // default constructer ����Ʈ ������
	~StartPointSelectPacket(); // default destructer ����Ʈ �Ҹ���
	StartPointSelectPacket(const StartPointSelectPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	StartPointSelectPacket(StartPointSelectPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	StartPointSelectPacket& operator=(const StartPointSelectPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StartPointSelectPacket& operator=(const StartPointSelectPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

