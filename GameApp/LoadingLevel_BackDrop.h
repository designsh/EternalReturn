#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : BackDrop
// 용도 : 
// 설명 : 배경
class GameEngineUIRenderer;
class LoadingLevel_BackDrop : public GameEngineActor
{
public:
protected:
private:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	LoadingLevel_BackDrop();
	~LoadingLevel_BackDrop();

protected:
	LoadingLevel_BackDrop(const LoadingLevel_BackDrop& _other) = delete;
	LoadingLevel_BackDrop(LoadingLevel_BackDrop&& _other) noexcept = delete;

private:
	LoadingLevel_BackDrop& operator=(const LoadingLevel_BackDrop& _other) = delete;
	LoadingLevel_BackDrop& operator=(const LoadingLevel_BackDrop&& _other) = delete;

public:
protected:
private:
	GameEngineUIRenderer* BackDropRenderer_;
	GameEngineUIRenderer* LogoRenderer_;
	GameEngineUIRenderer* BottomPanelRenderer_;
};

