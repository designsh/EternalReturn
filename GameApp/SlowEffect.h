#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineEffectRenderer;
class SlowEffect : public GameEngineActor
{
public:
	SlowEffect(); // default constructer ����Ʈ ������
	~SlowEffect(); // default destructer ����Ʈ �Ҹ���
	SlowEffect(const SlowEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	SlowEffect(SlowEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	SlowEffect& operator=(const SlowEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	SlowEffect& operator=(const SlowEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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
	GameEngineEffectRenderer* zoneRenderer0_;
	GameEngineEffectRenderer* zoneRenderer1_;
	GameEngineEffectRenderer* zoneRenderer2_;
private:
	float timer_;
	float rotationDegree_;
};

