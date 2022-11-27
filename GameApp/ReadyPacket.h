#pragma once

#include <GameEngine/GameEnginePacketBase.h>

// �з� : 
// �뵵 : 
// ���� : �ڽ��� ���� ���¸� �������� ���� ��û�մϴ�.
// ĳ���� ���� �Ϸ� �� �غ� �ƴٰ� �����ִ� �뵵�Դϴ�.
// �ƴϸ� �ٽ� ���� ����ϰ� �غ� ���� �ʾҴٰ� �����ִ� �뵵�ų�...

class ReadyPacket : public GameEnginePacketBase
{
public:
	ReadyPacket(); // default constructer ����Ʈ ������
	~ReadyPacket(); // default destructer ����Ʈ �Ҹ���
	ReadyPacket(const ReadyPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	ReadyPacket(ReadyPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	ReadyPacket& operator=(const ReadyPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ReadyPacket& operator=(const ReadyPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

