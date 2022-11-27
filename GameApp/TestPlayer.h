#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// Ό³Έν :
class GameEngineLevel;
class GameEngineImageRenderer;
class TestPlayer : public GameEngineActor
{
public:
	static TestPlayer* MainPlayer;

public:
	// constrcuter destructer
	TestPlayer();
	~TestPlayer();

	// delete Function
	TestPlayer(const TestPlayer& _Other) = delete;
	TestPlayer(TestPlayer&& _Other) noexcept = delete;
	TestPlayer& operator=(const TestPlayer& _Other) = delete;
	TestPlayer& operator=(TestPlayer&& _Other) noexcept = delete;

protected:
	GameEngineCollision* PlayerCollision;

	GameEngineFSM StateManager_;

	float Speed;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void TestTimeEvent();

	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
};

