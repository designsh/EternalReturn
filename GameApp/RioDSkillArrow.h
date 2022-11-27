#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngineBase/GameEngineRandom.h>
// �з� : 
// �뵵 : 
// ���� : 

class GameEngineEffectRenderer;
class GameEngineFBXRenderer;
class RioDSkillArrow : public GameEngineActor
{
public:
	RioDSkillArrow(); // default constructer ����Ʈ ������
	~RioDSkillArrow(); // default destructer ����Ʈ �Ҹ���
	RioDSkillArrow(const RioDSkillArrow& _other) = delete; // default Copy constructer ����Ʈ ���������
	RioDSkillArrow(RioDSkillArrow&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	RioDSkillArrow& operator=(const RioDSkillArrow& _other) = delete; // default Copy operator ����Ʈ ���� ������
	RioDSkillArrow& operator=(const RioDSkillArrow&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake(float _speed, float4 _localPosition);
	void PlaySleep();

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
	GameEngineFBXRenderer* arrowRenderer_;
	GameEngineRandom random_;
	GameEngineEffectRenderer* shotEffect_;

	float4 scaleContainer_;

	float timer_;
	float speed_;
	float fadeTimer_;

};

