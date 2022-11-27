#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineEffectRenderer;
class DeathEffect : public GameEngineActor
{
public:
	DeathEffect(); // default constructer ����Ʈ ������
	~DeathEffect(); // default destructer ����Ʈ �Ҹ���
	DeathEffect(const DeathEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	DeathEffect(DeathEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	DeathEffect& operator=(const DeathEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	DeathEffect& operator=(const DeathEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake(bool _isProhibitedArea);

protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* barBreakRenderer_;
	GameEngineEffectRenderer* barEffectRenderer_;
	GameEngineEffectRenderer* restrictionExplodeRenderer_;

	float barBreakTimer_;
	float barEffectTimer_;
	float4 scaleContainer_;
	bool isExplosion_;
};

