 #pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineEffectRenderer;
class StunEffect : public GameEngineActor
{
public:
	StunEffect(); // default constructer ����Ʈ ������
	~StunEffect(); // default destructer ����Ʈ �Ҹ���
	StunEffect(const StunEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	StunEffect(StunEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	StunEffect& operator=(const StunEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StunEffect& operator=(const StunEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake(float _time);

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwake();
	void updateAwake(float _deltaTime);

	void startSleep();
	void updateSleep(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* stunRingRenderer_;
	GameEngineEffectRenderer* starRenderer0_;
	GameEngineEffectRenderer* starRenderer1_;

private:
	float timer_;
	float rotationDegree_;
};

