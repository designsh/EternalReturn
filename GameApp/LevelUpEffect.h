#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineUIRenderer;
class GameEngineEffectRenderer;
class LevelUpEffect : public GameEngineActor
{
public:
	LevelUpEffect(); // default constructer 디폴트 생성자
	~LevelUpEffect(); // default destructer 디폴트 소멸자
	LevelUpEffect(const LevelUpEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	LevelUpEffect(LevelUpEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	LevelUpEffect& operator=(const LevelUpEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	LevelUpEffect& operator=(const LevelUpEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake();

	void Hide(bool _tf);

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwake();
	void updateAwake(float _deltaTime);

	void startSleep();
	void updateSleep(float _deltaTime);

	void startFade();
	void updateFade(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* levelUpRenderer_;
	GameEngineEffectRenderer* glowLineRenderer_;
	GameEngineEffectRenderer* ringRenderer_;

private:
	float timer_;
	float glowTimer_;
	float rotationDegree_;
	float4 scaleContainer_;
};

