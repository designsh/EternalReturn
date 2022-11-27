#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineEffectRenderer;
class SlowEffect : public GameEngineActor
{
public:
	SlowEffect(); // default constructer 디폴트 생성자
	~SlowEffect(); // default destructer 디폴트 소멸자
	SlowEffect(const SlowEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	SlowEffect(SlowEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	SlowEffect& operator=(const SlowEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	SlowEffect& operator=(const SlowEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

