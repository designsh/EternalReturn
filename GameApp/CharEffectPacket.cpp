#include "PreCompile.h"
#include "CharEffectPacket.h"
#include "Character.h"
#include "LumiaLevel.h"
#include <GameEngine/GameEngineCore.h>

#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include "Monsters.h"


CharEffectPacket::CharEffectPacket() // default constructer ����Ʈ ������
	: targetIndex_(-1), victimIndex_(-1)
	, victimType_(static_cast<int>(UnitType::CHARACTER))
	, worldPos_(float4::ZERO), popupDamageCount_(0)
{

}

CharEffectPacket::~CharEffectPacket() // default destructer ����Ʈ �Ҹ���
{

}

CharEffectPacket::CharEffectPacket(CharEffectPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void CharEffectPacket::SetTargetIndex(int _index)
{
    targetIndex_ = _index;
}

void CharEffectPacket::SetVictimIndex(IUnit& _unit)
{
	victimType_ = static_cast<int>(_unit.UnitType_);
	victimIndex_ = _unit.GetIndex();
}

void CharEffectPacket::SetAnimationName(const std::string& _animation)
{
    effectAnimationName_ = _animation;
}

void CharEffectPacket::SetWorldPos(float4 _worldPos)
{
	worldPos_ = _worldPos;
}

void CharEffectPacket::SetPopupDamage(float _damage)
{
	popupDamageCount_ = static_cast<int>(_damage);
}

void CharEffectPacket::userSerialize()
{
	serializer_ << targetIndex_;
	serializer_ << effectAnimationName_;
	serializer_ << victimIndex_;
	serializer_ << victimType_;
	serializer_ << worldPos_;
	serializer_ << popupDamageCount_;
}

void CharEffectPacket::userDeserialize()
{
	serializer_ >> targetIndex_;
	serializer_ >> effectAnimationName_;
	serializer_ >> victimIndex_;
	serializer_ >> victimType_;
	serializer_ >> worldPos_;
	serializer_ >> popupDamageCount_;
}

void CharEffectPacket::initPacketID()
{
	SetPacketID(ePacketID::CharEffectPacket);
}

GameEnginePacketBase* CharEffectPacket::getUserObject()
{
    return new CharEffectPacket;
}

void CharEffectPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{

	LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());

	// �Ȱ��� ���;��ϴµ�
	// Ŭ���̾�Ʈ���� ������ �� ��Ŷ�� 

	if (nullptr == level)
	{
		GameEngineDebug::MsgBoxError("����Ʈ ��Ŷ execute ���� ������ �������� �� �����߽��ϴ�.");
		return;
	}

	if (victimType_ == static_cast<int>(UnitType::CHARACTER))
	{
		Character* targetChar = level->GetCharacterActorList()[targetIndex_];


		if (-1 != victimIndex_)
		{
			if (popupDamageCount_ != 0)
			{
				Character* victimChar = level->GetCharacterActorList()[victimIndex_];
				targetChar->PlayEffect(effectAnimationName_, targetIndex_, victimChar, float4::ZERO, static_cast<float>(popupDamageCount_));
			}
			else
			{

				Character* victimChar = level->GetCharacterActorList()[victimIndex_];
				targetChar->PlayEffect(effectAnimationName_, targetIndex_, victimChar);
			}

		}
		else if (worldPos_ != float4::ZERO)
		{

			if (popupDamageCount_ != 0)
			{
				targetChar->PlayEffect(effectAnimationName_, targetIndex_, nullptr, worldPos_, static_cast<float>(popupDamageCount_));
			}
			else
			{

				targetChar->PlayEffect(effectAnimationName_, targetIndex_, nullptr, worldPos_);
			}

		
		}
		else
		{
			targetChar->PlayEffect(effectAnimationName_, targetIndex_, nullptr, float4::ZERO, static_cast<float>(popupDamageCount_));
		}
	}
	else if (victimType_ == static_cast<int>(UnitType::MONSTER))
	{
		Character* targetChar = level->GetCharacterActorList()[targetIndex_];

		if (-1 != victimIndex_)
		{
			Monsters* victimChar = level->GetMonsterActorList()[victimIndex_];
			targetChar->PlayEffect(effectAnimationName_, targetIndex_, victimChar);
		}
		else
		{
			targetChar->PlayEffect(effectAnimationName_, targetIndex_);
		}
	}




	if (_bServer)
	{
		_network->Send(this);
	}
}
