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
	// 소멸시 입력중인 채팅 삭제
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
	// 키등록
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
	//		// 캐럿 반납
	//		SetCaretBlinkTime(500);
	//		HideCaret(GameEngineWindow::GetInst().GetWindowHWND());
	//		DestroyCaret();


	// 키보드 키입력 체크 및 문자열 편집
	//if (true == ChattingInputOK_)
	{
		while (!KeyboardClass::GetInst().CharBufferIsEmpty())
		{
			std::string SetStr = "";
			unsigned char ReadChar = KeyboardClass::GetInst().ReadChar();
			SetStr += ReadChar;
			AddText(SetStr);
			inputRenderer_->TextSetting("굴림", InputText_, 30, FW1_CENTER);
		}
	}
	//else // 현재 입력상태가 아닌와중에도 키보드 입력이 있다면 charbuffer를 클리어한다.
	//{
	//	KeyboardClass::GetInst().DeleteCharBuffer();
	//}

	// 입력중인 채팅 문자열을 채팅 이력으로 보내기(채팅입력완료)
	//if (/*true == ChattingInputOK_ && */true == GameEngineInput::GetInst().Down("Chatting_Enter"))
	//{
	//	// 문자열이 있을때만 이력에 남길수있다.
	//	if (!InputText_.empty())
	//	{
	//		// 현재 입력된 문자열을 ChattingHistory로 넘겨준다.
	//		ChattingHistory::ChattingHistoryAdd(InputText_);
	//
	//		InputText_.clear();
	//
	//		// 캐럿위치 초기화
	//		SetCaretPos(20, 660);
	//	}
	//}

	// 입력중인 채팅 문자열 마지막부터 삭제
	if (/*true == ChattingInputOK_ && */true == GameEngineInput::GetInst().Down("Chatting_Del"))
	{
		// 채팅 입력창 활성화 시에만 아래 처리가능
		if (!InputText_.empty())
		{
			InputText_.pop_back();
			inputRenderer_->TextSetting("굴림", InputText_, 30, FW1_CENTER);
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
	// 채팅 최대글자수 넘어가면 입력불가
	std::string CurText = InputText_;
	CurText += _Text;
	if (16 <= lstrlen(CurText.c_str()))
	{
		// 입력불가
		return;
	}

	InputText_ += _Text;
}

