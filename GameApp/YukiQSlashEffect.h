#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineEffectRenderer;
class YukiQSlashEffect : public GameEngineActor
{
public:
	YukiQSlashEffect(); // default constructer 디폴트 생성자
	~YukiQSlashEffect(); // default destructer 디폴트 소멸자
	YukiQSlashEffect(const YukiQSlashEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	YukiQSlashEffect(YukiQSlashEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	YukiQSlashEffect& operator=(const YukiQSlashEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	YukiQSlashEffect& operator=(const YukiQSlashEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayBurst(float4 _pos);
	void Stop();

protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startBurst();
	void updateBurst(float _deltaTime);

private:
	GameEngineFSM renderState_;

	GameEngineEffectRenderer* slashRenderer_;
	GameEngineEffectRenderer* kiBurstRenderer_;
private:
	float timer_;
	float4 targetPos_;
};

