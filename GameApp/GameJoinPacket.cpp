#include "PreCompile.h"
#include "GameJoinPacket.h"
#include "ePacketID.h"
#include "LobbyLevel.h"

#include "PlayerInfo.h"
// 사실상 GameEngineServer 에서 서버 개설 / 클라이언트 Accept 할 때 번호를 매기는 게 맞다고 생각한다.

GameJoinPacket::GameJoinPacket() // default constructer 디폴트 생성자
{
}

GameJoinPacket::~GameJoinPacket() // default destructer 디폴트 소멸자
{

}

void GameJoinPacket::SetOtherPlayers(std::vector<PlayerInfo> _others)
{
    othersSize_ = static_cast<int>(_others.size());

    for (size_t i = 0; i < _others.size(); i++)
    {
        otherCharacter_.push_back(_others[i].character_);
        otherStartPoint_.push_back(_others[i].startPoint_);
        otherIsReady_.push_back(_others[i].isReady_);
    }
}

void GameJoinPacket::userSerialize()
{
    serializer_ << othersSize_;
    serializer_.WriteVector(otherCharacter_);
    serializer_.WriteVector(otherStartPoint_);
    serializer_.WriteVector(otherIsReady_);
}

void GameJoinPacket::userDeserialize()
{
    serializer_ >> othersSize_;
    serializer_.ReadVector(otherCharacter_);
    serializer_.ReadVector(otherStartPoint_);
    serializer_.ReadVector(otherIsReady_);
}

void GameJoinPacket::initPacketID()
{
    SetPacketID(ePacketID::GameJoinPacket);
}

GameEnginePacketBase* GameJoinPacket::getUserObject()
{
    return new GameJoinPacket;
}

void GameJoinPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    //if (PlayerInfoManager::GetInstance()->GetMyNumber() == 0)
    //{
    //    // 아직 플레이어 넘버가 정해지지 않은 신규 클라이언트는 이 패킷 대상에서 제외됩니다.
    //    return;
    //}

    //PlayerInfo info;
    //info.character_ = -1;
    //info.startPoint_ = -1;
    //info.isReady_ = false;
    //_network->serverPlayerList_.push_back(info);
    //// 브로드캐스팅 받는 기존 클라이언트들에게는 신규 클라의 구좌를 만들어줍니다.

    //if (_bServer)
    //{

    //    _network->Send(this);
    //}
}

