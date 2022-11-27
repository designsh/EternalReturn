#include "PreCompile.h"
#include "CharStatPacket.h"

#include "ePacketID.h"
#include "PlayerInfoManager.h"

CharStatPacket::CharStatPacket() // default constructer 디폴트 생성자
{

}

CharStatPacket::~CharStatPacket() // default destructer 디폴트 소멸자
{

}

CharStatPacket::CharStatPacket(CharStatPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void CharStatPacket::SetTargetIndex(int _index)
{
    targetIndex_ = _index;
}

void CharStatPacket::SetStat(CharacterStat _status)
{
    stat_ = _status;
}

void CharStatPacket::userSerialize()
{
    serializer_ << targetIndex_;
    serializer_ << stat_.AttackPower;
    serializer_ << stat_.AttackRange;
    serializer_ << stat_.AttackSpeed;
    serializer_ << stat_.AttackSpeedRatio;
    serializer_ << stat_.BasicAttackAmplification;
    serializer_ << stat_.BasicAttackDamageReduction;
    serializer_ << stat_.CooldownReduction;
    serializer_ << stat_.CriticalChance;
    serializer_ << stat_.CriticalDamageAmplification;
    serializer_ << stat_.CriticalDamageReduction;
    serializer_ << stat_.Defence;
    serializer_ << stat_.HP;
    serializer_ << stat_.HPMax;
    serializer_ << stat_.HPRegeneration;
    serializer_ << stat_.LifeSteel;
    serializer_ << stat_.MovementSpeed;
    serializer_ << stat_.OmniVamp;
    serializer_ << stat_.SkillAttackDamageReduction;
    serializer_ << stat_.SkillDamageAmplification;
    serializer_ << stat_.SP;
    serializer_ << stat_.SPMax;
    serializer_ << stat_.SPRegeneration;
    serializer_ << stat_.Tenacity;
    serializer_ << stat_.TrapDamageReduction;
    serializer_ << stat_.VisionRange;
    serializer_ << stat_.passive_Count;
    serializer_ << stat_.SP_Q;
    serializer_ << stat_.SP_W;
    serializer_ << stat_.SP_E;
    serializer_ << stat_.SP_R;
}

void CharStatPacket::userDeserialize()
{
    serializer_ >> targetIndex_;
    serializer_ >> stat_.AttackPower;
    serializer_ >> stat_.AttackRange;
    serializer_ >> stat_.AttackSpeed;
    serializer_ >> stat_.AttackSpeedRatio;
    serializer_ >> stat_.BasicAttackAmplification;
    serializer_ >> stat_.BasicAttackDamageReduction;
    serializer_ >> stat_.CooldownReduction;
    serializer_ >> stat_.CriticalChance;
    serializer_ >> stat_.CriticalDamageAmplification;
    serializer_ >> stat_.CriticalDamageReduction;
    serializer_ >> stat_.Defence;
    serializer_ >> stat_.HP;
    serializer_ >> stat_.HPMax;
    serializer_ >> stat_.HPRegeneration;
    serializer_ >> stat_.LifeSteel;
    serializer_ >> stat_.MovementSpeed;
    serializer_ >> stat_.OmniVamp;
    serializer_ >> stat_.SkillAttackDamageReduction;
    serializer_ >> stat_.SkillDamageAmplification;
    serializer_ >> stat_.SP;
    serializer_ >> stat_.SPMax;
    serializer_ >> stat_.SPRegeneration;
    serializer_ >> stat_.Tenacity;
    serializer_ >> stat_.TrapDamageReduction;
    serializer_ >> stat_.VisionRange;
    serializer_ >> stat_.passive_Count;
    serializer_ >> stat_.SP_Q;
    serializer_ >> stat_.SP_W;
    serializer_ >> stat_.SP_E;
    serializer_ >> stat_.SP_R;
}

void CharStatPacket::initPacketID()
{
    SetPacketID(ePacketID::CharStatPacket);
}

GameEnginePacketBase* CharStatPacket::getUserObject()
{  
    return new CharStatPacket;
}

void CharStatPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

    *(pm->GetPlayerList()[targetIndex_].stat_) = stat_;

    if (_bServer)
    {
        _network->Send(this);
    }
}