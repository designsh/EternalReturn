#pragma once
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngine/GameEngineFSM.h>
#include "Character.h"
#include "BasicAttackEffect.h"
#include "JackieQEffect.h"
#include "JackieEEffect.h"
// 용도 : 
// 설명 : 
class Jackie : public Character
{
public:
	Jackie(); // default constructer 디폴트 생성자
	~Jackie(); // default destructer 디폴트 소멸자
	Jackie(const Jackie& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Jackie(Jackie&& _other) noexcept = delete; // default RValue Copy constructer 디폴트 RValue 복사생성자
	Jackie& operator=(const Jackie& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Jackie& operator=(const Jackie&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	static void LoadResource();
	static void ReleaseResource();

public:
	virtual JobType GetJobType() { return JobType::JACKIE; }

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	// Character을(를) 통해 상속됨
	virtual void initRendererAndAnimation() override;
	void initJackieCollision();
	void initJackieCustomState();
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
	void startSkillEBegin();
	void updateSkillEBegin(float _deltaTime);

	void startSkillEShot();
	void updateSkillEShot(float _deltaTime);

	void startSkillEEnd();
	void updateSkillEEnd(float _deltaTime);

private:
	GameEngineFSM customState_;

	bool atkFlag_;

	float timer_collision_Q;
	float timer_end_Q;
	int b_Qhit_;
	GameEngineCollision* collision_Q;


	float timer_W;
	bool isW_;

	float timer_collision_E;
	float timer_end_E;
	bool b_Ehit_;
	GameEngineCollision* collision_E;
	float4 eStartPosition_;
	float4 eLandingPosition_;
	bool bSkillEPassable_;
	float skillETime_;

	BasicAttackEffect* basicAttackEffect_;
	GameEngineEffectRenderer* skillQEffectRenderer_;

	JackieQEffect* qEffect_;
	JackieEEffect* eEffect_;

	float timer_R;
	bool isR_;

	GameEngineFBXRenderer* sawRenderer_;
	GameEngineFBXRenderer* axeRenderer_;

public:
	float debugX;
	float debugY;
	float debugZ;

private:
	float savedSpeed_;
};

