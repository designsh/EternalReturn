#include "PreCompile.h"
#include "TitleIDInput.h"
#include "ChattingHistory.h"
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include "KeyboardClass.h"
#include <GameEngine/GameEngineCollision.h>

bool TitleIDInput::caretshow_ = false;

void TitleIDInput::SetCurCaretState(bool _Flag)
{
	caretshow_ = _Flag;
}

TitleIDInput::TitleIDInput() :
	ChattingInputOK_(false),
	curcaretpos_(0),
	prevcaretpos_(0),
	inputRenderer_(nullptr)
{

}

TitleIDInput::~TitleIDInput()
{
	// �Ҹ�� �Է����� ä�� ����
	if (!InputText_.empty())
	{
		InputText_.clear();
	}
}

TitleIDInput::TitleIDInput(TitleIDInput&& _other) noexcept :
	ChattingInputOK_(_other.ChattingInputOK_),
	curcaretpos_(_other.curcaretpos_),
	prevcaretpos_(_other.prevcaretpos_),
	inputRenderer_(_other.inputRenderer_)
{

}

void TitleIDInput::Start()
{
	float4 ImageHarfSize = float4::ZERO;
	// Ű���
	if (false == GameEngineInput::GetInst().IsKey("Chatting_Enter"))
	{
		GameEngineInput::GetInst().CreateKey("Chatting_Enter", VK_RETURN);
	}

	if (false == GameEngineInput::GetInst().IsKey("Chatting_Del"))
	{
		GameEngineInput::GetInst().CreateKey("Chatting_Del", VK_BACK);
	}

	inputRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	inputRenderer_->GetTransform()->SetLocalPosition({ 0.0f, -30.0f, 0.0f });
	inputWindowRenderer_ = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	inputWindowRenderer_->SetImage("NicknameSlot.png", "PointSmp");
	inputWindowRenderer_->GetTransform()->SetLocalPosition({ 0.0f, -40.0f, -10.0f });
	inputWindowRenderer_->GetTransform()->SetLocalScaling(inputWindowRenderer_->GetCurrentTexture()->GetTextureSize());

	inputCollision_ = CreateTransformComponent<GameEngineCollision>();
	inputCollision_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, -102.0f });
	inputCollision_->GetTransform()->SetLocalScaling({150.0f, 50.0f, 0.0f});
	inputCollision_->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
}

void TitleIDInput::Update(float _DeltaTime)
{
	if (false == caretshow_)
	{
		caretshow_ = true;
		CreateCaret(GameEngineWindow::GetInst().GetWindowHWND(), NULL, 2, 14);
		ShowCaret(GameEngineWindow::GetInst().GetWindowHWND());
		SetCaretBlinkTime(10);
		SetCaretPos(0, 0);
	}
	
	//else if (false == ChattingInputOK_)
	//{
	//	if (true == caretshow_)
	//	{
	//		caretshow_ = false;
	//
	//		// ĳ�� �ݳ�
	//		SetCaretBlinkTime(500);
	//		HideCaret(GameEngineWindow::GetInst().GetWindowHWND());
	//		DestroyCaret();


	// Ű���� Ű�Է� üũ �� ���ڿ� ����
	//if (true == ChattingInputOK_)
	{
		while (!KeyboardClass::GetInst().CharBufferIsEmpty())
		{
			std::string SetStr = "";
			unsigned char ReadChar = KeyboardClass::GetInst().ReadChar();
			SetStr += ReadChar;
			AddText(SetStr);
			inputRenderer_->TextSetting("����", InputText_, 30, FW1_CENTER);
		}
	}
	//else // ���� �Է»��°� �ƴѿ��߿��� Ű���� �Է��� �ִٸ� charbuffer�� Ŭ�����Ѵ�.
	//{
	//	KeyboardClass::GetInst().DeleteCharBuffer();
	//}

	// �Է����� ä�� ���ڿ��� ä�� �̷����� ������(ä���Է¿Ϸ�)
	//if (/*true == ChattingInputOK_ && */true == GameEngineInput::GetInst().Down("Chatting_Enter"))
	//{
	//	// ���ڿ��� �������� �̷¿� ������ִ�.
	//	if (!InputText_.empty())
	//	{
	//		// ���� �Էµ� ���ڿ��� ChattingHistory�� �Ѱ��ش�.
	//		ChattingHistory::ChattingHistoryAdd(InputText_);
	//
	//		InputText_.clear();
	//
	//		// ĳ����ġ �ʱ�ȭ
	//		SetCaretPos(20, 660);
	//	}
	//}

	// �Է����� ä�� ���ڿ� ���������� ����
	if (/*true == ChattingInputOK_ && */true == GameEngineInput::GetInst().Down("Chatting_Del"))
	{
		// ä�� �Է�â Ȱ��ȭ �ÿ��� �Ʒ� ó������
		if (!InputText_.empty())
		{
			InputText_.pop_back();
			inputRenderer_->TextSetting("����", InputText_, 30, FW1_CENTER);
			curcaretpos_ = static_cast<int>(lstrlen(InputText_.c_str()));
			SIZE CurTextSize;
			GetTextExtentPoint(GameEngineWindow::GetInst().GetWindowDC(), InputText_.c_str(), lstrlen(InputText_.c_str()), &CurTextSize);
			SetCaretPos(20 + CurTextSize.cx, 660);
		}
	}

	// Chatting
	if (!InputText_.empty())
	{
		TextOut(GameEngineWindow::GetInst().GetWindowDC(), 0, 0, InputText_.c_str(), lstrlen(InputText_.c_str()));


		if (true == caretshow_)
		{
			curcaretpos_ = static_cast<int>(lstrlen(InputText_.c_str()));
			SIZE CurTextSize;
			GetTextExtentPoint(GameEngineWindow::GetInst().GetWindowDC(), InputText_.c_str(), lstrlen(InputText_.c_str()), &CurTextSize);
			SetCaretPos(20 + CurTextSize.cx, 660);
		}
	}
}

void TitleIDInput::AddText(const std::string& _Text)
{
	// ä�� �ִ���ڼ� �Ѿ�� �ԷºҰ�
	std::string CurText = InputText_;
	CurText += _Text;
	if (16 <= lstrlen(CurText.c_str()))
	{
		// �ԷºҰ�
		return;
	}

	InputText_ += _Text;
}

