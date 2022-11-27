#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngineBase/GameEngineRandom.h>
#include "IUnit.h"

class RioDSkillArrow;
class GameEngineEffectRenderer;
class RioDSkillEffect : public GameEngineActor
{
public:
	RioDSkillEffect(); // default constructer 디폴트 생성자
	~RioDSkillEffect(); // default destructer 디폴트 소멸자
	RioDSkillEffect(const RioDSkillEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	RioDSkillEffect(RioDSkillEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	RioDSkillEffect& operator=(const RioDSkillEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	RioDSkillEffect& operator=(const RioDSkillEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake();
	void PlayShot();

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime);

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

	void startShot();
	void updateShot(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* zoneRenderer_;
	GameEngineEffectRenderer* impactRenderer_;
	GameEngineEffectRenderer* targetRenderer_;
	GameEngineCollision* collision_;
	std::vector<RioDSkillArrow*> arrowList_;
	

private:
	float timer_;
	float rotationContainer_;

	float4 impactScaleContainer_;
	float4 impactOriginalScale_;
	float4 targetScaleContainer_;
	float4 targetOriginalScale_;

	float shotTimer_;
	GameEngineRandom random_;
	bool bHitsound_;
};


