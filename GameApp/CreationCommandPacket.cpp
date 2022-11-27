#include "PreCompile.h"
#include "CreationCommandPacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include "MonsterInfoManager.h"
#include "UserGame.h"
#include "LoadingLevel.h"
#include "LumiaLevel.h"

void CreationCommandPacket::userSerialize()
{
    int sendIsFirst = 0;
    if (true == IsFirst_)
    {
        sendIsFirst = 1;
        serializer_ << sendIsFirst;
    }
    else
    {
        sendIsFirst = 0;
        serializer_ << sendIsFirst;
    }

    serializer_ << TotMonsterCount_;

    // 몬스터상세정보
    serializer_ << MonsterInfo_.Index_;
    serializer_ << static_cast<int>(MonsterInfo_.RegionType_);
    serializer_ << static_cast<int>(MonsterInfo_.MonsterType_);

    int sendIsGroup = 0;
    if (true == MonsterInfo_.IsGroup_)
    {
        sendIsGroup = 1;
        serializer_ << sendIsGroup;
    }
    else
    {
        serializer_ << sendIsGroup;
    }

    serializer_ << MonsterInfo_.SpawnPosition_;
}

void CreationCommandPacket::userDeserialize()
{
    int rcvIsFirst = 0;
    serializer_ >> rcvIsFirst;
    if (1 == rcvIsFirst)
    {
        IsFirst_ = true;
    }
    else
    {
        IsFirst_ = false;
    }
    
    serializer_ >> TotMonsterCount_;

    // 몬스터상세정보
    serializer_ >> MonsterInfo_.Index_;

    int rcvRegionType = 0;
    serializer_ >> rcvRegionType;
    MonsterInfo_.RegionType_ = static_cast<Location>(rcvRegionType);

    int rcvMonsterType = 0;
    serializer_ >> rcvMonsterType;
    MonsterInfo_.MonsterType_ = static_cast<MonsterType>(rcvMonsterType);

    int rcvIsGroup = 0;
    serializer_ >> rcvIsGroup;
    if (1 == rcvIsGroup)
    {
        MonsterInfo_.IsGroup_ = true;
    }
    else
    {
        MonsterInfo_.IsGroup_ = false;
    }

    serializer_ << MonsterInfo_.SpawnPosition_;
}

void CreationCommandPacket::initPacketID()
{
    SetPacketID(ePacketID::CreationPacket);
}

GameEnginePacketBase* CreationCommandPacket::getUserObject()
{
    return new CreationCommandPacket;
}

void CreationCommandPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    MonsterInfoManager* InfoManager = MonsterInfoManager::GetInstance();

    // 클라(게스트)에서만 수신
    if (false == _bServer)
    {
        // 1. 최초 몬스터정보생성 셋팅일때
        if (true == IsFirst_)
        {
            // 정보추가
            InfoManager->AddMonsterInfo(MonsterInfo_);

            // 모든정보추가완료시 강제생성명령 실행
            if (TotMonsterCount_ == InfoManager->GetCurMonsterListSize())
            {
                // 강제생성명령 실행
                LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
                PlayerLevel->GuestCreateCommand();
            }
        }
        // 2. 파일로드가 필요한 몬스터정보 셋팅일때
        else
        {
            // 파일로드 시작 및 정보셋팅
            InfoManager->LoadMonsterInfoFile();

            // 로드하여 모든정보 셋팅완료시 강제생성명령 실행
            LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
            PlayerLevel->GuestCreateCommand();
        }
    }
}

CreationCommandPacket::CreationCommandPacket()
    : IsFirst_(false)
    , TotMonsterCount_(0)
    , MonsterInfo_{}
{
}

CreationCommandPacket::~CreationCommandPacket()
{
}
