#pragma once
#include <GameEngine/GameEnginePacketBase.h>
#include "eCrowdControlType.h"

// 분류 : 패킷
// 용도 : 몬스터 CC 처리
// 설명 : 
class MonsterCrowdControlPacket : public GameEnginePacketBase
{
public:
	inline void SetStun(int _Index, float _StunTime)
	{
		Index_ = _Index;
		CCType_ = eCrowdControlType::Stun;
		StunTime_ = _StunTime;
	}

	inline void SetKnockback(int _Index, float _KnockbackTime, float4 _KnockbackSpeed)
	{
		Index_ = _Index;
		CCType_ = eCrowdControlType::Knockback;
		KnockbackTime_ = _KnockbackTime;
		KnockbackSpeed_ = _KnockbackSpeed;
	}

	inline void SetWallSlam(int _Index, float _KnockbackTime, float4 _KnockbackSpeed, float _StunTime)
	{
		Index_ = _Index;
		CCType_ = eCrowdControlType::WallSlam;
		KnockbackTime_ = _KnockbackTime;
		KnockbackSpeed_ = _KnockbackSpeed;
		StunTime_ = _StunTime;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
public:
	MonsterCrowdControlPacket();
	~MonsterCrowdControlPacket();

protected:
	MonsterCrowdControlPacket(const MonsterCrowdControlPacket& _other) = delete;
	MonsterCrowdControlPacket(MonsterCrowdControlPacket&& _other) noexcept = delete;

private:
	MonsterCrowdControlPacket& operator=(const MonsterCrowdControlPacket& _other) = delete;
	MonsterCrowdControlPacket& operator=(const MonsterCrowdControlPacket&& _other) = delete;

public:
protected:
private:
	int Index_;												// 몬스터인덱스
	eCrowdControlType CCType_;								// 군중제어(CC) 타입
	float StunTime_;										// 스턴시간
	float KnockbackTime_;									// 넉백시간
	float4 KnockbackSpeed_;									// 넉백속도
};

