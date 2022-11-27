#pragma once
#include <GameEngine/GameEngineActor.h>

#include "Enums.h"

// �з� : UI
// �뵵 : �⺻(�θ�)Ŭ����(�߻�Ŭ����)
// ���� : ��ư�� ���º� ���� �� �ݹ��Լ��� ���
//        ��, �Ϲ����� Button�ϰ�� �ش� Ŭ������ ��ӹ����� Ư������ Button�� �ش� Ŭ������ ��ӹ����ʰ� ������.
//        �Ϲ����� ��ư -> �ϳ��� �������� �ϳ��� �浹ü�� ������ ��ư
class GameEngineUIRenderer;
class GameEngineCollision;
class GameEngineSoundPlayer;
class UIButton : public GameEngineActor
{
public:
	void ButtonDisabled();
	void ButtonAbled();

protected:
	void SetButtonState(ButtonState _ButtonState, const std::string& _TextureName);

protected:
	virtual void ButtonNormalFunction() = 0;
	virtual void ButtonClickFunction() = 0;
	virtual void ButtonOverFunction() = 0;

protected:
	void Update(float _DeltaTime) override;
	
private:
	bool CheckAnimation(ButtonState _CheckButtonState);

public:
	UIButton();
	virtual ~UIButton() = 0;

protected:
	UIButton(const UIButton& _other) = delete;
	UIButton(UIButton&& _other) noexcept = delete;

private:
	UIButton& operator=(const UIButton& _other) = delete;
	UIButton& operator=(const UIButton&& _other) = delete;

public:

protected:
	GameEngineUIRenderer* Renderer_;
	GameEngineCollision* Collider_;
	ButtonState CurButtonState_;

protected:
	std::string MouseOverSoundName_;
	std::string MouseClickSoundName_;
	//GameEngineSoundPlayer ButtonSound_;

private:
};

