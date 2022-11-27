#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : UI
// 용도 : 
// 설명 : 현재 게임에 참여한 플레이어들의 정보 로딩상태 표시
class GameEngineProgressBarRenderer;
class GameEngineUIRenderer;
class UI_SkillGauge : public GameEngineActor
{
public:
	UI_SkillGauge(); // default constructer 디폴트 생성자
	~UI_SkillGauge(); // default destructer 디폴트 소멸자
	UI_SkillGauge(const UI_SkillGauge& _other) = delete; // default Copy constructer 디폴트 복사생성자
	UI_SkillGauge(UI_SkillGauge&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	UI_SkillGauge& operator=(const UI_SkillGauge& _other) = delete; // default Copy operator 디폴트 대입 연산자
	UI_SkillGauge& operator=(const UI_SkillGauge&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake(float _time, const std::string& _text);

	void PlayReverseAwake(float _time, const std::string& _text);

	void PlayFadeForce();

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

	void startReverseAwake();
	void updateReverseAwake(float _deltaTime);

	void startFade();
	void updateFade(float _deltaTime);


private:
	float standardTime_; // 기준이 되는 시간
	float gaugeTimer_;
	float loadingPercent_;
	float reverseGaugeTimer_;
	float fadeTimer_;

	GameEngineFSM gaugeState_;

	GameEngineUIRenderer* gaugeBgRenderer_;
	GameEngineProgressBarRenderer* gaugeRenderer_;
	GameEngineUIRenderer* whiteMaskRenderer_;

	std::string text_;

};

