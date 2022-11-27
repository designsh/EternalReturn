#include "PreCompile.h"
#include "CharAnimationPacket.h"

#include "ePacketID.h"
#include "PlayerInfoManager.h"

CharAnimationPacket::CharAnimationPacket() // default constructer 디폴트 생성자
    : targetIndex_(-1)
{

}

CharAnimationPacket::~CharAnimationPacket() // default destructer 디폴트 소멸자
{

}

void CharAnimationPacket::SetTargetIndex(int _index)
{
    targetIndex_ = _index;
}

void CharAnimationPacket::SetAnimationName(const std::string& _animation)
{
    curAnimation_ = _animation;
}

void CharAnimationPacket::SetOverrideAnimation(const std::string& _overrideAnimationName, const std::string& _boneNameToAffect)
{
    overrideAnimationName_ = _overrideAnimationName;
    overrideAnimationBoneName_ = _boneNameToAffect;
}

void CharAnimationPacket::userSerialize()
{
    serializer_ << targetIndex_;
    serializer_ << curAnimation_;
    serializer_ << overrideAnimationName_;
    serializer_ << overrideAnimationBoneName_;
}

void CharAnimationPacket::userDeserialize()
{
    serializer_ >> targetIndex_;
    serializer_ >> curAnimation_;
    serializer_ >> overrideAnimationName_;
    serializer_ >> overrideAnimationBoneName_;
}

void CharAnimationPacket::initPacketID()
{
    SetPacketID(ePacketID::CharAnimationPacket);
}

GameEnginePacketBase* CharAnimationPacket::getUserObject()
{
    return new CharAnimationPacket;
}

void CharAnimationPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
    pm->GetPlayerList()[targetIndex_].curAnimation_ = curAnimation_;
    pm->GetPlayerList()[targetIndex_].overrideAnimationName_ = overrideAnimationName_;
    pm->GetPlayerList()[targetIndex_].overrideAnimationBoneName_ = overrideAnimationBoneName_;

    if (_bServer)
    {
        _network->Send(this);
    }
}

