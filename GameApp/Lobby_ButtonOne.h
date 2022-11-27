#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineUIRenderer.h>

using namespace std;

class Lobby_ButtonOne : public GameEngineActor
{
public:
	Lobby_ButtonOne();
	~Lobby_ButtonOne();

	bool MouseCollisionCheck();
	void SetText(const std::string& _text);
	void SetImageByIndex(int _index);

public:
	GameEngineUIRenderer* ButtonOneRenderer;
	GameEngineCollision* ButtonOneCollision;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_ButtonOne(const Lobby_ButtonOne& _Other) = delete;
	Lobby_ButtonOne(Lobby_ButtonOne&& _Other) noexcept = delete;
	Lobby_ButtonOne& operator=(const Lobby_ButtonOne& _Other) = delete;
	Lobby_ButtonOne& operator=(Lobby_ButtonOne&& _Other) noexcept = delete;
};

