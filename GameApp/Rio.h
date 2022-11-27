#pragma once

#include <GameEngine/GameEngineActor.h>
#include "Character.h"
#include <GameEngine/GameEngineFSM.h>

class RioDSkillEffect;
class RioDSkillFirstShot;
class BasicAttackEffect;
class GameEngineFBXRenderer;
class LumiaMap;
class Rio : public Character
{
public:
	Rio();
	~Rio();
	Rio(const Rio& _other) = delete;
	Rio(Rio&& _other) = delete;
	Rio& operator=(const Rio& _other) = delete;
	Rio& operator=(const Rio&& _other) = delete;

public:
	static void LoadResource();
	static void ReleaseResource();

public:
	virtual JobType GetJobType() { return JobType::RIO; }

public:
	bool IsLongBow() { return bLongBow_; }
	void Show() override;
	void Hide() override;

protected:
	void Start() override;
	void Update(float _deltaTime) override;

	// Character을(를) 통해 상속됨
	virtual void initRendererAndAnimation() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationRun() override;
	virtual void changeAnimationBasicAttack() override;
	virtual void changeAnimationCraft() override;
	virtual void changeAnimationCook() override;
	virtual void changeDeathAnimation() override;
	virtual void changeAnimationOperate() override;
	virtual void changeAnimationArrive() override;
	virtual void changeAnimationRestStart() override;
	virtual void changeAnimationRestLoop() override;
	virtual void changeAnimationRestEnd() override;
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
	virtual void onUpdateRSkill(float _deltaTime) override;

	virtual void onStartDSkill() override;
	virtual void onUpdateDSkill(float _deltaTime) override;

	virtual void onStartDeath() override;
	virtual void onUpdateDeath(float _deltaTime) override;

	virtual void onUpdateCustomState(float _deltaTime) override;

	virtual void onPlayEffect(const std::string& _effectName, IUnit* _victim = nullptr, float4 _pos = float4::ZERO) override;
	virtual void onEffectTransformCheck(float _deltaTime) override;

	virtual void onLevelUp() override;
private:
	void startSkillEBegin();
	void updateSkillEBegin(float _deltaTime);

	void startSkillEShot();
	void updateSkillEShot(float _deltaTime);

	void startSkillEEnd();
	void updateSkillEEnd(float _deltaTime);

private:
	GameEngineFBXRenderer* shortBow_;
	GameEngineFBXRenderer* longBow_;
	GameEngineCollision* skillECollision_;
	GameEngineFSM customState_;
	bool bLongBow_;
	BasicAttackEffect* basicHit0Effect_;
	BasicAttackEffect* basicHitEffect_;
	bool bSkillEPassable_;
	float skillETime_;
	float4 landingPosition_;
	float4 startPosition_;

	bool bShortRSkillExtra_;
	float skillRTime_;

	RioDSkillFirstShot* dSkillShotEffect_;
	RioDSkillEffect* dSkillEffect_;
};