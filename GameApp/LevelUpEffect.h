#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineUIRenderer;
class GameEngineEffectRenderer;
class LevelUpEffect : public GameEngineActor
{
public:
	LevelUpEffect(); // default constructer ����Ʈ ������
	~LevelUpEffect(); // default destructer ����Ʈ �Ҹ���
	LevelUpEffect(const LevelUpEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	LevelUpEffect(LevelUpEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	LevelUpEffect& operator=(const LevelUpEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	LevelUpEffect& operator=(const LevelUpEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();

	void Hide(bool _tf);

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwake();
	void updateAwake(float _deltaTime);

	void startSleep();
	void updateSleep(float _deltaTime);

	void startFade();
	void updateFade(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* levelUpRenderer_;
	GameEngineEffectRenderer* glowLineRenderer_;
	GameEngineEffectRenderer* ringRenderer_;

private:
	float timer_;
	float glowTimer_;
	float rotationDegree_;
	float4 scaleContainer_;
};

