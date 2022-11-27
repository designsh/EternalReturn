#pragma once

#include <GameEngine/GameEngineSocketClient.h>

class GameClient : public GameEngineSocketClient
{
public:
	GameClient(); 
	~GameClient();
	GameClient(const GameClient& _other) = delete; 
	GameClient(GameClient&& _other) = delete; 
	GameClient& operator=(const GameClient& _other) = delete;
	GameClient& operator=(const GameClient&& _other) = delete;

public:
	static GameClient* GetInstance() { static GameClient instance; return &instance; }
};

