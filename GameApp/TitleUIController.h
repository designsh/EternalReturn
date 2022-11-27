#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : 
// ���� : 

class TitleIDInput;
class TitlePrologueBG;
class TitleBG;
class TitleFoxLogo;
class TitleStartButton;
class TitleUIController : public GameEngineActor
{
public:
	TitleUIController(); // default constructer ����Ʈ ������
	~TitleUIController(); // default destructer ����Ʈ �Ҹ���
	TitleUIController(const TitleUIController& _other) = delete; // default Copy constructer ����Ʈ ���������
	TitleUIController(TitleUIController&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	TitleUIController& operator=(const TitleUIController& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TitleUIController& operator=(const TitleUIController&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

