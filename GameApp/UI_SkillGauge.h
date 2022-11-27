#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : UI
// �뵵 : 
// ���� : ���� ���ӿ� ������ �÷��̾���� ���� �ε����� ǥ��
class GameEngineProgressBarRenderer;
class GameEngineUIRenderer;
class UI_SkillGauge : public GameEngineActor
{
public:
	UI_SkillGauge(); // default constructer ����Ʈ ������
	~UI_SkillGauge(); // default destructer ����Ʈ �Ҹ���
	UI_SkillGauge(const UI_SkillGauge& _other) = delete; // default Copy constructer ����Ʈ ���������
	UI_SkillGauge(UI_SkillGauge&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	UI_SkillGauge& operator=(const UI_SkillGauge& _other) = delete; // default Copy operator ����Ʈ ���� ������
	UI_SkillGauge& operator=(const UI_SkillGauge&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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
	float standardTime_; // ������ �Ǵ� �ð�
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

