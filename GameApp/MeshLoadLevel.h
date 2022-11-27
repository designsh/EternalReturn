#pragma once
#include <GameEngine/GameEngineLevel.h>

// Ό³Έν :
class MouseActor;
class MeshLoadLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	MeshLoadLevel();
	~MeshLoadLevel();

	// delete Function
	MeshLoadLevel(const MeshLoadLevel& _Other) = delete;
	MeshLoadLevel(MeshLoadLevel&& _Other) noexcept = delete;
	MeshLoadLevel& operator=(const MeshLoadLevel& _Other) = delete;
	MeshLoadLevel& operator=(MeshLoadLevel&& _Other) noexcept = delete;

	MouseActor* MActor; 
	class PostFade* FadeEffect;

protected:

	class GameEngineFBXWindow* FBXWindow;

private:
	void CreateActorLevel();

	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

};

