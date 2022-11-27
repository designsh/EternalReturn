#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineUIRenderer.h>
#include "Enums.h"

using namespace std;

class Lobby_PortraitBg : public GameEngineActor
{
public:
	Lobby_PortraitBg();
	~Lobby_PortraitBg();

	void SetChar(JobType _Character);
	JobType GetChar()
	{
		return Char;
	}

	void SelectOn();
	void SelectOff();

	GameEngineCollision* GetCollision()
	{
		return MouseCollision;
	}

	bool isSelected()
	{
		return SelectImageRenderer->IsUpdate();
	}




public:
	GameEngineUIRenderer* BasicImageRenderer;
	GameEngineUIRenderer* SelectImageRenderer;
	GameEngineUIRenderer* CharPortraitRenderer;
	GameEngineCollision* MouseCollision;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_PortraitBg(const Lobby_PortraitBg& _Other) = delete;
	Lobby_PortraitBg(Lobby_PortraitBg&& _Other) noexcept = delete;
	Lobby_PortraitBg& operator=(const Lobby_PortraitBg& _Other) = delete;
	Lobby_PortraitBg& operator=(Lobby_PortraitBg&& _Other) noexcept = delete;

private:
	JobType Char;
};

