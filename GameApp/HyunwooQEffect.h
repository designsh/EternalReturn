#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class HyunwooQEffect : public GameEngineActor
{
public:
	HyunwooQEffect(); // default constructer 디폴트 생성자
	~HyunwooQEffect(); // default destructer 디폴트 소멸자
	HyunwooQEffect(const HyunwooQEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	HyunwooQEffect(HyunwooQEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	HyunwooQEffect& operator=(const HyunwooQEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	HyunwooQEffect& operator=(const HyunwooQEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake();

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwake();
	void updateAwake(float _deltaTime);

	void startSleep();
	void updateSleep(float _deltaTime);

private:

	GameEngineFSM renderState_;
	GameEngineEffectRenderer* groundCrackRenderer_;
	GameEngineEffectRenderer* impactRenderer_;
	GameEngineEffectRenderer* dustRenderer_;

private:
	bool isActivated_;
	float crackFadeTime_;
	float timer_;

};

