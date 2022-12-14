#include "PreCompile.h"
#include "GameEnginePacketHandler.h"
#include <mutex>
#include <iostream>

//#include "ChattingPacket.h"
//
//#include "ePacketID.h"

GameEnginePacketHandler::GameEnginePacketHandler(bool _bServer)
	: bServer_(_bServer)
{
}

GameEnginePacketHandler::~GameEnginePacketHandler()
{
	while (!packetQueue_.empty())
	{
		GameEnginePacketBase* p = packetQueue_.front();
		delete p;
		packetQueue_.pop();
	}

	for (auto p : parents_)
	{
		delete p.second;
	}

	parents_.clear();
}

void GameEnginePacketHandler::AnalyzePacketAndPush(char* _data, int _size, SOCKET _sender)
{
	GameEnginePacketBase* analyzedPacket = nullptr;

	int packetID = -1;
	int packetSize = 0;

	// 패킷 헤더를 가져옵니다.
	memcpy(&packetID, _data, sizeof(int));
	memcpy(&packetSize, _data + sizeof(int), sizeof(int));


	// 부모 맵에서 찾으면 객체를 생성해 주는 구조 입니다.
	auto findIter = parents_.find(packetID);
	if (findIter != parents_.end())
	{
		analyzedPacket = findIter->second->GetNewObject();
	}

	// 부모 맵에서 찾아왔다면 
	if (nullptr != analyzedPacket)
	{
		analyzedPacket->GetSerializer().SetDataPtr(_data, _size);
		analyzedPacket->SetPacketID(packetID);
		if (_sender != 0)
		{
			analyzedPacket->setSocketSender(_sender);
		}
		PushPacket(analyzedPacket);
	}
}

void GameEnginePacketHandler::PushPacket(GameEnginePacketBase* _packet)
{
	locker_.lock();
	packetQueue_.push(_packet);
	locker_.unlock();
}

void GameEnginePacketHandler::ProcessPacket(GameEngineSocketInterface* _network)
{
	// 패킷 처리중에 패킷을 추가하면 안 되므로 잠근다.
	std::lock_guard<std::mutex> lg(locker_);
	while (!packetQueue_.empty())
	{
		GameEnginePacketBase* packet = packetQueue_.front();
		packetQueue_.pop();

		//GameEngineTime time;
		//time.TimeCheckReset();
		//time.TimeCheck();
		//GameEngineDebug::OutPutDebugString("패킷 역직렬화 시작");

		packet->Deserialize();

		//time.TimeCheck();
		//GameEngineDebug::OutPutDebugString("패킷 역직렬화 종료 : " + std::to_string(time.GetDeltaTime()) + "초\n");
		//GameEngineDebug::OutPutDebugString("패킷 ID : " + std::to_string(packet->GetPacketID()) + "\n");

		//GameEngineDebug::OutPutDebugString("패킷 처리 시작 " + std::string("패킷 ID : ") + std::to_string(packet->GetPacketID()) + "\n");
		packet->execute(packet->socketSender_, _network, bServer_);
		//time.TimeCheck();
		//GameEngineDebug::OutPutDebugString("패킷 처리 종료 " + std::string("패킷 ID : ") + std::to_string(packet->GetPacketID()) + ", 시간 : " + std::to_string(time.GetDeltaTime()) + "초\n");
		delete packet;
	}
}

void GameEnginePacketHandler::AddHandler(int _packetID, GameEnginePacketBase* _packetObject)
{
	auto findIter = parents_.find(_packetID);

	if (findIter != parents_.end())
	{
		return;
	}

	parents_[_packetID] = _packetObject;
}

void GameEnginePacketHandler::RemoveHandler(int _packetID)
{
	auto findHandler = parents_.find(_packetID);

	if (findHandler != parents_.end())
	{
		parents_.erase(findHandler);
	}
}
