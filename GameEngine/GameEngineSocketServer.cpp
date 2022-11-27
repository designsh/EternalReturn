#include "PreCompile.h"
#include "GameEngineSocketServer.h"
#include "GameEnginePacketHandler.h"
#include "GameEnginePacketBase.h"

GameEngineSocketServer::GameEngineSocketServer() // default constructer 디폴트 생성자
	: serverSocket_(0)
	, acceptThread_(nullptr)
	, bOpen_(false)
	, packetHandler_(nullptr)
{

}

GameEngineSocketServer::~GameEngineSocketServer() // default destructer 디폴트 소멸자
{
	// 소멸자 호출 시 서버는 자동으로 닫힙니다.
	CloseServer();
}

void GameEngineSocketServer::Initialize()
{
	WSAData wsaData;
	int errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	GameEngineDebug::OutPutDebugString("winSock 초기화를 실시합니다.\n");

	if (SOCKET_ERROR == errorCode)
	{
		GameEngineDebug::MsgBoxError("WSAStartup 실행에 실패했습니다.\n");
		return;
	}
}


void GameEngineSocketServer::OpenServer()
{
	GameEngineDebug::OutPutDebugString("서버를 엽니다.\n");
	if (serverSocket_ != 0)
	{
		GameEngineDebug::MsgBoxError("서버가 이미 열려 있는 상태에서 다시 서버 오픈을 시도했습니다.");
		return;
	}

	serverSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == serverSocket_)
	{
		GameEngineDebug::MsgBoxError("서버 소켓 생성에 실패했습니다.");
		return;
	}

	SOCKADDR_IN address = { 0, };
	address.sin_family = AF_INET; // ipv4
	address.sin_port = htons(30001);
	address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &address.sin_addr)) // 로컬 호스트
	{
		GameEngineDebug::MsgBoxError("주소 초기화에 실패했습니다.");
		return;
	}

	if (SOCKET_ERROR == bind(serverSocket_, (const sockaddr*)&address, sizeof(SOCKADDR_IN)))
	{
		GameEngineDebug::MsgBoxError("소켓에 IP주소와 포트를 바인드 할 수 없습니다.");
		return;
	}

	if (SOCKET_ERROR == listen(serverSocket_, 512))
	{
		GameEngineDebug::MsgBoxError("서버를 리슨 상태로 전환할 수 없습니다.");
		return;
	}

	GameEngineDebug::OutPutDebugString("서버 오픈이 성공적으로 완료되었습니다.\n");

	// 0830 박종원
	//PlayerInfo info;
	//info.character_ = -1;
	//info.startPoint_ = -1;
	//info.isReady_ = false;

	//serverPlayerList_.push_back(info);
	//myPlayerNumber_ = 1;

	bOpen_ = true;
	packetHandler_ = new GameEnginePacketHandler(true);
	acceptThread_ = new std::thread(std::bind(&GameEngineSocketServer::acceptFunction, this));
}

void GameEngineSocketServer::CloseServer()
{
	// 서버를 닫는 도중, 클라이언트의 자체적인 스레드 삭제를 막기 위해 Lock() 으로 코드블록을 만든다.
	locker_.lock();
	bOpen_ = false;

	if (nullptr != acceptThread_)
	{
		closesocket(serverSocket_);
		serverSocket_ = 0;
		acceptThread_->join();
		delete acceptThread_;
		acceptThread_ = nullptr;
	}

	for (SOCKET s : clientSocketList_)
	{
		closesocket(s);
	}

	clientSocketList_.clear();

	locker_.unlock();

	auto startIter = clientReceiveThreadList_.begin();
	auto endIter = clientReceiveThreadList_.end();

	while (startIter != endIter)
	{
		if (startIter->second.joinable())
		{
			startIter->second.join();
		}
		startIter++;
	}

	if (nullptr != packetHandler_)
	{
		delete packetHandler_;
		packetHandler_ = nullptr;
	}
}

void GameEngineSocketServer::ProcessPacket()
{
	if (nullptr != packetHandler_)
	{
		packetHandler_->ProcessPacket(this);
	}
}

void GameEngineSocketServer::Send(GameEnginePacketBase* _packet)
{
	if (_packet->GetSerializer().GetOffSet() == 0)
	{
		_packet->Serialize();
	}

	char sendData[PACKET_SIZE] = { 0, };

	memcpy(sendData, _packet->GetSerializer().GetDataPtr(), _packet->GetSerializer().GetOffSet());

	locker_.lock();
	for (auto clientSocket : clientSocketList_)
	{
		send(clientSocket, sendData, PACKET_SIZE, 0);
	}
	locker_.unlock();
}

// 구 버전 Send()
// 인덱스 값을 전달받아 특정 소켓에게만 전달하는 오버로딩 함수
// (하단의 소켓 값을 직접 받는 Send() 로 대체 예정
void GameEngineSocketServer::Send(GameEnginePacketBase* _packet, int _index)
{
	if (clientSocketList_.size() < _index)
	{
		GameEngineDebug::MsgBoxError("서버에서 Send 를 했는데 잘못된 클라이언트 인덱스가 들어갔습니다.");
	}

	if (_packet->GetSerializer().GetOffSet() == 0)
	{
		_packet->Serialize();
	}

	char sendData[PACKET_SIZE] = { 0, };

	memcpy(sendData, _packet->GetSerializer().GetDataPtr(), _packet->GetSerializer().GetOffSet());

	locker_.lock();
	send(clientSocketList_[_index-1], sendData, PACKET_SIZE, 0);
	locker_.unlock();
}

void GameEngineSocketServer::AddPacketHandler(int _packetID, GameEnginePacketBase* _packetObject)
{
	if (packetHandler_ != nullptr)
	{
		packetHandler_->AddHandler(_packetID, _packetObject);
	}
}

void GameEngineSocketServer::RemovePacketHandler(int _packetID)
{
	packetHandler_->RemoveHandler(_packetID);
}

void GameEngineSocketServer::Send(SOCKET _receiver, GameEnginePacketBase* _packet)
{
	if (0 == _receiver)
	{
		GameEngineDebug::OutPutDebugString("전송받을 소켓이 비어있습니다.\n");
		return;
	}

	if (_packet->GetSerializer().GetOffSet() == 0)
	{
		_packet->Serialize();
	}

	char sendData[PACKET_SIZE] = { 0, };

	memcpy(sendData, _packet->GetSerializer().GetDataPtr(), _packet->GetSerializer().GetOffSet());

	send(_receiver, sendData, PACKET_SIZE, 0);
}

void GameEngineSocketServer::acceptFunction()
{
	// 새로운 클라이언트가 들어왔을 때 Accept 를 해 주는 단계입니다.

	SOCKADDR_IN userAddress = { 0, };
	int size = sizeof(SOCKADDR_IN);

	while (true)
	{
		SOCKET socketNewUser = accept(serverSocket_, (sockaddr*)&userAddress, &size);
		if (INVALID_SOCKET == socketNewUser)
		{
			return;
		}
		
		GameEngineDebug::OutPutDebugString("새로운 클라이언트가 접속했습니다.\n");
		// 새로운 클라이언트의 접속을 확인한 순간부터
		// 클라이언트의 최소 정보인 순번까지는 게임앱에서 알고 있어야 한다...
		//

		locker_.lock();
		clientSocketList_.push_back(socketNewUser);

		//// 0830 박종원
		//PlayerInfo info;
		//info.character_ = -1;
		//info.startPoint_ = -1;
		//info.isReady_ = false;
		//
		//serverPlayerList_.push_back(info);
		////(std::to_string(clientSocketList_.size() + 1));


		std::thread newReceiveThread(std::bind(&GameEngineSocketServer::receiveFunction, this, socketNewUser));
		clientReceiveThreadList_.insert(std::pair<SOCKET, std::thread>(socketNewUser, std::move(newReceiveThread)));

		locker_.unlock();
	}
}

void GameEngineSocketServer::receiveFunction(SOCKET _clientSocket)
{
	char packet[PACKET_SIZE] = { 0 };

	while (true)
	{
		int result = recv(_clientSocket, packet, sizeof(packet), 0);

		if (0 < result)
		{
			packetHandler_->AnalyzePacketAndPush(packet, result, _clientSocket);
		}
		else if (SOCKET_ERROR == result)
		{
			GameEngineDebug::OutPutDebugString("클라이언트의 접속이 종료되었습니다.\n");

			locker_.lock();
			if (bOpen_ == false)
			{
				locker_.unlock();
				return;
			}

			size_t clientCount = clientSocketList_.size();

			for (size_t i = 0; i < clientCount; i++)
			{
				if (_clientSocket == clientSocketList_[i])
				{
					clientSocketList_[i] = clientSocketList_.back();
					clientSocketList_.pop_back();
					break;
				}
			}


			auto findThreadIter = clientReceiveThreadList_.find(_clientSocket);
			if (findThreadIter != clientReceiveThreadList_.end())
			{
				findThreadIter->second.detach();

				clientReceiveThreadList_.erase(_clientSocket);

			}

			// 0830 박종원
			//serverPlayerList_.pop_back();


			locker_.unlock();
			return;
		}

		ZeroMemory(packet, PACKET_SIZE);
	}
}