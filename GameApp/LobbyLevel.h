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
	// 스테이트를 나눠 사용하는 게 옳을 것
	// Idle :: 아무 입력이 들어가지 않은 초기 스테이트
	// CharSelect :: 캐릭터 셀렉트와 시작 지점 설정을 위한 스테이트?
	// (플레이어 정보... 캐릭터 선택 시작 위치 등을 담을 수 있는 패킷을 준비해야 한다.)
	// LoginPacket? CharSelectPacket? 이름을 무엇으로?

	
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
	// 기존 플레이어 수
	// 만약 서버 소켓에 새로운 클라이언트가 들어와서 ServerPlayerList 의 size 가 늘어났다면
	// 이 PlayerList 와 크기 차이를 조건문으로 비교해 "서버에 새 클라가 들어왔으니, 그 들어온 클라이언트에게 패킷을 보내라"
	// 라는 코드로 들어오게 한다



	// 0906 박종원
private:
	int myCharacterSelection_;
	int myStartPointSelection_;
	bool myIsReady_;
	//UI_TestMouse* Mouse_;

	//이건호 : 로비내 UI용 멤버변수
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


private: // voice play 용
	float voicePlayTime_;
	bool isVoicePlayed_;
};

