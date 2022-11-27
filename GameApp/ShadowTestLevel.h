#pragma once
#include <GameEngine/GameEngineLevel.h>

// 분류 : 레벨
// 용도 : 테스트
// 설명 : 그림자 적용 테스트 레벨
class LightActor;
class ShadowTestMap;
class ShadowTestActor;
class ShadowTestLevel : public GameEngineLevel
{
public:
	static bool ThreadLoadingEnd;

public:
protected:

private:
	void LoadBasicActorResrouce();
	void CreateBasicActor();

private:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

public:
	ShadowTestLevel();
	~ShadowTestLevel();

protected:
	ShadowTestLevel(const ShadowTestLevel& _other) = delete;
	ShadowTestLevel(ShadowTestLevel&& _other) noexcept = delete;

private:
	ShadowTestLevel& operator=(const ShadowTestLevel& _other) = delete;
	ShadowTestLevel& operator=(const ShadowTestLevel&& _other) = delete;

public:
protected:
private:
	LightActor* TestLightActor_;
	ShadowTestMap* TestShadowMap_;
	ShadowTestActor* TestShadowActor_;
};

