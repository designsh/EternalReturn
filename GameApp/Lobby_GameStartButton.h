#pragma once
#include "UIButton.h"

// 분류 : UI
// 용도 : 
// 설명 : 게임시작 기능제공. 단, 호스트에서만 생성
class Lobby_GameStartButton : public UIButton
{
public:
	static Lobby_GameStartButton* Button;

public:
protected:
	void ButtonNormalFunction() override;
	void ButtonClickFunction() override;
	void ButtonOverFunction() override;

protected:
	void Start() override;

private:
public:
	Lobby_GameStartButton();
	virtual ~Lobby_GameStartButton();

protected:
	Lobby_GameStartButton(const Lobby_GameStartButton& _other) = delete;
	Lobby_GameStartButton(Lobby_GameStartButton&& _other) noexcept = delete;

private:
	Lobby_GameStartButton& operator=(const Lobby_GameStartButton& _other) = delete;
	Lobby_GameStartButton& operator=(const Lobby_GameStartButton&& _other) = delete;

public:
protected:
private:
};

