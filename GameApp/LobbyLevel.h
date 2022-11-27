#pragma once
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>


class UI_TestMouse;
class TempLobbyRenderer;
class GameEngineImageRenderer;
class Lobby_PortraitBg;
class LobbyUIController;
class Lobby_GameStartButton;
class LobbyLevel : public GameEngineLevel
{
public:
	LobbyLevel(); 
	~LobbyLevel();
	LobbyLevel(const LobbyLevel& _other) = delete;
	LobbyLevel(LobbyLevel&& _other) noexcept = delete;
	LobbyLevel& operator=(const LobbyLevel& _other) = delete;
	LobbyLevel& operator=(const LobbyLevel&& _other) = delete;

public:
	// ������Ʈ�� ���� ����ϴ� �� ���� ��
	// Idle :: �ƹ� �Է��� ���� ���� �ʱ� ������Ʈ
	// CharSelect :: ĳ���� ����Ʈ�� ���� ���� ������ ���� ������Ʈ?
	// (�÷��̾� ����... ĳ���� ���� ���� ��ġ ���� ���� �� �ִ� ��Ŷ�� �غ��ؾ� �Ѵ�.)
	// LoginPacket? CharSelectPacket? �̸��� ��������?

	
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

protected:
	void StartIdle();
	void UpdateIdle(float _DeltaTime);
	void EndIdle();

	void StartSelect();
	void UpdateSelect(float _DeltaTime);
	void EndSelect();

	void StartJoin();
	void UpdateJoin(float _DeltaTime);
	void EndJoin();

	void Check_PortraitCollision();
	void PlaySelectedCharVoice(JobType _type);

private:
	GameEngineFSM state_;

public:
	//std::vector<PlayerInfo> playerList_;
	int playerCount_;
	// ���� �÷��̾� ��
	// ���� ���� ���Ͽ� ���ο� Ŭ���̾�Ʈ�� ���ͼ� ServerPlayerList �� size �� �þ�ٸ�
	// �� PlayerList �� ũ�� ���̸� ���ǹ����� ���� "������ �� Ŭ�� ��������, �� ���� Ŭ���̾�Ʈ���� ��Ŷ�� ������"
	// ��� �ڵ�� ������ �Ѵ�



	// 0906 ������
private:
	int myCharacterSelection_;
	int myStartPointSelection_;
	bool myIsReady_;
	//UI_TestMouse* Mouse_;

	//�̰�ȣ : �κ� UI�� �������
private:
	//class LobbyBackground* BackgroundRenderer;
	//class Lobby_ButtonOne* ButtonLeft;
	//class Lobby_ButtonTwo* ButtonRight;
	//std::vector<Lobby_PortraitBg*> PortraitBg;

	LobbyUIController* UIController_;

private:
	std::vector<TempLobbyRenderer*> tempLobbyRenderers_;

private:
	Lobby_GameStartButton* GameStartButton_;


private: // voice play ��
	float voicePlayTime_;
	bool isVoicePlayed_;
};

