#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class YukiREffect : public GameEngineActor
{
public:
	YukiREffect(); // default constructer 디폴트 생성자
	~YukiREffect(); // default destructer 디폴트 소멸자
	YukiREffect(const YukiREffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	YukiREffect(YukiREffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	YukiREffect& operator=(const YukiREffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	YukiREffect& operator=(const YukiREffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake();
	void PlaySlash();

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwake();
	void updateAwake(float _deltaTime);

	void startSleep();
	void updateSleep(float _deltaTime);

	void startIntermission();
	void updateIntermission(float _deltaTime);

	void startSlash();
	void updateSlash(float _deltaTime);
private:

	GameEngineFSM renderState_;
	GameEngineEffectRenderer* zoneRenderer_;
	GameEngineEffectRenderer* impactRenderer_;

private:
	float groundFadeTime_;
	float groundOutTime_;

	bool slashFlag_;
	float slashFadeTime_;
	float timer_;
};

