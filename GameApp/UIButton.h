#pragma once
#include <GameEngine/GameEngineActor.h>

#include "Enums.h"

// 분류 : UI
// 용도 : 기본(부모)클래스(추상클래스)
// 설명 : 버튼의 상태별 사운드 및 콜백함수를 등록
//        단, 일반적인 Button일경우 해당 클래스를 상속받으며 특수적인 Button은 해당 클래스를 상속받지않고 만들어낸다.
//        일반적인 버튼 -> 하나의 렌더러와 하나의 충돌체를 가지는 버튼
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

