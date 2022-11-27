#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineRenderTarget;
class GameEngineFBXRenderer;
class GameEngineShadowRenderer;
class GameEnginePreprocessingRenderer;
class ShadowTestActor : public GameEngineActor
{
public:
	static void TestResourceLoad();

public:
protected:
private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	ShadowTestActor();
	~ShadowTestActor();

protected:
	ShadowTestActor(const ShadowTestActor& _other) = delete;
	ShadowTestActor(ShadowTestActor&& _other) noexcept = delete;

private:
	ShadowTestActor& operator=(const ShadowTestActor& _other) = delete;
	ShadowTestActor& operator=(const ShadowTestActor&& _other) = delete;

public:
protected:
private:
	bool IsChange_;
	bool First_;

private:
	GameEngineFBXRenderer* TestBaseRenderer_;
	GameEngineShadowRenderer* ShadowRenderer_;
	GameEngineRenderTarget* ShadowRenderTarget_;
};

