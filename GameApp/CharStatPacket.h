#pragma once
#include <GameEngine/GameEnginePacketBase.h>
#include "CharacterStat.h"
// �з� : 
// �з� : 
// �뵵 : 
// ���� : 
class CharStatPacket : public GameEnginePacketBase
{
public:
	CharStatPacket(); // default constructer ����Ʈ ������
	~CharStatPacket(); // default destructer ����Ʈ �Ҹ���
	CharStatPacket(const CharStatPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharStatPacket(CharStatPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	CharStatPacket& operator=(const CharStatPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharStatPacket& operator=(const CharStatPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetTargetIndex(int _index);
	void SetStat(CharacterStat _status);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int targetIndex_;
	CharacterStat stat_;

};

