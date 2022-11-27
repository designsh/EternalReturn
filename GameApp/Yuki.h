#pragma once
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include "BasicAttackEffect.h"
#include "Character.h"

// 분류 : 
// 용도 : 
// 설명 : 

class YukiESlashEffect;
class YukiQEffect;
class YukiQSlashEffect;
class YukiREffect;
class Yuki : public Character
{	
public:
	Yuki(); // default constructer 디폴트 생성자
	~Yuki(); // default destructer 디폴트 소멸자
	Yuki(const Yuki& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Yuki(Yuki&& _other) noexcept = delete;
	Yuki& operator=(const Yuki& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Yuki& operator=(const Yuki&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자


public:
	static void LoadResource();
	static void ReleaseResource();

public:
	void Show() override;
	void Hide() override;

public:
	virtual JobType GetJobType() { return JobType::YUKI; }

protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	// Character을(를) 통해 상속됨
	virtual void initRendererAndAnimation() override;
	void initYukiCollision();
	void initYukiCustomState();
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
	void startCustomRStandBy();
	void updateCustomRStandBy(float _deltaTime);
	void endCustomRStandBy();

	void startCustomRSlash();
	void updateCustomRSlash(float _deltaTime);
	void endCustomRSlash();

	void startCustomRAfterBurst();
	void updateCustomRAfterBurst(float _deltaTime);
	void endCustomRAfterBurst();

private:
	GameEngineFBXRenderer* swordRenderer_;

	GameEngineEffectRenderer* groundEffectRenderer_;

	GameEngineEffectRenderer* rearEffectRenderer_;

	GameEngineEffectRenderer* dEffectRenderer_;

	BasicAttackEffect* basicAttackEffect_;
	bool atkFlag_;

	YukiQEffect* qEffect_;
	YukiQSlashEffect* qSlashEffect_;
	bool b_isQ_;
	float timer_Q;
	bool b_Qhit_;

	float timer_Dash_E;		// 대쉬 지속시간
	GameEngineCollision* collision_E;
	bool b_Ehit_;
	YukiESlashEffect* eSlashEffect_;

	YukiREffect* rEffect_;
	GameEngineFSM customState_;
	GameEngineCollision* collision_R;
	float timer_R;
	bool b_RHit_;
	bool bisSlashDone_;

	std::vector<IUnit*> rUnitList_;
	float rExplodeTimer_;


	int passiveToken_; // 단추 수
	float passiveDamage_; // 단추로 적용되는 추가 고정 대미지


	float timer_D;
	bool b_DShot_;
	float timer_unstoppable_;

};