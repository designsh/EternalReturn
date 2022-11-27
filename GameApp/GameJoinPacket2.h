#pragma once
#include <GameEngine/GameEnginePacketBase.h>

#include "PlayerInfo.h"

// ���� ������ Ŭ���̾�Ʈ�� ����� 1ȸ ��ü�� ��ε�ĳ���õ˴ϴ�.
// 
// �ܼ��� "PlayerInfo�� �ϳ� ��üȭ��, ����Ʈ�� ��ƶ�" �� Execute() �ϴ� ��Ŷ�Դϴ�.


// ȣ��Ʈ �÷��̾ 1������ �����Ǳ� ������, 2������ ���������� Ŭ���̾�Ʈ���� �ο��մϴ�.

// 

class GameJoinPacket2 : public GameEnginePacketBase
{
public:
	GameJoinPacket2();
	~GameJoinPacket2();
	GameJoinPacket2(const GameJoinPacket2& _other) = delete;
	GameJoinPacket2(GameJoinPacket2&& _other) noexcept = delete;
	GameJoinPacket2& operator=(const GameJoinPacket2& _other) = delete;
	GameJoinPacket2& operator=(const GameJoinPacket2&& _other) = delete;


public:
	void SetPlayerInfo(PlayerInfo _info);
	void SetListSize(int _size) { listSize_ = _size; }

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;


private:
	PlayerInfo info_;
	int listSize_;
};

