#include "PreCompile.h"
#include "NoticeBroadcastPacket.h"
#include "ePacketID.h"
#include "LumiaLevel.h"
#include <GameEngine/GameEngineCore.h>
#include "PlayerInfoManager.h"

#include "PlayerUIController.h"
NoticeBroadcastPacket::NoticeBroadcastPacket() // default constructer ����Ʈ ������
{

}

NoticeBroadcastPacket::~NoticeBroadcastPacket() // default destructer ����Ʈ �Ҹ���
{

}

NoticeBroadcastPacket::NoticeBroadcastPacket(NoticeBroadcastPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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

    // �ڱ� �÷��̾��� uicontroller �� ����
    // noticeUI ������ ��ε�ĳ��Ʈ ��ü �ϰ������� ��Ű�� ���
    LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());
    PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
    level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetNoticeUI()->SetText(string_, timer_);

    if (_bServer)
    {
        _network->Send(this);
    }
}

