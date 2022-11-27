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
	// ���⼭ packetID_ �� �ʱ�ȭ �� �ּ���.
	virtual void initPacketID() = 0;
	virtual void userSerialize() = 0;
	virtual void userDeserialize() = 0;
	// ���⼭ ���ο� ������Ʈ�� ��ȯ �� �ּ���.
	virtual GameEnginePacketBase* getUserObject() = 0;

	// * ��Ŷ�� �� �ൿ�� �����ּ���.
	// * _bServer �Ű������� ���� �������� ����Ǵ����� ���� �����Դϴ�.
	// * _network �Ű������� ���� �ν��Ͻ��� GameEngineSocketServer Ÿ������, GameEngineSocketClient Ÿ������ �˷��� _bServer ���� üũ�ϸ� �˴ϴ�.
	// * client�� server�� Send �Լ��� ���� �ٸ��� ������ ��Ŷ�� ������ �ϴ� ��쿡 �����մϴ�.
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) = 0;

private:
	// ��Ŷ �ڵ鷯������ ȣ���մϴ�.
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
