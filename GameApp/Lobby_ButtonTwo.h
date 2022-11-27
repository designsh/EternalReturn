#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineUIRenderer.h>

using namespace std;

class Lobby_ButtonTwo : public GameEngineActor
{
public:
	Lobby_ButtonTwo();
	~Lobby_ButtonTwo();

	bool MouseCollisionCheck();
	void SetText(const std::string& _text);
	void SetImageByIndex(int _index);
public:
	GameEngineUIRenderer* ButtonTwoRenderer;
	GameEngineCollision* ButtonTwoCollision;



protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_ButtonTwo(const Lobby_ButtonTwo& _Other) = delete;
	Lobby_ButtonTwo(Lobby_ButtonTwo&& _Other) noexcept = delete;
	Lobby_ButtonTwo& operator=(const Lobby_ButtonTwo& _Other) = delete;
	Lobby_ButtonTwo& operator=(Lobby_ButtonTwo&& _Other) noexcept = delete;
};

