#pragma once

#include <GameEngine/GameEnginePacketBase.h>
#include "eCrowdControlType.h"

class CharCrowdControlPacket : public GameEnginePacketBase
{
public:
	CharCrowdControlPacket();
	~CharCrowdControlPacket();

	CharCrowdControlPacket(const CharCrowdControlPacket& _other) = delete; 
	CharCrowdControlPacket(CharCrowdControlPacket&& _other) = delete; 

	CharCrowdControlPacket& operator=(const CharCrowdControlPacket& _other) = delete;
	CharCrowdControlPacket& operator=(const CharCrowdControlPacket&& _other) = delete;

public:
	//void SetCharacterIndex(int _targetIndex) { targetIndex_ = _targetIndex; }
	//void SetCrowdControlType(eCrowdControlType _type) { type_ = _type; }
	//void SetStunTime(float _stunTime) { stunTime_ = _stunTime; }
	//void SetKnockbackTime(float _knockbackTime) { knockbackTime_ = _knockbackTime; }
	//void SetKnockbackSpeed(const float4& _knockbackSpeed) { knockbackSpeed_ = _knockbackSpeed; }
	
	eCrowdControlType GetType() { return static_cast<eCrowdControlType>(type_); }
	
	void SetTargetIndex(int _targetIndex) { targetIndex_ = _targetIndex; }
	void SetStun(float _stunTime);
	void SetKnockback(float _knockbackTime, float4 _knockbackSpeed);
	void SetWallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime);
	void SetSlow(float _slowTime, float _slowRatio);

protected:
	virtual void initPacketID() override;
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) override;


private:
	int type_;
	float stunTime_;
	float knockbackTime_;
	float4 knockbackSpeed_;

	float slowTime_;
	float slowRatio_;
	int targetIndex_;
};

