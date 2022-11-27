#pragma once

#include "Character.h"
// �з� : 
// �뵵 : 
// ���� : 
class Dummy : public Character
{
public:
	Dummy(); // default constructer ����Ʈ ������
	~Dummy(); // default destructer ����Ʈ �Ҹ���
	Dummy(const Dummy& _other) = delete; // default Copy constructer ����Ʈ ���������
	Dummy(Dummy&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	Dummy& operator=(const Dummy& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Dummy& operator=(const Dummy&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
public:
	static void LoadResource();
	static void ReleaseResource();


protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	// Character��(��) ���� ��ӵ�
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

