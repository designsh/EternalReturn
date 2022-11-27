#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineEffectRenderer;
class YukiQEffect : public GameEngineActor
{
public:
	YukiQEffect(); // default constructer 디폴트 생성자
	~YukiQEffect(); // default destructer 디폴트 소멸자
	YukiQEffect(const YukiQEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	YukiQEffect(YukiQEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	YukiQEffect& operator=(const YukiQEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	YukiQEffect& operator=(const YukiQEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake();
	void Stop();

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* kiGatheringRenderer_;
	GameEngineEffectRenderer* kiCoreRenderer_;
private:
	float timer_;
	float4 targetPos_;

	float rotationDegree_;
	float4 scaleContainer_;
};