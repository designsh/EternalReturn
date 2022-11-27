#pragma once
#include <GameEngine/GameEnginePacketBase.h>

#include "PlayerInfo.h"

class LoadingLumiaPacket : public GameEnginePacketBase
{
public:
	LoadingLumiaPacket(); // default constructer ����Ʈ ������
	~LoadingLumiaPacket(); // default destructer ����Ʈ �Ҹ���
	LoadingLumiaPacket(const LoadingLumiaPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	LoadingLumiaPacket(LoadingLumiaPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	LoadingLumiaPacket& operator=(const LoadingLumiaPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	LoadingLumiaPacket& operator=(const LoadingLumiaPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:

};

