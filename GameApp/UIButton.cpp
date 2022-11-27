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
	// ���� ��ư���� üũ
	if (ButtonState::Disabled != CurButtonState_ && ButtonState::Max != CurButtonState_)
	{
		if (ButtonState::Click == CurButtonState_)
		{
			// ���� ��ȯ
			CurButtonState_ = ButtonState::Normal;

			// �̹����� ����
			if (nullptr != Renderer_ && true == CheckAnimation(CurButtonState_))
			{
				Renderer_->SetChangeAnimation("Normal");
			}

			// Ŭ�� ���� ���
			if (false == MouseClickSoundName_.empty())
			{
				GameEngineSoundManager::GetInstance()->PlaySoundByName(MouseClickSoundName_);
			}

			// ���콺Ŭ�� CallbackFunction ����
			ButtonClickFunction();
		}
		else if (ButtonState::Normal == CurButtonState_ || ButtonState::MouseOver == CurButtonState_)
		{
			// �����¿��� ���콺�� �浹�� ������ȯ
			if (nullptr != Collider_)
			{
#ifdef _DEBUG
				//GetLevel()->PushDebugRenderUI(Collider_->GetTransform(), Collider_->GetCollisionType());
#endif // _DEBUG

				// ���콺�� �浹üũ
				if (true == Collider_->Collision(static_cast<int>(eCollisionGroup::MousePointer)))
				{
					if (true == GameEngineInput::GetInst().Down("LBUTTON"))
					{
						// ��ư ���� ��ȯ
						CurButtonState_ = ButtonState::Click;

						// �̹����� ����
						if (nullptr != Renderer_ && true == CheckAnimation(CurButtonState_))
						{
							Renderer_->SetChangeAnimation("Click");
						}
					}
					else
					{
						// ��ư ���� ��ȯ
						CurButtonState_ = ButtonState::MouseOver;

						// ���콺���� ���� ���
						if (false == MouseOverSoundName_.empty())
						{
							GameEngineSoundManager::GetInstance()->PlaySoundByName(MouseOverSoundName_);
						}

						// �̹��� ����
						if (nullptr != Renderer_ && true == CheckAnimation(CurButtonState_))
						{
							Renderer_->SetChangeAnimation("MouseOver");
						}

						// ���콺���� CallbackFunction ����
						ButtonOverFunction();
					}
				}
				else
				{
					// ���콺�������¿��� �� �̻� �浹�����ʴ´ٸ� ������
					if (ButtonState::MouseOver == CurButtonState_)
					{
						// ��ư ���� ��ȯ
						CurButtonState_ = ButtonState::Normal;

						// �̹��� ����
						if (nullptr != Renderer_ && true == CheckAnimation(CurButtonState_))
						{
							Renderer_->SetChangeAnimation("Normal");
						}

						// ���콺�������¿��� �浹�� �������Ƿ� �Լ�����
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
