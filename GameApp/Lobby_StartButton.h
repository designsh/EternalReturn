#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineUIRenderer.h>

using namespace std;

class Lobby_StartButton : public GameEngineActor
{
public:
	Lobby_StartButton();
	~Lobby_StartButton();

	bool MouseCollisionCheck();
	void CharSelectSwitch();
public:
	GameEngineUIRenderer* ButtonRenderer;
	GameEngineCollision* ButtonCollision;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_StartButton(const Lobby_StartButton& _Other) = delete;
	Lobby_StartButton(Lobby_StartButton&& _Other) noexcept = delete;
	Lobby_StartButton& operator=(const Lobby_StartButton& _Other) = delete;
	Lobby_StartButton& operator=(Lobby_StartButton&& _Other) noexcept = delete;
};

