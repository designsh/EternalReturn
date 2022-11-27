#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 
// 설명 : 

class TitleIDInput;
class TitlePrologueBG;
class TitleBG;
class TitleFoxLogo;
class TitleStartButton;
class TitleUIController : public GameEngineActor
{
public:
	TitleUIController(); // default constructer 디폴트 생성자
	~TitleUIController(); // default destructer 디폴트 소멸자
	TitleUIController(const TitleUIController& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TitleUIController(TitleUIController&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	TitleUIController& operator=(const TitleUIController& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TitleUIController& operator=(const TitleUIController&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void InitUI();
	TitlePrologueBG* GetTitlePrologueBG() { return titlePrologueBG_; }
	TitleBG* GetTitleBG() { return titleBG_; }
	TitleStartButton* GetStartButton() { return startButton_; }
	TitleIDInput* GetIDInput() { return idInput_; }

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	TitlePrologueBG* titlePrologueBG_;
	TitleBG* titleBG_;
	TitleIDInput* idInput_;
	TitleStartButton* startButton_;
	TitleFoxLogo* logo_;
};

