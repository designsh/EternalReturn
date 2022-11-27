#pragma once
#include <typeinfo>
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineTransform.h"
#include "GameEngineTransformComponent.h"

// 설명 :
class GameEngineComponent;
class GameEngineLevel;
class GameEngineActor : public GameEngineObjectNameBase
{
	friend GameEngineLevel;
public:
	GameEngineActor();
	~GameEngineActor();
	GameEngineActor(const GameEngineActor& _Other) = delete;
	GameEngineActor(GameEngineActor&& _Other) noexcept = delete;
	GameEngineActor& operator=(const GameEngineActor& _Other) = delete;
	GameEngineActor& operator=(GameEngineActor&& _Other) noexcept = delete;

	bool NextLevelMove_;
	bool IsDestroyed_;
	float DeathTime_;

	void MoveNextOn()
	{
		NextLevelMove_ = true;
	}

	GameEngineLevel* GetLevel()
	{
		return level_;
	}

	template<typename LevelType>
	LevelType* GetLevelConvert()
	{
		return dynamic_cast<LevelType*>(level_);
	}

	void Release(float _Time = 0.0f)
	{
		if (0.0f >= _Time)
		{
			Death();
		}
		else
		{
			IsDestroyed_ = true;
			DeathTime_ = _Time;
		}
	}

	template<typename ComponentType>
	ComponentType* CreateComponent(int _Order = 0);

	template<typename ComponentType>
	ComponentType* CreateTransformComponent(int _Order = 0);

	template<typename ComponentType>
	ComponentType* CreateTransformComponent(GameEngineTransform* _Transform, int _Order = 0);

	template<typename ComType>
	std::vector<ComType*> GetTransformComponent();

protected:
	virtual void Start() {}
	virtual void Update(float _DeltaTime) {}
	virtual void ReleaseEvent() {}
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) {}
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) {}


////////////////////////

public:
	GameEngineTransform* GetTransform()
	{
		return &transform_;
	}

protected:
	GameEngineTransform transform_;
	GameEngineLevel* level_;

private:
	std::list<GameEngineComponent*> ComponentList_;

	std::list<GameEngineTransformComponent*> TransformComponentList_;

	void SetLevel(GameEngineLevel* Level);

	void UpdateComponent(float _DeltaTime);

	void ComponentRelease();

	void ReleaseUpdate(float _DeltaTime);
};

template<typename ComponentType>
inline ComponentType* GameEngineActor::CreateComponent(int _Order)
{
	GameEngineComponent* NewComponent = new ComponentType();
	NewComponent->SetParent(this);
	NewComponent->SetOrder(_Order);
	NewComponent->InitComponent(this);
	ComponentList_.push_back(NewComponent);
	NewComponent->Start();
	return dynamic_cast<ComponentType*>(NewComponent);
}

template<typename ComponentType>
inline ComponentType* GameEngineActor::CreateTransformComponent(int _Order)
{
	// 업캐스팅을 이용해서 컴파일 에러를 낼것이다.
	GameEngineTransformComponent* NewComponent = new ComponentType();
	NewComponent->SetParent(this);
	NewComponent->SetOrder(_Order);
	NewComponent->InitComponent(this);
	NewComponent->AttachTransform(GetTransform());
	TransformComponentList_.push_back(NewComponent);

	NewComponent->Start();
	return dynamic_cast<ComponentType*>(NewComponent);
}

template<typename ComponentType>
inline ComponentType* GameEngineActor::CreateTransformComponent(GameEngineTransform* _Transform, int _Order)
{
	// 업캐스팅을 이용해서 컴파일 에러를 낼것이다.
	GameEngineTransformComponent* NewComponent = new ComponentType();

	NewComponent->SetOrder(_Order);
	NewComponent->InitComponent(this);

	if (nullptr == _Transform)
	{
		NewComponent->SetParent(nullptr);
	}
	else
	{
		NewComponent->SetParent(this);
		NewComponent->AttachTransform(_Transform);
	}

	TransformComponentList_.push_back(NewComponent);

	NewComponent->Start();
	return dynamic_cast<ComponentType*>(NewComponent);
}

template<typename ComType>
inline std::vector<ComType*> GameEngineActor::GetTransformComponent()
{
	std::vector<ComType*> Return;

	for (auto& TransformComponent : TransformComponentList_)
	{
		if (typeid(*TransformComponent) != typeid(ComType))
		{
			continue;
		}

		Return.push_back(dynamic_cast<ComType*>(TransformComponent));
	}

	return Return;
}
