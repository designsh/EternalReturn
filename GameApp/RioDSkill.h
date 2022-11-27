#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "IUnit.h"

class RioDSkillEffect;
class GameEngineCollision;
class RioDSkill : public GameEngineActor
{
public:
	RioDSkill();
	~RioDSkill();
	RioDSkill(const RioDSkill& _other) = delete;
	RioDSkill(RioDSkill&& _other) = delete;
	RioDSkill& operator=(const RioDSkill& _other) = delete;
	RioDSkill& operator=(const RioDSkill&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime);

public:
	void SetWaitTime(float _waitTime) { waitTime_ = _waitTime; }
	void SetDamage(float _damage) { damage_ = _damage; }
	void SetOwner(IUnit* _owner) { owner_ = _owner; }

private:
	void startWait();
	void updateWait(float _deltaTime);

	void startFalling();
	void updateFalling(float _deltaTime);

	void startFall();
	void updateFall(float _deltaTime);

private:
	GameEngineFSM state_;
	GameEngineCollision* collision_;
	IUnit* owner_;
	float waitTime_;
	float damage_;
	int soundCount_;

	RioDSkillEffect* effect_;

	bool bShotSound_;
};

