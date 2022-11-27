#pragma once
#include "GameEngineSocketInterface.h"
// 분류 : 
// 용도 : 
// 설명 : 

class GameEnginePacketBase;
class GameEnginePacketHandler;
class GameEngineSocketClient : public GameEngineSocketInterface
{
public:
	GameEngineSocketClient();
	~GameEngineSocketClient();
	GameEngineSocketClient(const GameEngineSocketClient& _other) = delete;
	GameEngineSocketClient(GameEngineSocketClient&& _other) = delete;
	GameEngineSocketClient& operator=(const GameEngineSocketClient& _other) = delete;
	GameEngineSocketClient& operator=(const GameEngineSocketClient&& _other) = delete;

public:
	// 소켓 초기화를 합니다.
	void Initialize();
	// 서버에 연결합니다.
	void Connect(const std::string& _hostIP);
	// 서버와 연결을 끊습니다.
	void Disconnect();
	// 서버에 데이터를 전송합니다.
	void Send(GameEnginePacketBase* _packet) override;
	// 특정 소켓에 데이터를 전송합니다. (사실 서버...)
	void Send(SOCKET _receiver, GameEnginePacketBase* _packet) override;
	// 저장된 패킷을 실행합니다.
	void ProcessPacket();

	void AddPacketHandler(int _packetID, GameEnginePacketBase* _packetObject);

	template<typename T>
	void AddPacketHandler(T _packetID, GameEnginePacketBase* _packetObject);

	void RemovePacketHandler(int _packetID);
	template<typename T>
	void RemovePacketHandler(T _packetID);

public:
	bool IsConnected() { return bConneted_; }

private:
	void receiveFunction(SOCKET _clientSocket);

private:
	enum { PACKET_SIZE = 1024 };
	SOCKET socket_;

	std::thread* receiveThread_;

	bool bConneted_;

	GameEnginePacketHandler* packetHandler_;
};

template<typename T>
inline void GameEngineSocketClient::AddPacketHandler(T _packetID, GameEnginePacketBase* _packetObject)
{
	AddPacketHandler(static_cast<int>(_packetID), _packetObject);
}

template<typename T>
inline void GameEngineSocketClient::RemovePacketHandler(T _packetID)
{
	RemovePacketHandler(static_cast<int>(_packetID));
}
