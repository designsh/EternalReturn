#pragma once

#include <GameEngine/GameEnginePacketBase.h>
// �з� : 
// �뵵 : 
// ���� : 
class ProhibitedAreaPacket : public GameEnginePacketBase
{
public:
	ProhibitedAreaPacket(); // default constructer ����Ʈ ������
	~ProhibitedAreaPacket(); // default destructer ����Ʈ �Ҹ���
	ProhibitedAreaPacket(const ProhibitedAreaPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	ProhibitedAreaPacket(ProhibitedAreaPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	ProhibitedAreaPacket& operator=(const ProhibitedAreaPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ProhibitedAreaPacket& operator=(const ProhibitedAreaPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetReserved(int _tf);
	void SetLocation(int _loca0, int _loca1);

protected:
	virtual void initPacketID() override;
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int location0_;
	int location1_;
	int isReserved_;
};

