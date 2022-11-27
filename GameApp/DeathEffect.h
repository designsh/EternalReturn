#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineEffectRenderer;
class DeathEffect : public GameEngineActor
{
public:
	DeathEffect(); // default constructer 디폴트 생성자
	~DeathEffect(); // default destructer 디폴트 소멸자
	DeathEffect(const DeathEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	DeathEffect(DeathEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	DeathEffect& operator=(const DeathEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	DeathEffect& operator=(const DeathEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake(bool _isProhibitedArea);

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
	GameEngineEffectRenderer* barBreakRenderer_;
	GameEngineEffectRenderer* barEffectRenderer_;
	GameEngineEffectRenderer* restrictionExplodeRenderer_;

	float barBreakTimer_;
	float barEffectTimer_;
	float4 scaleContainer_;
	bool isExplosion_;
};

