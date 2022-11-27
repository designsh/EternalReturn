#pragma once
#include "GameEngineSocketInterface.h"
// �з� : 
// �뵵 : 
// ���� : 

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

	void Initialize();			// ���� �ʱ�ȭ�� �մϴ�.
	void OpenServer();			// ���� ������ �ʱ�ȭ�ϰ�, accept �Լ��� ������ �����带 �����մϴ�.
	void CloseServer();			// ���� ������ �ݰ�, ��� ������ �����ϴ�.


	void ProcessPacket();									// Ŭ���̾�Ʈ���� ���� ��Ŷ���� �����մϴ�.
	void Send(GameEnginePacketBase* _packet) override;		// ����� Ŭ���̾�Ʈ�鿡�� ��Ŷ�� �����մϴ�.
	void Send(GameEnginePacketBase* _packet, int _index);		// Ư�� Ŭ���̾�Ʈ���Ը� ��Ŷ�� �����մϴ�.
	// Ư�� ���Ͽ� �����͸� �����մϴ�. (��Ŷ�� ���� Ŭ���̾�Ʈ �� �ϳ��� �̴ϴ�.)
	void Send(SOCKET _receiver, GameEnginePacketBase* _packet) override;

	// �ν��ؾ� �� ��Ŷ�� ����(���� ������Ʈ, ä�� ��) �� �νĽ�Ű�� �Լ��Դϴ�.
	void AddPacketHandler(int _packetID, GameEnginePacketBase* _packetObject);
	template<typename T>
	void AddPacketHandler(T _packetID, GameEnginePacketBase* _packetObject);

	void RemovePacketHandler(int _packetID);
	template<typename T>
	void RemovePacketHandler(T _packetID);

	// 0829 ������
	int GetClientSocketSize()
	{
		return static_cast<int>(clientSocketList_.size());
	}

public:
	bool IsOpened() { return bOpen_; }
	void TestOpen() { bOpen_ = true; } // TODO : delete it
	//0830 ������
	//size_t GetServerPlayerListSize() { return serverPlayerList_.size(); }
	//std::vector<PlayerInfo> GetServerPlayerList() { return serverPlayerList_; }
	//std::vector<SOCKET> GetSocketList() { return clientSocketList_; }

private:
	// Ŭ���̾�Ʈ�� ������ �ޱ� ���� �Լ�. ������� ó���˴ϴ�.
	void acceptFunction();
	// Ŭ���̾�Ʈ�� ������������ �ޱ� ���� �Լ�. n ���� �����带 �����մϴ�.(n == ����� Ŭ���̾�Ʈ ��)
	void receiveFunction(SOCKET _clientSocket);

private:
	enum { PACKET_SIZE = 1024 };

	std::vector<SOCKET> clientSocketList_; 
	std::map<SOCKET, std::thread> clientReceiveThreadList_;

	bool bOpen_;
	SOCKET serverSocket_;
	std::thread* acceptThread_;

	std::mutex locker_; // ����ȭ�� ���ؽ�

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
