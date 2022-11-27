#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 액터
// 용도 : 테스트
// 설명 : 
class GameEngineFBXRenderer;
class GameEnginePreprocessingRenderer;
class OutLineTestActor : public GameEngineActor
{
public:
	static void TestResourceLoad();

protected:
private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	OutLineTestActor();
	~OutLineTestActor();

protected:
	OutLineTestActor(const OutLineTestActor& _other) = delete;
	OutLineTestActor(OutLineTestActor&& _other) = delete;

private:
	OutLineTestActor& operator=(const OutLineTestActor& _other) = delete;
	OutLineTestActor& operator=(const OutLineTestActor&& _other) = delete;

public:
protected:
private:
	bool IsChange_;

private:
	GameEngineFBXRenderer* TestBaseRenderer_;
	GameEnginePreprocessingRenderer* TestRenderer_;
};

