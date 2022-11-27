#pragma once
#include <GameEngine/GameEnginePacketBase.h>
// �з� : 
// �뵵 : 
// ���� : 
class LobbyToLumiaPacket : public GameEnginePacketBase
{
public:
	LobbyToLumiaPacket(); // default constructer ����Ʈ ������
	~LobbyToLumiaPacket(); // default destructer ����Ʈ �Ҹ���
	LobbyToLumiaPacket(const LobbyToLumiaPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	LobbyToLumiaPacket(LobbyToLumiaPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	LobbyToLumiaPacket& operator=(const LobbyToLumiaPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	LobbyToLumiaPacket& operator=(const LobbyToLumiaPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;
};

