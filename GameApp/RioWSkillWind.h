#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "IUnit.h"

class GameEngineEffectRenderer;
class GameEngineCollision;
// ���� : 
class RioWSkillWind : public GameEngineActor
{
public:
	RioWSkillWind(); // default constructer ����Ʈ ������
	~RioWSkillWind(); // default destructer ����Ʈ �Ҹ���
	RioWSkillWind(const RioWSkillWind& _other) = delete; // default Copy constructer ����Ʈ ���������
	RioWSkillWind(RioWSkillWind&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	RioWSkillWind& operator=(const RioWSkillWind& _other) = delete; // default Copy operator ����Ʈ ���� ������
	RioWSkillWind& operator=(const RioWSkillWind&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake(float _time, float _slowRatio = 0.0f);
	void SetParentIndex(int _index) { parentIndex_ = _index; }

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime);

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

	void startFade();
	void updateFade(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* zoneRenderer_;
	GameEngineEffectRenderer* impactRenderer_;
	GameEngineCollision* collision_;

private:
	float timer_;
	float fadeTimer_;
	float rotationContainer_;
	
	int parentIndex_;

	float slowRatio_;
};

