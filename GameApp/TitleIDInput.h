#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineImageRenderer;
class GameEngineUIRenderer;
class GameEngineCollision;
class TitleIDInput : public GameEngineActor
{
private:
	static bool caretshow_;

public:
	static void SetCurCaretState(bool _Flag);

private:	// member Var
	bool ChattingInputOK_;
	std::string InputText_;
	int curcaretpos_;
	int prevcaretpos_;

private:
	GameEngineUIRenderer* inputRenderer_;
	GameEngineImageRenderer* inputWindowRenderer_;
	GameEngineCollision* inputCollision_;

public:
	TitleIDInput(); // default constructer 디폴트 생성자
	~TitleIDInput(); // default destructer 디폴트 소멸자
	TitleIDInput(const TitleIDInput& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TitleIDInput(TitleIDInput&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	TitleIDInput& operator=(const TitleIDInput& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TitleIDInput& operator=(const TitleIDInput&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자


protected:
	virtual void Start() override;
	virtual void Update(float _DeltaTime) override;

public:
	void AddText(const std::string& _Text);
	const std::string GetInput() { return InputText_; }

};

