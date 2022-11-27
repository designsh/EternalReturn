#include "PreCompile.h"
#include "ProhibitedAreaPacket.h"
#include "ePacketID.h"

#include "LumiaLevel.h"
#include <GameEngine/GameEngineCore.h>
#include "ProhibitedArea.h"
#include "PlayerInfoManager.h"
#include "PlayerUIController.h"
ProhibitedAreaPacket::ProhibitedAreaPacket() // default constructer ����Ʈ ������
    : location0_(-1), location1_(-1), isReserved_(-1)
{

}

ProhibitedAreaPacket::~ProhibitedAreaPacket() // default destructer ����Ʈ �Ҹ���
{

}

ProhibitedAreaPacket::ProhibitedAreaPacket(ProhibitedAreaPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void ProhibitedAreaPacket::SetReserved(int _tf)
{
    isReserved_ = _tf;
}

void ProhibitedAreaPacket::SetLocation(int _loca0, int _loca1)
{
    location0_ = _loca0;
    location1_ = _loca1;
}

void ProhibitedAreaPacket::initPacketID()
{
    SetPacketID(ePacketID::ProhibitedAreaPacket);
}

void ProhibitedAreaPacket::userSerialize()
{
    serializer_ << location0_;
    serializer_ << location1_;
    serializer_ << isReserved_;
}

void ProhibitedAreaPacket::userDeserialize()
{
    serializer_ >> location0_;
    serializer_ >> location1_;
    serializer_ >> isReserved_;
}

GameEnginePacketBase* ProhibitedAreaPacket::getUserObject()
{
    return new ProhibitedAreaPacket;
}

void ProhibitedAreaPacket::execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer)
{
    LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());
    PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

    if (isReserved_ == 0)
    {
        level->GetProhibitedAreaList()[location0_]->SetProhibited(true);
        level->GetProhibitedAreaList()[location1_]->SetProhibited(true);

        level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetProhibitedArea(static_cast<Location>(location0_));
        level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetProhibitedArea(static_cast<Location>(location1_));
        GameEngineSoundManager::GetInstance()->PlaySoundByName("Announce_Restricted Area_01.wav");
    }
    else if (isReserved_ == 1)
    {
        level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetNextProhibitedArea(static_cast<Location>(location0_));
        level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetNextProhibitedArea(static_cast<Location>(location1_));
    }
    else if (isReserved_ == -1)
    {
        level->GetProhibitedAreaList()[location0_]->SetProhibited(true);
        level->GetProhibitedAreaList()[location1_]->SetProhibited(true);

        level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetProhibitedArea(static_cast<Location>(location0_));
        level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetMinimapUI()->SetProhibitedArea(static_cast<Location>(location1_));
        GameEngineSoundManager::GetInstance()->PlaySoundByName("Announce_Restricted Area_01.wav");
    }


    if (_bServer)
    {
        _network->Send(this);
    }
}

