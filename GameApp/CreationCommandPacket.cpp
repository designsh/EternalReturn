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

    // ���ͻ�����
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

    // ���ͻ�����
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

    // Ŭ��(�Խ�Ʈ)������ ����
    if (false == _bServer)
    {
        // 1. ���� ������������ �����϶�
        if (true == IsFirst_)
        {
            // �����߰�
            InfoManager->AddMonsterInfo(MonsterInfo_);

            // ��������߰��Ϸ�� ����������� ����
            if (TotMonsterCount_ == InfoManager->GetCurMonsterListSize())
            {
                // ����������� ����
                LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
                PlayerLevel->GuestCreateCommand();
            }
        }
        // 2. ���Ϸε尡 �ʿ��� �������� �����϶�
        else
        {
            // ���Ϸε� ���� �� ��������
            InfoManager->LoadMonsterInfoFile();

            // �ε��Ͽ� ������� ���ÿϷ�� ����������� ����
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
