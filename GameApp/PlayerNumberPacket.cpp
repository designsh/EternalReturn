#include "PreCompile.h"
#include "PlayerNumberPacket.h"
#include "ePacketID.h"

PlayerNumberPacket::PlayerNumberPacket() // default constructer 디폴트 생성자
    : playerNumber_(-1)
{

}

PlayerNumberPacket::~PlayerNumberPacket() // default destructer 디폴트 소멸자
{

}

void PlayerNumberPacket::SetPlayerNumber(int _playerNumber)
{
    playerNumber_ = _playerNumber;
}

//void PlayerNumberPacket::SetOtherPlayers(std::vector<PlayerInfo> _others)
//{
//    othersSize_ = _others.size();
//
//    for (size_t i = 0; i < _others.size(); i++)
//    {
//        otherCharacter_.push_back(_others[i].character_);
//        otherStartPoint_.push_back(_others[i].startPoint_);
//        otherIsReady_.push_back(_others[i].isReady_);
//    }
//}


void PlayerNumberPacket::userSerialize()
{
    serializer_ << playerNumber_;
    serializer_ << othersSize_;
    serializer_.WriteVector(otherCharacter_);
    serializer_.WriteVector(otherStartPoint_);
    serializer_.WriteVector(otherIsReady_);
}

void PlayerNumberPacket::userDeserialize()
{
    serializer_ >> playerNumber_;
    serializer_ >> othersSize_;
    serializer_.ReadVector(otherCharacter_);
    serializer_.ReadVector(otherStartPoint_);
    serializer_.ReadVector(otherIsReady_);
}

void PlayerNumberPacket::initPacketID()
{
    SetPacketID(ePacketID::PlayerNumberPacket);
}

GameEnginePacketBase* PlayerNumberPacket::getUserObject()
{
    return new PlayerNumberPacket;
}

void PlayerNumberPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    //// 서버에서 받아온 남들의 정보
    //for (int i = 0; i < othersSize_; i++)
    //{
    //    PlayerInfo info;
    //    info.character_ = otherCharacter_[i];
    //    info.startPoint_ = otherStartPoint_[i];
    //    info.isReady_ = otherIsReady_[i];
    //    
    //    _network->serverPlayerList_.push_back(info);
    //}

    //_network->myPlayerNumber_ = playerNumber_;

    //if (_bServer)
    //{
    //    _network->Send(this);
    //}
}

