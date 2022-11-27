#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "IUnit.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include "eProjectileType.h"

class BasicAttackEffect;
class GameEngineFBXRenderer;
class GameEngineCollision;
class Character;
class RioArrow : public GameEngineActor
{
public:
	RioArrow(); 
	~RioArrow();
	RioArrow(const RioArrow& _other) = delete; 
	RioArrow(RioArrow&& _other) = delete; 
	RioArrow& operator=(const RioArrow& _other) = delete;
	RioArrow& operator=(const RioArrow&& _other) = delete;

public:
	virtual void Start();
	virtual void Update(float _deltaTime);

public:
	void MakeTargetArrow(IUnit& _owner, float _damage, const float4& _position, float _speed, IUnit& _target);
	void MakeNonTargetArrow(IUnit& _owner, float _damage, const float4& _position, float _rotationY, float _speed);

	void SetWaitTime(float _waitTime) { waitTime_ = _waitTime; }
	void SetLifeTime(float _lifeTime) { lifeTime_ = _lifeTime; }
	void SetScale(const float4& _scale) { scale_ = _scale;  transform_.SetLocalScaling(scale_); }
	void SetKnockback(bool _knockback) { bKnockback_ = _knockback; }
	void SetType(eProjectileType _type) { type_ = _type; }

	GameEngineEffectRenderer* GetEffectRenderer() { return effect_; }

private:
	void startWait();
	void updateWait(float _deltaTime);

	void startChase();
	void updateChase(float _deltaTime);

	void startFly();
	void updateFly(float _deltaTime);
	
	void setRotationTo(const float4& _destination, const float4 _startPosition);

private:
	GameEngineFSM state_;
	//GameEngineFBXRenderer* renderer_;
	GameEngineEffectRenderer* effect_;
	GameEngineCollision* collision_;

	IUnit* owner_;
	IUnit* target_;
	eProjectileType type_;

	float4 scale_;
	float damage_;
	float rotationY_;
	float speed_;

	float waitTime_;
	float lifeTime_;
	bool bKnockback_;
};

