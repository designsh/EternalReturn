#pragma once
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineEffectRenderer.h>
// �з� : 
// �뵵 : 
// ���� : bFocused �� true �� ĳ������ ���� ���θ� üũ�ؼ� ��� ��Ʈ�� �ֵ����� ���� ��, ���â���� ���� ��ư�� ��� �ݴϴ�.
// ��ư�� ������ ������ PM ������ ���� ���� �� Result level �� �ݿ� �� �̵��մϴ�.

class UI_WinLose : public GameEngineActor
{
public:
	UI_WinLose(); // default constructer ����Ʈ ������
	~UI_WinLose(); // default destructer ����Ʈ �Ҹ���
	UI_WinLose(const UI_WinLose& _other) = delete; // default Copy constructer ����Ʈ ���������
	UI_WinLose(UI_WinLose&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	UI_WinLose& operator=(const UI_WinLose& _other) = delete; // default Copy operator ����Ʈ ���� ������
	UI_WinLose& operator=(const UI_WinLose&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetPortrait(JobType _jobtype, bool _isWin);
	void SetText(const std::string& _text);
	void Activate()
	{
		isActivated_ = true;
	}

	void SetWinner();

public:
	bool MouseCollisionCheck();

	void SetImageByIndex(int _index);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void startNoAppear();
	void updateNoAppear(float _DeltaTime);

	void startInterval();
	void updateInterval(float _DeltaTime);

	void startAppear();
	void updateAppear(float _DeltaTime);

	void startStandBy();
	void updateStandBy(float _DeltaTime);


private:
	GameEngineUIRenderer* portraitRenderer_;
	GameEngineUIRenderer* backRenderer_;

	GameEngineUIRenderer* deathCheckRenderer_;

	GameEngineUIRenderer* winLosePanelRenderer_;
	GameEngineUIRenderer* winLoseTextRenderer_;

	GameEngineUIRenderer* rankPanelRenderer_;
	GameEngineUIRenderer* textRenderer_;
	
	GameEngineUIRenderer* bloodStainRenderer_;
	GameEngineUIRenderer* bloodStainRendererUp_;

	GameEngineUIRenderer* exitBtnRenderer_;
	GameEngineCollision* exitBtnCollision_;

	float intervalTimer_;

	bool isActivated_;
	float appearTimer_;
	bool isAllAppeared_;
	float TIME_APPEAR_DEFAULT = 1.5f;
	bool isWinner_;


	GameEngineFSM UIstate_;
};

