#pragma once

#include <GameEngine/GameEnginePacketBase.h>
// �뵵 : 
// ���� : 
class NoticeBroadcastPacket : public GameEnginePacketBase
{
public:
	NoticeBroadcastPacket(); // default constructer ����Ʈ ������
	~NoticeBroadcastPacket(); // default destructer ����Ʈ �Ҹ���
	NoticeBroadcastPacket(const NoticeBroadcastPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	NoticeBroadcastPacket(NoticeBroadcastPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	NoticeBroadcastPacket& operator=(const NoticeBroadcastPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	NoticeBroadcastPacket& operator=(const NoticeBroadcastPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

