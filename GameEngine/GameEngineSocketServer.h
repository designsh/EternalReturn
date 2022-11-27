#pragma once
#include "GameEngineSocketInterface.h"
// 분류 : 
// 용도 : 
// 설명 : 

class GameEnginePacketBase;
class GameEnginePacketHandler;
class GameEngineSocketServer : public GameEngineSocketInterface
{
public:
	GameEngineSocketServer();
	~GameEngineSocketServer();
	GameEngineSocketServer(const GameEngineSocketServer& _other) = delete;
	GameEngineSocketServer(GameEngineSocketServer&& _other) = delete;
	GameEngineSocketServer& operator=(const GameEngineSocketServer& _rhs) = delete;
	GameEngineSocketServer& operator=(GameEngineSocketServer&& _rhs) = delete;

public:

	void Initialize();			// 서버 초기화를 합니다.
	void OpenServer();			// 서버 소켓을 초기화하고, accept 함수를 실행할 스레드를 생성합니다.
	void CloseServer();			// 서버 소켓을 닫고, 모든 연결을 끊습니다.


	void ProcessPacket();									// 클라이언트에서 오는 패킷들을 감지합니다.
	void Send(GameEnginePacketBase* _packet) override;		// 연결된 클라이언트들에게 패킷을 전송합니다.
	void Send(GameEnginePacketBase* _packet, int _index);		// 특정 클라이언트에게만 패킷을 전송합니다.
	// 특정 소켓에 데이터를 전송합니다. (패킷을 보낸 클라이언트 중 하나일 겁니다.)
	void Send(SOCKET _receiver, GameEnginePacketBase* _packet) override;

	// 인식해야 할 패킷의 유형(액터 업데이트, 채팅 등) 을 인식시키는 함수입니다.
	void AddPacketHandler(int _packetID, GameEnginePacketBase* _packetObject);
	template<typename T>
	void AddPacketHandler(T _packetID, GameEnginePacketBase* _packetObject);

	void RemovePacketHandler(int _packetID);
	template<typename T>
	void RemovePacketHandler(T _packetID);

	// 0829 박종원
	int GetClientSocketSize()
	{
		return static_cast<int>(clientSocketList_.size());
	}

public:
	bool IsOpened() { return bOpen_; }
	void TestOpen() { bOpen_ = true; } // TODO : delete it
	//0830 박종원
	//size_t GetServerPlayerListSize() { return serverPlayerList_.size(); }
	//std::vector<PlayerInfo> GetServerPlayerList() { return serverPlayerList_; }
	//std::vector<SOCKET> GetSocketList() { return clientSocketList_; }

private:
	// 클라이언트의 연결을 받기 위한 함수. 스레드로 처리됩니다.
	void acceptFunction();
	// 클라이언트의 데이터전송을 받기 위한 함수. n 개의 스레드를 생성합니다.(n == 연결된 클라이언트 수)
	void receiveFunction(SOCKET _clientSocket);

private:
	enum { PACKET_SIZE = 1024 };

	std::vector<SOCKET> clientSocketList_; 
	std::map<SOCKET, std::thread> clientReceiveThreadList_;

	bool bOpen_;
	SOCKET serverSocket_;
	std::thread* acceptThread_;

	std::mutex locker_; // 동기화용 뮤텍스

	GameEnginePacketHandler* packetHandler_;
};

template<typename T>
inline void GameEngineSocketServer::AddPacketHandler(T _packetID, GameEnginePacketBase* _packetObject)
{
	AddPacketHandler(static_cast<int>(_packetID), _packetObject);
}

template<typename T>
inline void GameEngineSocketServer::RemovePacketHandler(T _packetID)
{
	RemovePacketHandler(static_cast<int>(_packetID));
}
