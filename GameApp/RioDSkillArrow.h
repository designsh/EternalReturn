#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngineBase/GameEngineRandom.h>
// 분류 : 
// 용도 : 
// 설명 : 

class GameEngineEffectRenderer;
class GameEngineFBXRenderer;
class RioDSkillArrow : public GameEngineActor
{
public:
	RioDSkillArrow(); // default constructer 디폴트 생성자
	~RioDSkillArrow(); // default destructer 디폴트 소멸자
	RioDSkillArrow(const RioDSkillArrow& _other) = delete; // default Copy constructer 디폴트 복사생성자
	RioDSkillArrow(RioDSkillArrow&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	RioDSkillArrow& operator=(const RioDSkillArrow& _other) = delete; // default Copy operator 디폴트 대입 연산자
	RioDSkillArrow& operator=(const RioDSkillArrow&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake(float _speed, float4 _localPosition);
	void PlaySleep();

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
	GameEngineFBXRenderer* arrowRenderer_;
	GameEngineRandom random_;
	GameEngineEffectRenderer* shotEffect_;

	float4 scaleContainer_;

	float timer_;
	float speed_;
	float fadeTimer_;

};

