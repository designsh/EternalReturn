#pragma once
#include <unordered_map>
#include <queue>
#include <mutex>

#include "GameEnginePacketBase.h"
//#include "ePacketID.h"

class GameEnginePacketHandler
{
public:
	GameEnginePacketHandler(bool _bServer);
	~GameEnginePacketHandler();
	GameEnginePacketHandler(const GameEnginePacketHandler& _other) = delete;
	GameEnginePacketHandler(GameEnginePacketHandler&& _other) = delete;
	GameEnginePacketHandler& operator=(const GameEnginePacketHandler& _other) = delete;
	GameEnginePacketHandler& operator=(const GameEnginePacketHandler&& _other) = delete;

public:
	// ��Ŷ�� �м��ϰ� Ǫ���մϴ�.
	void AnalyzePacketAndPush(char* _data, int _size, SOCKET _sender = 0);

	// ��Ŷ ������Ʈ�� Ǫ���մϴ�. Ǫ���� ������Ʈ�� �ڵ鷯���� �����ֱ⸦ �����մϴ�. (��, �ٸ� ������ �����ϸ� �� �˴ϴ�.)
	void PushPacket(GameEnginePacketBase* _packet);

	// Ǫ���� ��Ŷ���� ����ó���մϴ�.
	void ProcessPacket(GameEngineSocketInterface* _network);

	void AddHandler(int _packetID, GameEnginePacketBase* _packetObject);
	void RemoveHandler(int _packetID);

private:
	std::queue<GameEnginePacketBase*> packetQueue_;
	std::unordered_map<int, GameEnginePacketBase*> parents_;
	std::mutex locker_;
	bool bServer_;
};

