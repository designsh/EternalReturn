#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class JackieQEffect : public GameEngineActor
{
public:
	JackieQEffect(); // default constructer 디폴트 생성자
	~JackieQEffect(); // default destructer 디폴트 소멸자
	JackieQEffect(const JackieQEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	JackieQEffect(JackieQEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	JackieQEffect& operator=(const JackieQEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	JackieQEffect& operator=(const JackieQEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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
	GameEngineEffectRenderer* zoneRenderer_;
	GameEngineEffectRenderer* impactRenderer_;

private:
	bool slashFlag_;
	float crackFadeTime_;
	float timer_;

};

