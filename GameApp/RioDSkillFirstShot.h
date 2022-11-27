#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// �з� : 
class GameEngineEffectRenderer;
class RioDSkillFirstShot : public GameEngineActor
{
public:
	RioDSkillFirstShot(); // default constructer ����Ʈ ������
	~RioDSkillFirstShot(); // default destructer ����Ʈ �Ҹ���
	RioDSkillFirstShot(const RioDSkillFirstShot& _other) = delete; // default Copy constructer ����Ʈ ���������
	RioDSkillFirstShot(RioDSkillFirstShot&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	RioDSkillFirstShot& operator=(const RioDSkillFirstShot& _other) = delete; // default Copy operator ����Ʈ ���� ������
	RioDSkillFirstShot& operator=(const RioDSkillFirstShot&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime);

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* arrowRenderer_;
	GameEngineEffectRenderer* target0Renderer_;
	GameEngineEffectRenderer* target1Renderer_;

private:
	float timer_;

	float fadeTimer_;
};

