#pragma once
#include <GameEngine/GameEnginePacketBase.h>
#include "MonsterStateInfo.h"

// �з� : ��Ŷ
// �뵵 : 
// ���� : ���� Ÿ�ٿ��� �������� 1���� ���������� ��Ŷ
class MonsterDamagePacket : public GameEnginePacketBase
{
public: // Inline Get Function
public: // Inline Set Function
	inline void SetIndex(int _Index)
	{
		Index_ = _Index;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
public:
	MonsterDamagePacket();
	~MonsterDamagePacket();

protected:
	MonsterDamagePacket(const MonsterDamagePacket& _other) = delete;
	MonsterDamagePacket(MonsterDamagePacket&& _other) noexcept = delete;

private:
	MonsterDamagePacket& operator=(const MonsterDamagePacket& _other) = delete;
	MonsterDamagePacket& operator=(const MonsterDamagePacket&& _other) = delete;

public:
protected:
private:
	int Index_;							// �����ϴ� ������ �ε���
};

