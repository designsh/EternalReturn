#include "PreCompile.h"
#include "NoticeBroadcastPacket.h"
#include "ePacketID.h"
#include "LumiaLevel.h"
#include <GameEngine/GameEngineCore.h>
#include "PlayerInfoManager.h"

#include "PlayerUIController.h"
NoticeBroadcastPacket::NoticeBroadcastPacket() // default constructer 디폴트 생성자
{

}

NoticeBroadcastPacket::~NoticeBroadcastPacket() // default destructer 디폴트 소멸자
{

}

NoticeBroadcastPacket::NoticeBroadcastPacket(NoticeBroadcastPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void NoticeBroadcastPacket::SetString(const std::string& _string)
{
    string_ = _string;
}

void NoticeBroadcastPacket::SetTimer(float _timer)
{
    timer_ = _timer;
}

void NoticeBroadcastPacket::initPacketID()
{
    SetPacketID(ePacketID::NoticeBroadcastPacket);
}

void NoticeBroadcastPacket::userSerialize()
{
    serializer_ << string_;
    serializer_ << timer_;
}

void NoticeBroadcastPacket::userDeserialize()
{
    serializer_ >> string_;
    serializer_ >> timer_;
}

GameEnginePacketBase* NoticeBroadcastPacket::getUserObject()
{
    return new NoticeBroadcastPacket;
}

void NoticeBroadcastPacket::execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer)
{

    // 자기 플레이어의 uicontroller 에 접근
    // noticeUI 접근해 브로드캐스트 전체 일괄적으로 시키는 기능
    LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());
    PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
    level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetNoticeUI()->SetText(string_, timer_);

    if (_bServer)
    {
        _network->Send(this);
    }
}

