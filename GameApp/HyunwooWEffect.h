#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineEffectRenderer;
class HyunwooWEffect : public GameEngineActor
{
public:
	HyunwooWEffect(); // default constructer 디폴트 생성자
	~HyunwooWEffect(); // default destructer 디폴트 소멸자
	HyunwooWEffect(const HyunwooWEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	HyunwooWEffect(HyunwooWEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	HyunwooWEffect& operator=(const HyunwooWEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	HyunwooWEffect& operator=(const HyunwooWEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

	void startFade();
	void updateFade(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* maskRenderer_;
	GameEngineEffectRenderer* shieldRenderer_;

private:
	float awakeTimer_;
	float fadeTimer_;


};

