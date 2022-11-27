#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 용도 : 
// 설명 : 
class GameEngineFBXRenderer;
class AyaESpectrum : public GameEngineActor
{
public:
	AyaESpectrum(); // default constructer 디폴트 생성자
	~AyaESpectrum(); // default destructer 디폴트 소멸자
	AyaESpectrum(const AyaESpectrum& _other) = delete; // default Copy constructer 디폴트 복사생성자
	AyaESpectrum(AyaESpectrum&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	AyaESpectrum& operator=(const AyaESpectrum& _other) = delete; // default Copy operator 디폴트 대입 연산자
	AyaESpectrum& operator=(const AyaESpectrum&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake();
	void Reset();

	int GetCount() { return count_; }


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
	std::vector<GameEngineFBXRenderer*> renderers_;
	float revealTime_;
	int count_;
	bool isStopped_;
};

