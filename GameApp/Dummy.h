#pragma once

#include "Character.h"
// 분류 : 
// 용도 : 
// 설명 : 
class Dummy : public Character
{
public:
	Dummy(); // default constructer 디폴트 생성자
	~Dummy(); // default destructer 디폴트 소멸자
	Dummy(const Dummy& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Dummy(Dummy&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	Dummy& operator=(const Dummy& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Dummy& operator=(const Dummy&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
public:
	static void LoadResource();
	static void ReleaseResource();


protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	// Character을(를) 통해 상속됨
	virtual void initRendererAndAnimation() override;
	void initDummyCollision();


	virtual void changeAnimationRun() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationBasicAttack() override;


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

public:
	void InitHyunwooCollision();

private:
	float timer_collision_Q;
	float timer_end_Q;
	bool b_Qhit_;
	GameEngineCollision* collision_Q;

};

