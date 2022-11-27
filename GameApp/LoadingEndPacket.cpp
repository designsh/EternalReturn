#include "Precompile.h"
#include "LoadingEndPacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include "UserGame.h"

#include "LevelChangePacket.h"

#include "LoadingLevel_LoadPercent.h"

void LoadingEndPacket::userSerialize()
{
	serializer_ << UniqueID_;
	serializer_ << LoadingEndFlag_;
}

void LoadingEndPacket::userDeserialize()
{
	serializer_ >> UniqueID_;
	serializer_ >> LoadingEndFlag_;
}

void LoadingEndPacket::initPacketID()
{
	SetPacketID(ePacketID::LoadEndPacket);
}

GameEnginePacketBase* LoadingEndPacket::getUserObject()
{
	return new LoadingEndPacket;
}

void LoadingEndPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	PlayerInfoManager* InfoManager = PlayerInfoManager::GetInstance();

	// 서버(호스트) 수신처리
	if (true == _bServer)
	{
		// 서버측에서 해당 패킷을 수신시 플레이어목록의 정보를 갱신하고,
		InfoManager->GetPlayerList()[UniqueID_].IsLoading_ = LoadingEndFlag_;

		// 로딩퍼센트 동기화를 위해 다른 클라이언트에게 패킷 전송 후
		LoadingEndPacket EndPacket;
		EndPacket.SetUniqueID(UniqueID_);
		EndPacket.SetLoadingFlag(LoadingEndFlag_);
		_network->Send(&EndPacket);

		// 나의 로딩퍼센트 갱신
		LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();

		// 관리중은 플레이어정보목록에 존재하는 모든 플레이어가 로딩완료라면 레벨체인지 패킷 송신
		// 단, 본인의 로딩완료상태 체크 포함
		// -> 서버가 로딩완료되고 난뒤 클라가 순차적 로딩되었을때 아래 패킷으로 레벨체인지
		//    해당 조건을 만족하지않는다면 서버가 로딩완료했을때 레벨체인지패킷송신
		if (true == InfoManager->AllPlayerLoadingStateCheck())
		{
			// 반환값이 true 이라면 모든 플레이어(호스트 포함) 로딩완료 되었다는 의미이므로 레벨 체인지 패킷 송신
			LevelChangePacket Packet;
			Packet.SetChangeLevelName("LumiaLevel");
			_network->Send(&Packet);

			// 패킷송신이 끝났으므로 나의 레벨도 체인지
			UserGame::LevelChange("LumiaLevel");
		}
	}
	// 클라(게스트) 수신처리
	else if (false == _bServer)
	{
		// 서버측에서 해당 패킷을 수신시 플레이어목록의 정보를 갱신하고,
		InfoManager->GetPlayerList()[UniqueID_].IsLoading_ = LoadingEndFlag_;

		// 나의 로딩퍼센트 갱신
		LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();
	}
}

LoadingEndPacket::LoadingEndPacket()
	: UniqueID_(-1)
	, LoadingEndFlag_(0)
{
}

LoadingEndPacket::~LoadingEndPacket()
{
}
