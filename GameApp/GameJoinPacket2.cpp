#include "PreCompile.h"
#include "GameJoinPacket2.h"
#include "ePacketID.h"
#include "LobbyLevel.h"

#include "PlayerInfoManager.h"
#include "SetPlayerNumberPacket.h"
#include <GameEngine/GameEngineCore.h>

// 사실상 GameEngineServer 에서 서버 개설 / 클라이언트 Accept 할 때 번호를 매기는 게 맞다고 생각한다.

GameJoinPacket2::GameJoinPacket2() // default constructer 디폴트 생성자
    : info_({-1, -1, -1, 0})
    , listSize_(-1)
{
}

GameJoinPacket2::~GameJoinPacket2() // default destructer 디폴트 소멸자
{

}

void GameJoinPacket2::SetPlayerInfo(PlayerInfo _info)
{
    info_ = _info;
}

void GameJoinPacket2::userSerialize()
{
    serializer_ << info_.character_;
    serializer_ << info_.isReady_;
    serializer_ << info_.playerNumber_;
    serializer_ << info_.startPoint_;
    serializer_ << info_.playerNickname_;
    serializer_ << listSize_;
}

void GameJoinPacket2::userDeserialize()
{
    serializer_ >> info_.character_;
    serializer_ >> info_.isReady_;
    serializer_ >> info_.playerNumber_;
    serializer_ >> info_.startPoint_;
    serializer_ >> info_.playerNickname_;
    serializer_ >> listSize_;
}

void GameJoinPacket2::initPacketID()
{
    SetPacketID(ePacketID::GameJoinPacket2);
}

GameEnginePacketBase* GameJoinPacket2::getUserObject()
{
    return new GameJoinPacket2;
}

void GameJoinPacket2::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

    if ("LobbyLevel" == GameEngineCore::CurrentLevel()->GetName())
    {
        if (_bServer)
        {
            // 클라이언트가 보낸 리스트 사이즈가 0 이면 최초 접속으로 간주한다.
            if (listSize_ == 0)
            {
                for (size_t i = 0; i < pm->GetPlayerList().size(); i++)
                {
                    GameJoinPacket2 p;
                    p.SetPlayerInfo(pm->GetPlayerList()[i]);
                    _network->Send(_sender, &p);
                }
            }
            else if (listSize_ < 0)
            {
                GameEngineDebug::OutPutDebugString("잘못된 리스트 사이즈를 전송받았습니다.");
                return;
            }

            // 이건 접속한 클라이언트의 정보가 될 것이다.
            info_.playerNumber_ = static_cast<int>(pm->GetPlayerList().size());
            info_.character_ = -1;
            info_.startPoint_ = -1;
            info_.isReady_ = 0;


            // 새로운 클라이언트 정보를 서버와 클라이언트들에게 추가한다.
            pm->AddNewPlayer(info_);
            serializer_.OffsetReset();
            _network->Send(this);

            // 접속한 클라이언트 본인에게는 플레이어리스트의 인덱스를 부여한다.
            SetPlayerNumberPacket packet;
            packet.SetNumber(info_.playerNumber_);
            _network->Send(_sender, &packet);

        }
        else // client
        {
            pm->AddNewPlayer(info_);
        }
    }

   
}

