#pragma once

#include <GameEngine/GameEngineSocketServer.h>

class GameServer : public GameEngineSocketServer
{
public:
	GameServer(); 
	~GameServer();
	GameServer(const GameServer& _other) = delete; 
	GameServer(GameServer&& _other) = delete; 
	GameServer& operator=(const GameServer& _other) = delete;
	GameServer& operator=(const GameServer&& _other) = delete;

public:
	static GameServer* GetInstance() { static GameServer instance; return &instance; }

private:

	
};

