#pragma once
#include <GameEngine/GameEnginePacketBase.h>


// ���� ������ Ŭ���̾�Ʈ�� ����� 1ȸ ��ü�� ��ε�ĳ���õ˴ϴ�.
// 
// �ܼ��� "PlayerInfo�� �ϳ� ��üȭ��, ����Ʈ�� ��ƶ�" �� Execute() �ϴ� ��Ŷ�Դϴ�.


// ȣ��Ʈ �÷��̾ 1������ �����Ǳ� ������, 2������ ���������� Ŭ���̾�Ʈ���� �ο��մϴ�.

// 

#include "PlayerInfo.h"

class GameJoinPacket : public GameEnginePacketBase
{
public:
	GameJoinPacket();
	~GameJoinPacket();
	GameJoinPacket(const GameJoinPacket& _other) = delete;
	GameJoinPacket(GameJoinPacket&& _other) noexcept;
	GameJoinPacket& operator=(const GameJoinPacket& _other) = delete;
	GameJoinPacket& operator=(const GameJoinPacket&& _other) = delete;


public:
	void SetOtherPlayers(std::vector<PlayerInfo> _others);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;


private:
	int othersSize_;
	std::vector<int> otherCharacter_;
	std::vector<int> otherStartPoint_;
	std::vector<int> otherIsReady_;
};

