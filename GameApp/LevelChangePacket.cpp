#include "Precompile.h"
#include "LevelChangePacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "UserGame.h"

void LevelChangePacket::SetChangeLevelName(const std::string _LevelName)
{
	LevelName_ = _LevelName;
}

void LevelChangePacket::userSerialize()
{
	serializer_ << LevelName_;
}

void LevelChangePacket::userDeserialize()
{
	serializer_ >> LevelName_;
}

void LevelChangePacket::initPacketID()
{
	SetPacketID(ePacketID::LvChangePacket);
}

GameEnginePacketBase* LevelChangePacket::getUserObject()
{
	return new LevelChangePacket;
}

void LevelChangePacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	// 서버(호스트)측에서 현재 게임에 입장한 모든 플레이어의 로딩상태를 체크하므로
	// 클라이언트(게스트)측에서 해당 패킷수신처리
	if (false == _bServer)
	{
		std::string CurLevelName = UserGame::CurrentLevel()->GetName();
		if (CurLevelName != LevelName_)
		{
			UserGame::LevelChange(LevelName_);
		}
	}
}

LevelChangePacket::LevelChangePacket()
	: LevelName_{}
{
}

LevelChangePacket::~LevelChangePacket()
{
}
