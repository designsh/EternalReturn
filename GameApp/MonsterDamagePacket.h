#pragma once
#include <GameEngine/GameEnginePacketBase.h>
#include "MonsterStateInfo.h"

// 분류 : 패킷
// 용도 : 
// 설명 : 현재 타겟에게 데미지를 1번만 입히기위한 패킷
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
	int Index_;							// 공격하는 몬스터의 인덱스
};

