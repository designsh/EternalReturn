#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class Hyunwoo;
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class HyunwooREffect : public GameEngineActor
{
private:	// member Var
	friend Hyunwoo;

public:
	HyunwooREffect(); // default constructer 디폴트 생성자
	~HyunwooREffect(); // default destructer 디폴트 소멸자
	HyunwooREffect(const HyunwooREffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	HyunwooREffect(HyunwooREffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	HyunwooREffect& operator=(const HyunwooREffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	HyunwooREffect& operator=(const HyunwooREffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetMyCharacter(Hyunwoo* _character);
	
	void PlayAwake();
	void PlayExplode();

	void SetSleepMandatory();

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwaken();
	void updateAwaken(float _deltaTime);

	void startExplode();
	void updateExplode(float _deltaTime);

	void startSleeping();
	void updateSleeping(float _deltaTime);

private:

	GameEngineFSM renderState_;
	GameEngineEffectRenderer* hitBoxRenderer_;
	GameEngineEffectRenderer* impactRenderer_;
	GameEngineEffectRenderer* dustRenderer_;

	Hyunwoo* myCharacter_;

private:
	bool isActivated_;
	bool isTriggered_;
	float timer_;
};

