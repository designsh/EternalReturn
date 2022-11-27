#pragma once

//#include "ePacketID.h"
#include "GameEngineSerializer.h"
#include "GameEngineSocketInterface.h"

class GameEnginePacketHandler;
class GameEnginePacketBase
{
	friend GameEnginePacketHandler;
public:
	GameEnginePacketBase();
	virtual ~GameEnginePacketBase();
	GameEnginePacketBase(const GameEnginePacketBase& _other) = delete;
	GameEnginePacketBase(GameEnginePacketBase&& _other) = delete;
	GameEnginePacketBase& operator=(const GameEnginePacketBase& _other) = delete;
	GameEnginePacketBase& operator=(const GameEnginePacketBase&& _other) = delete;

public:
	void Serialize();
	void Deserialize();

public:
	int						GetPacketID() { return packetID_; }
	int						GetPacketSize() { return packetSize_; }
	SOCKET					GetSocketSender() { return socketSender_; }
	GameEngineSerializer&	GetSerializer() { return serializer_; }

	void					SetPacketID(int _id) { packetID_ = _id; }
	void					SetPacketSize(int _size) { packetSize_ = _size; }

	GameEnginePacketBase*	GetNewObject() { return getUserObject(); }

	template<typename T>
	void					SetPacketID(T _id);

protected:
	// 여기서 packetID_ 를 초기화 해 주세요.
	virtual void initPacketID() = 0;
	virtual void userSerialize() = 0;
	virtual void userDeserialize() = 0;
	// 여기서 새로운 오브젝트를 반환 해 주세요.
	virtual GameEnginePacketBase* getUserObject() = 0;

	// * 패킷이 할 행동을 정해주세요.
	// * _bServer 매개변수는 값은 서버에서 실행되는지에 대한 여부입니다.
	// * _network 매개변수의 실제 인스턴스가 GameEngineSocketServer 타입인지, GameEngineSocketClient 타입인지 알려면 _bServer 값을 체크하면 됩니다.
	// * client와 server의 Send 함수가 각각 다르기 때문에 패킷을 재전송 하는 경우에 유용합니다.
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) = 0;

private:
	// 패킷 핸들러에서만 호출합니다.
	void setSocketSender(SOCKET _socketSender) { socketSender_ = _socketSender; }

protected:
	int packetID_;
	int packetSize_;
	GameEngineSerializer serializer_;

private:
	SOCKET socketSender_;
};

template<typename T>
inline void GameEnginePacketBase::SetPacketID(T _id)
{
	packetID_ = static_cast<int>(_id);
}
