#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineCollision.h>

class GameEngineTransformComponent;
class TestOBB : public GameEngineActor
{
public:
	TestOBB(); 
	~TestOBB();

	TestOBB(const TestOBB& _other) = delete; 
	TestOBB(TestOBB&& _other) = delete; 

	TestOBB& operator=(const TestOBB& _other) = delete;
	TestOBB& operator=(const TestOBB&& _other) = delete;

public:
	virtual void Start();
	virtual void Update(float _deltaTime);


private:
	GameEngineCollision* collision_;
	GameEngineCollision* collision2_;
	GameEngineTransformComponent* tc_;
	GameEngineTransformComponent* tc2_;
};

