#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineEffectRenderer.h>

class TestLevelBox : public GameEngineActor
{
public:
	TestLevelBox(); 
	~TestLevelBox();

	TestLevelBox(const TestLevelBox& _other) = delete; 
	TestLevelBox(TestLevelBox&& _other) = delete; 

	TestLevelBox& operator=(const TestLevelBox& _other) = delete;
	TestLevelBox& operator=(const TestLevelBox&& _other) = delete;

public:
	void SetTransparency(float _transparency);

protected:
	void Start() override;
	void Update(float _deltaTime) override;

private:
	GameEngineEffectRenderer* renderer_;
	float time_;
};

