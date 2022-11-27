#pragma once
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>

class TitleUIController;
class GameEngineImageRenderer;
class TitleIDInput;
class TitleLevel : public GameEngineLevel
{
public:
	TitleLevel();
	~TitleLevel();
	TitleLevel(const TitleLevel& _other) = delete;
	TitleLevel(TitleLevel&& _other) noexcept = delete;
	TitleLevel& operator=(const TitleLevel& _other) = delete;
	TitleLevel& operator=(const TitleLevel&& _other) = delete;

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

protected:
	void StartBooting();
	void UpdateBooting(float _DeltaTime);
	void EndBooting();

	void StartIdle();
	void UpdateIdle(float _DeltaTime);
	void EndIdle();

	void StartSelect();
	void UpdateSelect(float _DeltaTime);
	void EndSelect();

private:
	// On level change start event
	void loadResource();
	void initRenderWindow();

	// On level change end event
	void releaseResource();

private:
	const float DEFAULT_PROLOGUE_FADETIME = 3.5f;

	GameEngineFSM state_;

	TitleUIController* UIController_;
	float fadeWaitTime_;
	float fadeTime_;
	float mandatoryFadeTime_;
	float levelChangeTime_;
	
	bool isTyped_;


};

