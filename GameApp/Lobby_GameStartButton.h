#pragma once
#include "UIButton.h"

// �з� : UI
// �뵵 : 
// ���� : ���ӽ��� �������. ��, ȣ��Ʈ������ ����
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

