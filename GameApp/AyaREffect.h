#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class AyaREffect : public GameEngineActor
{
public:
	AyaREffect(); // default constructer 디폴트 생성자
	~AyaREffect(); // default destructer 디폴트 소멸자
	AyaREffect(const AyaREffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	AyaREffect(AyaREffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	AyaREffect& operator=(const AyaREffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	AyaREffect& operator=(const AyaREffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake();

protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

	void startExplode();
	void updateExplode(float _deltaTime);


	void startFade();
	void updateFade(float _deltaTime);
private:

	GameEngineFSM renderState_;
	GameEngineEffectRenderer* rangeRenderer_;
	GameEngineEffectRenderer* gatherRenderer_;

	GameEngineEffectRenderer* shotRenderer1_;
	GameEngineEffectRenderer* shotRenderer2_;
	GameEngineEffectRenderer* shotRenderer3_;

private:
	float timer_;
	float fadeTimer_;
	float spinCounter_;
	float4 rangeScale_;
	float4 gatherScale_;
};

