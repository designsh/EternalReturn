#pragma once
#include "GameEngineComponent.h"
#include "GameEngineTransform.h"
// Ό³Έν :
class GameEngineTransformComponent : public GameEngineComponent
{
	friend GameEngineActor;

public:
	// constrcuter destructer
	GameEngineTransformComponent();
	virtual ~GameEngineTransformComponent();

	// delete Function
	GameEngineTransformComponent(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent(GameEngineTransformComponent&& _Other) noexcept = delete;
	GameEngineTransformComponent& operator=(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent& operator=(GameEngineTransformComponent&& _Other) noexcept = delete;

	GameEngineTransform* GetTransform()
	{
		return &Transform_;
	}

public:
	void AttachTransform(GameEngineTransform* _Transform);
	
	virtual void Start() {};
	virtual void Update(float _DeltaTime) {};

protected:
	// virtual void InitComponent(GameEngineActor* Actor_);

private:
	GameEngineTransform Transform_;
};

