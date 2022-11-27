#include "PreCompile.h"
#include "CharCrowdControlPacket.h"
#include "LumiaLevel.h"
#include "ePacketID.h"

#include <GameEngine/GameEngineCore.h>

#include "Character.h"

CharCrowdControlPacket::CharCrowdControlPacket()
	: type_(0)
	, stunTime_(0.0f)
	, knockbackTime_(0.0f)
	, targetIndex_(-1)
	, slowTime_(0.0f)
	, slowRatio_(0.0f)
{

}

CharCrowdControlPacket::~CharCrowdControlPacket()
{

}

void CharCrowdControlPacket::SetStun(float _stunTime)
{
	type_ = static_cast<int>(eCrowdControlType::Stun);
	stunTime_ = _stunTime;
}

void CharCrowdControlPacket::SetKnockback(float _knockbackTime, float4 _knockbackSpeed)
{
	type_ = static_cast<int>(eCrowdControlType::Knockback);
	knockbackTime_ = _knockbackTime;
	knockbackSpeed_ = _knockbackSpeed;
}

void CharCrowdControlPacket::SetWallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime)
{
	type_ = static_cast<int>(eCrowdControlType::WallSlam);
	stunTime_ = _stunTime;
	knockbackTime_ = _knockbackTime;
	knockbackSpeed_ = _knockbackSpeed;
}

void CharCrowdControlPacket::SetSlow(float _slowTime, float _slowRatio)
{
	type_ = static_cast<int>(eCrowdControlType::Slow);
	slowTime_ = _slowTime;
	slowRatio_ = _slowRatio;
}

void CharCrowdControlPacket::initPacketID()
{
	SetPacketID(ePacketID::CharCrowdControlPacket);
}

void CharCrowdControlPacket::userSerialize()
{
	serializer_ << type_;
	serializer_ << stunTime_;
	serializer_ << knockbackTime_;
	serializer_ << knockbackSpeed_;
	serializer_ << targetIndex_;
	serializer_ << slowTime_;
	serializer_ << slowRatio_;
}

void CharCrowdControlPacket::userDeserialize()
{
	serializer_ >> type_;
	serializer_ >> stunTime_;
	serializer_ >> knockbackTime_;
	serializer_ >> knockbackSpeed_;
	serializer_ >> targetIndex_;
	serializer_ >> slowTime_;
	serializer_ >> slowRatio_;
}

GameEnginePacketBase* CharCrowdControlPacket::getUserObject()
{
	return new CharCrowdControlPacket;
}

void CharCrowdControlPacket::execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer)
{
	LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());

	if (nullptr == level)
	{
		return;
	}

	if (_bServer)
	{
		_network->Send(this);
	}

	const std::vector<Character*>& list = level->GetCharacterActorList();

	if (list.size() <= targetIndex_ || targetIndex_ < 0)
	{
		return;
	}

	Character* character = list[targetIndex_];
	if (character == nullptr)
	{
		return;
	}

	if (character->IsDead())
	{
		return;
	}

	switch (static_cast<eCrowdControlType>(type_))
	{
	case eCrowdControlType::Stun:
		character->Stun(stunTime_);
		break;
	case eCrowdControlType::Knockback:
		character->Knockback(knockbackTime_, knockbackSpeed_);
		break;
	case eCrowdControlType::WallSlam:
		character->WallSlam(knockbackTime_, knockbackSpeed_, stunTime_);
		break;
	case eCrowdControlType::Slow:
		character->Slow(slowTime_, slowRatio_);
		break;
	default:
		break;
	}
}
