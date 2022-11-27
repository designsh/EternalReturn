#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : �κ񷹺��� �� UI���͸� ��Ƶδ� ��
// ���� : 


class CharFullSelectEffect;
class Lobby_ButtonOne;
class Lobby_ButtonTwo;
class Lobby_PortraitBg;
class LobbyBackground;
class Lobby_CharFull;
class Lobby_StartButton;
class Lobby_Map;
class LobbyUIController : public GameEngineActor
{
public:
	LobbyUIController(); // default constructer ����Ʈ ������
	~LobbyUIController(); // default destructer ����Ʈ �Ҹ���
	LobbyUIController(const LobbyUIController& _other) = delete; // default Copy constructer ����Ʈ ���������
	LobbyUIController(LobbyUIController&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	LobbyUIController& operator=(const LobbyUIController& _other) = delete; // default Copy operator ����Ʈ ���� ������
	LobbyUIController& operator=(const LobbyUIController&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void InitUI();
	Lobby_PortraitBg* GetPortraitVector(int _index);
	Lobby_ButtonOne* GetHostButton() { return hostButtonUI_; }
	Lobby_ButtonTwo* GetClientButton() { return clientButtonUI_; }
	Lobby_StartButton* GetStartButton() { return startButtonUI_; }
	Lobby_Map* GetMapUI() { return mapUI_; }
	CharFullSelectEffect* GetCharFullEffect() { return charFullEffect_; }
public:
	//���� �� ĳ���Ͱ� ���� ������ ���� static ����
	static JobType SelectedChar;

	//���� ĳ���͸� ���� ĳ���� ���� ��ư���� ������ Ȯ���޴��� ���ߴ����� ����
	static bool CharSelectOn;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	LobbyBackground* backGroundUI_;
	Lobby_ButtonOne* hostButtonUI_;
	Lobby_ButtonTwo* clientButtonUI_;
	Lobby_CharFull* charFullUI_;
	CharFullSelectEffect* charFullEffect_;
	Lobby_StartButton* startButtonUI_;
	Lobby_Map* mapUI_;
	std::vector<Lobby_PortraitBg*> PortraitBg;


};

