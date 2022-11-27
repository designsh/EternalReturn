#include "PreCompile.h"
#include "PacketSoundPlay.h"

#include <GameEngineBase/GameEngineSoundPlayer.h>
#include <GameEngine/GameEngineCore.h>

#include "LumiaLevel.h"
#include "PlayerInfoManager.h"
#include "ePacketID.h"

PacketSoundPlay::PacketSoundPlay()
    : soundName_("")
    , position_(float4::ZERO)
    , bPlayAll_(false)
{

}

PacketSoundPlay::~PacketSoundPlay()
{

}

void PacketSoundPlay::SetSound(const std::string& _soundName, const float4& _position, bool _bPlayAll)
{
    soundName_ = _soundName;
    position_ = _position;
    bPlayAll_ = _bPlayAll;
}

void PacketSoundPlay::initPacketID()
{
    SetPacketID(ePacketID::PacketSoundPlay);
}

void PacketSoundPlay::userSerialize()
{
    serializer_ << soundName_;
    serializer_ << position_;
    serializer_ << bPlayAll_;

}

void PacketSoundPlay::userDeserialize()
{
    serializer_ >> soundName_;
    serializer_ >> position_;
    serializer_ >> bPlayAll_;
}

GameEnginePacketBase* PacketSoundPlay::getUserObject()
{
    return new PacketSoundPlay;
}

void PacketSoundPlay::execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer)
{
    if (_bServer)
    {
        _network->Send(this);
    }

    GameEngineSoundPlayer player(soundName_);
    player.Play();

    if (bPlayAll_)
    {
        return;
    }

    // 아래로 거리에 따라 볼륨 조절하는 부분
    // 루미아 레벨이 아니면 적용하지 않는다.
    LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());
    if (nullptr != level)
    {
        auto list = level->GetCharacterActorList();
        int playerIndex = PlayerInfoManager::GetInstance()->GetMyNumber();

        if (playerIndex < list.size() && playerIndex >= 0)
        {
            GameEngineActor* actor = list[playerIndex];
            float4 actorPosition = actor->GetTransform()->GetWorldPosition();
            float distance = float4::Calc_Len3D(actorPosition, position_);

            player.SetVolume(1.0f - distance / FT::Char::MAXIMUM_DISTANCE_TO_HEAR);
        }
    }
}
