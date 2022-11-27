#pragma once
#include <GameEngine/GameEnginePacketBase.h>


// 새로 접속한 클라이언트가 생기면 1회 전체에 브로드캐스팅됩니다.
// 
// 단순히 "PlayerInfo를 하나 객체화해, 리스트에 담아라" 를 Execute() 하는 패킷입니다.


// 호스트 플레이어가 1번으로 고정되기 때문에, 2번부터 순차적으로 클라이언트에게 부여합니다.

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

