#pragma once
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineEffectRenderer.h>
// 분류 : 
// 용도 : 
// 설명 : bFocused 가 true 인 캐릭터의 죽음 여부를 체크해서 모든 컨트롤 주도권을 제어 후, 결과창으로 가는 버튼을 띄워 줍니다.
// 버튼을 누르면 현재의 PM 값들을 따로 저장 후 Result level 로 반영 및 이동합니다.

class UI_WinLose : public GameEngineActor
{
public:
	UI_WinLose(); // default constructer 디폴트 생성자
	~UI_WinLose(); // default destructer 디폴트 소멸자
	UI_WinLose(const UI_WinLose& _other) = delete; // default Copy constructer 디폴트 복사생성자
	UI_WinLose(UI_WinLose&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	UI_WinLose& operator=(const UI_WinLose& _other) = delete; // default Copy operator 디폴트 대입 연산자
	UI_WinLose& operator=(const UI_WinLose&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

