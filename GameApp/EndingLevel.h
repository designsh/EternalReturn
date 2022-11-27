#pragma once
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>

class EndingUIController;
class UI_TestMouse;
class EndingLevel : public GameEngineLevel
{
public:
	EndingLevel();
	~EndingLevel();
	EndingLevel(const EndingLevel& _other) = delete;
	EndingLevel(EndingLevel&& _other) noexcept = delete;
	EndingLevel& operator=(const EndingLevel& _other) = delete;
	EndingLevel& operator=(const EndingLevel&& _other) = delete;

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

protected:
	void startIdle();
	void updateIdle(float _DeltaTime);

	void startPopup();
	void updatePopup(float _DeltaTime);

	void startPerson();
	void updatePerson(float _DeltaTime);

	void startCreditup();
	void updateCreditup(float _DeltaTime);

private:
	EndingUIController* UIcontroller_;



	GameEngineFSM state_;

	float timer_;
};

