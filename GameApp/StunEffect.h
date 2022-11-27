 #pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineEffectRenderer;
class StunEffect : public GameEngineActor
{
public:
	StunEffect(); // default constructer 디폴트 생성자
	~StunEffect(); // default destructer 디폴트 소멸자
	StunEffect(const StunEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StunEffect(StunEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	StunEffect& operator=(const StunEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StunEffect& operator=(const StunEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

