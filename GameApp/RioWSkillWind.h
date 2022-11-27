#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "IUnit.h"

class GameEngineEffectRenderer;
class GameEngineCollision;
// 설명 : 
class RioWSkillWind : public GameEngineActor
{
public:
	RioWSkillWind(); // default constructer 디폴트 생성자
	~RioWSkillWind(); // default destructer 디폴트 소멸자
	RioWSkillWind(const RioWSkillWind& _other) = delete; // default Copy constructer 디폴트 복사생성자
	RioWSkillWind(RioWSkillWind&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	RioWSkillWind& operator=(const RioWSkillWind& _other) = delete; // default Copy operator 디폴트 대입 연산자
	RioWSkillWind& operator=(const RioWSkillWind&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake(float _time, float _slowRatio = 0.0f);
	void SetParentIndex(int _index) { parentIndex_ = _index; }

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime);

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

	void startFade();
	void updateFade(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* zoneRenderer_;
	GameEngineEffectRenderer* impactRenderer_;
	GameEngineCollision* collision_;

private:
	float timer_;
	float fadeTimer_;
	float rotationContainer_;
	
	int parentIndex_;

	float slowRatio_;
};

