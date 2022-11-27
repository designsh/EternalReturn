#pragma once
#include <GameEngine/GameEnginePacketBase.h>
// �з� : 



class CharDeathPacket : public GameEnginePacketBase
{
public:
	CharDeathPacket(); // default constructer ����Ʈ ������
	~CharDeathPacket(); // default destructer ����Ʈ �Ҹ���
	CharDeathPacket(const CharDeathPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharDeathPacket(CharDeathPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	CharDeathPacket& operator=(const CharDeathPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharDeathPacket& operator=(const CharDeathPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetTargetIndex(int _index);
	void SetFraggerType(int _index);
	void SetFraggerIndex(int _index);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int targetIndex_;
	int fraggerType_;
	int fraggerIndex_;
};

