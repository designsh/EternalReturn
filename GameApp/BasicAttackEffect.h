#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineEffectRenderer;
class BasicAttackEffect : public GameEngineActor
{
public:
	BasicAttackEffect(); // default constructer 디폴트 생성자
	~BasicAttackEffect(); // default destructer 디폴트 소멸자
	BasicAttackEffect(const BasicAttackEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	BasicAttackEffect(BasicAttackEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	BasicAttackEffect& operator=(const BasicAttackEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	BasicAttackEffect& operator=(const BasicAttackEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake(const std::string& _animationName);
	void PlayFade(float _time);
	GameEngineEffectRenderer* GetAttackRenderer() { return atkRenderer_; }

	void SetFadeTime(float _time);

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
	GameEngineEffectRenderer* atkRenderer_;
	std::string animationName_;

	float fadeTime_;
	float standardTime_;
};

