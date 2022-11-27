#pragma once

#include <GameEngine/GameEngineActor.h>
#include "Character.h"
#include <GameEngine/GameEngineFSM.h>
#include "BasicAttackEffect.h"
#include "AyaESpectrum.h"
#include "AyaBullet.h"

class AyaREffect;
class GameEngineFBXRenderer;
class LumiaMap;
class Aya : public Character
{
public:
	Aya();
	~Aya();
	Aya(const Aya& _other) = delete;
	Aya(Aya&& _other) = delete;
	Aya& operator=(const Aya& _other) = delete;
	Aya& operator=(const Aya&& _other) = delete;

public:
	static void LoadResource();
	static void ReleaseResource();

public:
	void Show() override;
	void Hide() override;

public:
	void Start() override;
	void Update(float _deltaTime) override;


	// Character을(를) 통해 상속됨
	virtual JobType GetJobType() override;

	virtual void initRendererAndAnimation() override;
	virtual void changeAnimationRun() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationBasicAttack() override;
	virtual void changeDeathAnimation() override;
	virtual void changeAnimationOperate() override;
	virtual void changeAnimationArrive() override;
	virtual void changeAnimationRestStart() override;
	virtual void changeAnimationRestLoop() override;
	virtual void changeAnimationRestEnd() override;
	virtual void changeAnimationCraft() override;
	virtual void changeAnimationGather() override;

	virtual void onStartBasicAttacking(IUnit* _target) override;
	virtual void onUpdateBasicAttacking(IUnit* _target, float _deltaTime) override;
	virtual void onStartQSkill() override;
	virtual void onUpdateQSkill(float _deltaTime) override;
	virtual void onStartWSkill() override;
	virtual void onUpdateWSkill(float _deltaTime) override;
	virtual void onStartESkill() override;
	virtual void onUpdateESkill(float _deltaTime) override;
	virtual void onStartRSkill() override;
	virtual void onUpdateRSkill(float _d1eltaTime) override;
	virtual void onStartDSkill() override;
	virtual void onUpdateDSkill(float _deltaTime) override;
	virtual void onUpdateCustomState(float _deltaTime) override;
	virtual void onStartDeath() override;
	virtual void onUpdateDeath(float _deltaTime) override;

	virtual void onPlayEffect(const std::string& _effectName, IUnit* _victim = nullptr, float4 _pos = float4::ZERO) override;
	virtual void onEffectTransformCheck(float _deltaTime) override;

	virtual void onLevelUp() override;
private:
	float reloadTime_;
	int ammo_;
	int skillWFireCount_;
	float skillWFireDelay_;

	BasicAttackEffect* basicAttackEffect_;
	BasicAttackEffect* basicHitEffect_;
	BasicAttackEffect* basicHit0Effect_;
	GameEngineCollision* skillRCol_;
	GameEngineFBXRenderer* pistolRenderer_;

//	std::vector<bullet*>
	AyaBullet* bullet0_;
	AyaBullet* bullet1_;

	AyaESpectrum* eSpectrum_;
	AyaREffect* rEffect_;
};