#pragma once
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include "BasicAttackEffect.h"
#include "Character.h"


class HyunwooQEffect;
class HyunwooWEffect;
class HyunwooREffect;
class Hyunwoo : public Character
{
private:
	friend HyunwooREffect;
public:
	Hyunwoo(); 
	~Hyunwoo();

	Hyunwoo(const Hyunwoo& _other) = delete; 
	Hyunwoo(Hyunwoo&& _other) = delete; 

	Hyunwoo& operator=(const Hyunwoo& _other) = delete;
	Hyunwoo& operator=(const Hyunwoo&& _other) = delete;

public:
	static void LoadResource();
	static void ReleaseResource();

public:
	virtual JobType GetJobType() { return JobType::HYUNWOO; }

protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	// Character을(를) 통해 상속됨
	virtual void initRendererAndAnimation() override;
	void initHyunwooCollision();
	void initHyunwooCustomState();
	void initEffectRenderer();


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
	virtual void onUpdateRSkill(float _deltaTime) override;

	virtual void onStartDSkill() override;
	virtual void onUpdateDSkill(float _deltaTime) override;

	virtual void onStartDeath() override;
	virtual void onUpdateDeath(float _deltaTime) override;

	virtual void onUpdateCustomState(float _deltaTime) override;


	virtual void onPlayEffect(const std::string& _effectName, IUnit* _victim = nullptr, float4 _pos = float4::ZERO) override;
	virtual void onEffectTransformCheck(float _deltaTime) override;

	virtual void onLevelUp() override;

	// customstate
	void startCustomRSkill();
	void updateCustomRSkill(float _deltaTime);
	void endCustomRSkill();

private:
	HyunwooQEffect* qEffect_;
	HyunwooWEffect* wEffect_;
	HyunwooREffect* rEffect_;
	
	BasicAttackEffect* basicAttackEffect_;
	GameEngineEffectRenderer* QGroundCrackEffectRenderer_;
	GameEngineEffectRenderer* rearEffectRenderer_;


	bool atkFlag_;
	
	float timer_collision_Q;
	float timer_end_Q;
	bool b_Qhit_;
	GameEngineCollision* collision_Q;

	bool b_isW_;
	float timer_w_;
	float timer_unstoppable_;

	float timer_Dash_E;		// 대쉬 지속시간
	GameEngineCollision* collision_E;
	bool b_Ehit_;

	// 캐릭터의 고유한 상태를 처리하고 싶을 때 사용
	GameEngineFSM customState_;



	bool b_Rhit_;
	float rTimer_;
	bool rShot_;
	GameEngineCollision* collision_R;
	float collisionRRate_;

	bool b_Dhit_;
};
