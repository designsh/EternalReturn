#include "PreCompile.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"

GameEngineActor::GameEngineActor() 
	: level_(nullptr)
	, DeathTime_(-1.0f)
	, IsDestroyed_(nullptr)
	, NextLevelMove_(nullptr)
{
}

GameEngineActor::~GameEngineActor() 
{
	for (auto& Component : ComponentList_)
	{
		if (nullptr != Component)
		{
			delete Component;
			Component = nullptr;
		}
	}

	for (auto& TransformComponent : TransformComponentList_)
	{
		if (nullptr != TransformComponent)
		{
			delete TransformComponent;
			TransformComponent = nullptr;
		}
	}
}

void GameEngineActor::ComponentRelease()
{
	if (false == ComponentList_.empty())
	{
		std::list<GameEngineComponent*>::iterator BeginIter = ComponentList_.begin();
		std::list<GameEngineComponent*>::iterator EndIter = ComponentList_.end();

		for (; BeginIter != EndIter; )
		{
			if (true == (* BeginIter)->IsDeath())
			{
				delete* BeginIter;
				*BeginIter = nullptr;

				BeginIter = ComponentList_.erase(BeginIter);
				continue;
			}

			++BeginIter;
		}
	}

	if (false == TransformComponentList_.empty())
	{
		std::list<GameEngineTransformComponent*>::iterator BeginIter = TransformComponentList_.begin();
		std::list<GameEngineTransformComponent*>::iterator EndIter = TransformComponentList_.end();

		for (; BeginIter != EndIter; )
		{
			if (true == (*BeginIter)->IsDeath())
			{
				delete* BeginIter;
				*BeginIter = nullptr;

				BeginIter = TransformComponentList_.erase(BeginIter);
				continue;
			}

			++BeginIter;
		}
	}

}

void GameEngineActor::SetLevel(GameEngineLevel* _Level) 
{
	level_ = _Level;
}

void GameEngineActor::UpdateComponent(float _DeltaTime)
{
	for (GameEngineComponent* Component : ComponentList_)
	{
		Component->Update(_DeltaTime);
	}

	for (GameEngineTransformComponent* Component : TransformComponentList_)
	{
		Component->Update(_DeltaTime);
	}
}

void GameEngineActor::ReleaseUpdate(float _DeltaTime)
{
	if (false == IsDestroyed_)
	{
		return;
	}

	DeathTime_ -= _DeltaTime;

	if (0.0f >= DeathTime_)
	{
		ReleaseEvent();
		Death();
	}
}

