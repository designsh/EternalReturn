#include "PreCompile.h"
#include "UIButton.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngineBase/GameEngineSoundPlayer.h>

#include "MousePointer.h"

void UIButton::ButtonDisabled()
{
	Renderer_->SetChangeAnimation("Disabled");
	CurButtonState_ = ButtonState::Disabled;
}

void UIButton::ButtonAbled()
{
	Renderer_->SetChangeAnimation("Normal");
	CurButtonState_ = ButtonState::Normal;
}

void UIButton::SetButtonState(ButtonState _ButtonState, const std::string& _TextureName)
{
	if (nullptr != Renderer_)
	{
		switch (_ButtonState)
		{
			case ButtonState::Normal:
			{
				Renderer_->CreateAnimation(_TextureName, "Normal", 0, 0, 0.1f, false);
				break;
			}
			case ButtonState::MouseOver:
			{
				Renderer_->CreateAnimation(_TextureName, "MouseOver", 0, 0, 0.1f, false);
				break;
			}
			case ButtonState::Click:
			{
				Renderer_->CreateAnimation(_TextureName, "Click", 0, 0, 0.1f, false);
				break;
			}
			case ButtonState::Disabled:
			{
				Renderer_->CreateAnimation(_TextureName, "Disabled", 0, 0, 0.1f, false);
				break;
			}
		}
	}
}

void UIButton::Update(float _DeltaTime)
{
	// 현재 버튼상태 체크
	if (ButtonState::Disabled != CurButtonState_ && ButtonState::Max != CurButtonState_)
	{
		if (ButtonState::Click == CurButtonState_)
		{
			// 상태 전환
			CurButtonState_ = ButtonState::Normal;

			// 이미지로 변경
			if (nullptr != Renderer_ && true == CheckAnimation(CurButtonState_))
			{
				Renderer_->SetChangeAnimation("Normal");
			}

			// 클릭 사운드 재생
			if (false == MouseClickSoundName_.empty())
			{
				GameEngineSoundManager::GetInstance()->PlaySoundByName(MouseClickSoundName_);
			}

			// 마우스클릭 CallbackFunction 실행
			ButtonClickFunction();
		}
		else if (ButtonState::Normal == CurButtonState_ || ButtonState::MouseOver == CurButtonState_)
		{
			// 대기상태에서 마우스와 충돌시 상태전환
			if (nullptr != Collider_)
			{
#ifdef _DEBUG
				//GetLevel()->PushDebugRenderUI(Collider_->GetTransform(), Collider_->GetCollisionType());
#endif // _DEBUG

				// 마우스와 충돌체크
				if (true == Collider_->Collision(static_cast<int>(eCollisionGroup::MousePointer)))
				{
					if (true == GameEngineInput::GetInst().Down("LBUTTON"))
					{
						// 버튼 상태 전환
						CurButtonState_ = ButtonState::Click;

						// 이미지로 변경
						if (nullptr != Renderer_ && true == CheckAnimation(CurButtonState_))
						{
							Renderer_->SetChangeAnimation("Click");
						}
					}
					else
					{
						// 버튼 상태 전환
						CurButtonState_ = ButtonState::MouseOver;

						// 마우스오버 사운드 재생
						if (false == MouseOverSoundName_.empty())
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName(MouseOverSoundName_);
						}

						// 이미지 변경
						if (nullptr != Renderer_ && true == CheckAnimation(CurButtonState_))
						{
							Renderer_->SetChangeAnimation("MouseOver");
						}

						// 마우스오버 CallbackFunction 실행
						ButtonOverFunction();
					}
				}
				else
				{
					// 마우스오버상태에서 더 이상 충돌하지않는다면 대기상태
					if (ButtonState::MouseOver == CurButtonState_)
					{
						// 버튼 상태 전환
						CurButtonState_ = ButtonState::Normal;

						// 이미지 변경
						if (nullptr != Renderer_ && true == CheckAnimation(CurButtonState_))
						{
							Renderer_->SetChangeAnimation("Normal");
						}

						// 마우스오버상태에서 충돌이 끝났으므로 함수실행
						ButtonNormalFunction();
					}
				}
			}
		}
	}
}

bool UIButton::CheckAnimation(ButtonState _CheckButtonState)
{
	if (nullptr != Renderer_)
	{
		switch (_CheckButtonState)
		{
			case ButtonState::Normal:
			{
				if (false == Renderer_->CheckAnimation("Normal"))
				{
					return false;
				}
				break;
			}
			case ButtonState::MouseOver:
			{
				if (false == Renderer_->CheckAnimation("MouseOver"))
				{
					return false;
				}
				break;
			}
			case ButtonState::Click:
			{
				if (false == Renderer_->CheckAnimation("Click"))
				{
					return false;
				}
				break;
			}
			case ButtonState::Disabled:
			{
				if (false == Renderer_->CheckAnimation("Disabled"))
				{
					return false;
				}
				break;
			}
		}
	}

	return true;
}

UIButton::UIButton()
	: Renderer_(nullptr)
	, Collider_(nullptr)
	, CurButtonState_(ButtonState::Max)
	, MouseOverSoundName_{}
	, MouseClickSoundName_{}
{
}

UIButton::~UIButton()
{
}
