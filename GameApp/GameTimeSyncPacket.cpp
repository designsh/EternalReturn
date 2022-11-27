#include "Precompile.h"
#include "GameTimeSyncPacket.h"

#include "ePacketID.h"
#include "Enums.h"

#include "GameTimeController.h"

void GameTimeSyncPacket::userSerialize()
{
	serializer_ << Day_;
	serializer_ << static_cast<int>(Type_);
	serializer_ << GameTime_;
	serializer_ << DayAndNightTime_;
}

void GameTimeSyncPacket::userDeserialize()
{
	serializer_ >> Day_;

	int rcvType = 0;
	serializer_ >> rcvType;
	Type_ = static_cast<DayAndNightType>(rcvType);

	serializer_ >> GameTime_;
	serializer_ >> DayAndNightTime_;
}

void GameTimeSyncPacket::initPacketID()
{
	SetPacketID(ePacketID::TimeSyncPacket);
}

GameEnginePacketBase* GameTimeSyncPacket::getUserObject()
{
	return new GameTimeSyncPacket;
}

void GameTimeSyncPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	// 클라(게스트) 처리
	if (false == _bServer)
	{
		GameTimeController* gm = GameTimeController::GetInstance();
		gm->GuestUpdate(GameTime_, Type_, Day_, DayAndNightTime_);
	}
}

GameTimeSyncPacket::GameTimeSyncPacket()
	: Day_(0)
	, Type_(DayAndNightType::NONE)
	, GameTime_(0.0f)
	, DayAndNightTime_(0.0f)
{
}

GameTimeSyncPacket::~GameTimeSyncPacket()
{
}
