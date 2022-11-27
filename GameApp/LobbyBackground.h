#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineUIRenderer.h>

using namespace std;

class LobbyBackground : public GameEngineActor
{
public:
	LobbyBackground();
	~LobbyBackground();
	LobbyBackground(const LobbyBackground& _Other) = delete;
	LobbyBackground(LobbyBackground&& _Other) noexcept = delete;
	LobbyBackground& operator=(const LobbyBackground& _Other) = delete;
	LobbyBackground& operator=(LobbyBackground&& _Other) noexcept = delete;

public:

#pragma region ·»´õ·¯ ¸ðÀ½

	GameEngineUIRenderer* BackGroundRenderer;
	GameEngineUIRenderer* Deco_CircleRenderer;
	GameEngineUIRenderer* Deco_GlowingRenderer;
	GameEngineUIRenderer* Deco_CharSelectBgRenderer;

#pragma endregion

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:

};

