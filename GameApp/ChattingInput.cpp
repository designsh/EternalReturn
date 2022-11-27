#include "PreCompile.h"
#include "ChattingInput.h"
#include "ChattingHistory.h"
#include <GameEngine/GameEngineUIRenderer.h>
#include "KeyboardClass.h"


bool ChattingInput::caretshow_ = false;

void ChattingInput::SetCurCaretState(bool _Flag)
{
	caretshow_ = _Flag;
}

ChattingInput::ChattingInput() :
	ChattingInputOK_(false),
	curcaretpos_(0),
	prevcaretpos_(0),
	chattingRenderer_(nullptr)
{

}

ChattingInput::~ChattingInput()
{
	// �Ҹ�� �Է����� ä�� ����
	if (!InputText_.empty())
	{
		InputText_.clear();
	}
}

ChattingInput::ChattingInput(ChattingInput&& _other) noexcept :
	ChattingInputOK_(_other.ChattingInputOK_),
	curcaretpos_(_other.curcaretpos_),
	prevcaretpos_(_other.prevcaretpos_),
	chattingRenderer_(_other.chattingRenderer_     )
{

}

void ChattingInput::Start()
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

	chattingRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
}

void ChattingInput::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Down("Chatting_Enter"))
	{
		ChattingInputOK_ = !ChattingInputOK_; // ä�� Ȱ��ȭ / ��Ȱ��ȭ ���
	}

	if (true == ChattingInputOK_)
	{
		// ĳ�� ����
		if (false == caretshow_)
		{
			caretshow_ = true;
			CreateCaret(GameEngineWindow::GetInst().GetWindowHWND(), NULL, 2, 14);
			ShowCaret(GameEngineWindow::GetInst().GetWindowHWND());
			SetCaretBlinkTime(100);
			SetCaretPos(0, 0);
		}
	}
	else if (false == ChattingInputOK_) 
		{
			if (true == caretshow_)
			{
				caretshow_ = false;

				// ĳ�� �ݳ�
				SetCaretBlinkTime(500);
				HideCaret(GameEngineWindow::GetInst().GetWindowHWND());
				DestroyCaret();
			}
		}

	//////

	// Ű���� Ű�Է� üũ �� ���ڿ� ����
	if (true == ChattingInputOK_)
	{
		while (!KeyboardClass::GetInst().CharBufferIsEmpty())
		{
			std::string SetStr = "";
			unsigned char ReadChar = KeyboardClass::GetInst().ReadChar();
			SetStr += ReadChar;

			chattingRenderer_->TextSetting("����", InputText_, 30, FW1_CENTER);
			
			AddText(SetStr);
		}
	}
	else // ���� �Է»��°� �ƴѿ��߿��� Ű���� �Է��� �ִٸ� charbuffer�� Ŭ�����Ѵ�.
	{
		KeyboardClass::GetInst().DeleteCharBuffer();
	}

	// �Է����� ä�� ���ڿ��� ä�� �̷����� ������(ä���Է¿Ϸ�)
	if (true == ChattingInputOK_ && true == GameEngineInput::GetInst().Down("Chatting_Enter"))
	{
		// ���ڿ��� �������� �̷¿� ������ִ�.
		if (!InputText_.empty())
		{
			// ���� �Էµ� ���ڿ��� ChattingHistory�� �Ѱ��ش�.
			ChattingHistory::ChattingHistoryAdd(InputText_);

			InputText_.clear();

			// ĳ����ġ �ʱ�ȭ
			SetCaretPos(20, 660);
		}
	}

	// �Է����� ä�� ���ڿ� ���������� ����
	if (true == ChattingInputOK_ && true == GameEngineInput::GetInst().Down("Chatting_Del"))
	{
		// ä�� �Է�â Ȱ��ȭ �ÿ��� �Ʒ� ó������
		if (!InputText_.empty())
		{
			InputText_.pop_back();

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

void ChattingInput::AddText(const std::string& _Text)
{
	// ä�� �ִ���ڼ� �Ѿ�� �ԷºҰ�
	std::string CurText = InputText_;
	CurText += _Text;
	if (MAX_INPUTCOUNT <= lstrlen(CurText.c_str()))
	{
		// �ԷºҰ�
		return;
	}

	InputText_ += _Text;
}

