#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// 분류 : 
class GameEngineEffectRenderer;
class RioDSkillFirstShot : public GameEngineActor
{
public:
	RioDSkillFirstShot(); // default constructer 디폴트 생성자
	~RioDSkillFirstShot(); // default destructer 디폴트 소멸자
	RioDSkillFirstShot(const RioDSkillFirstShot& _other) = delete; // default Copy constructer 디폴트 복사생성자
	RioDSkillFirstShot(RioDSkillFirstShot&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	RioDSkillFirstShot& operator=(const RioDSkillFirstShot& _other) = delete; // default Copy operator 디폴트 대입 연산자
	RioDSkillFirstShot& operator=(const RioDSkillFirstShot&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake();

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime);

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* arrowRenderer_;
	GameEngineEffectRenderer* target0Renderer_;
	GameEngineEffectRenderer* target1Renderer_;

private:
	float timer_;

	float fadeTimer_;
};

