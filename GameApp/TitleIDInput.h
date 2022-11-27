#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : 
// �뵵 : 
// ���� : 
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
	TitleIDInput(); // default constructer ����Ʈ ������
	~TitleIDInput(); // default destructer ����Ʈ �Ҹ���
	TitleIDInput(const TitleIDInput& _other) = delete; // default Copy constructer ����Ʈ ���������
	TitleIDInput(TitleIDInput&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	TitleIDInput& operator=(const TitleIDInput& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TitleIDInput& operator=(const TitleIDInput&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����


protected:
	virtual void Start() override;
	virtual void Update(float _DeltaTime) override;

public:
	void AddText(const std::string& _Text);
	const std::string GetInput() { return InputText_; }

};

