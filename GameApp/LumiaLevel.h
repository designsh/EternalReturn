#pragma once
#include <GameEngine/GameEngineLevel.h>

#include "MousePointer.h"
#include "ItemBoxManager.h"
#include "ItemListWindow.h"

#define SERVER

// 분류 : 레벨(씬)
// 용도 : 
// 설명 : 플레이 레벨
class LightActor;
class LumiaMap;
class NavMesh;
class Character;
class Monsters;
class ProhibitedArea;
class GameEngineLevelControlWindow;
class MonsterDebugWindow;
class LumiaLevel : public GameEngineLevel
{
public: // Inline Get Function
	inline MousePointer* GetMousePointer() const
	{
		return MousePointer::InGameMouse;
	}

	inline LumiaMap* GetMap() const
	{
		return CurMap_;
	}

	inline ItemBoxManager* GetItemBoxManager() const
	{
		return ItemBoxManager_;
	}

	inline std::vector<Character*> GetCharacterActorList()
	{
		return CharacterActorList_;
	}

	inline std::vector<Monsters*> GetMonsterActorList() const
	{
		return MonsterActorList_;
	}

	inline std::vector<ProhibitedArea*> GetProhibitedAreaList() const
	{
		return prohitivedAreaList_;
	}

	inline int GetSurvivorCount() 
	{
		return SurvivorCount_; 
	}

public: // Inline Set Function
	inline void SubtractSurvivorCount()
	{
		SurvivorCount_--; 
	}

public: // Public Get Function
	Character* GetSpecificCharacter(int _Index);							// 특정 캐릭터 Get
	Monsters* GetSpecificMonster(int _Index);								// 특정 몬스터 Get
	std::vector<Monsters*>& GetMonsterTypeList(MonsterType _MonsterType);	// 특정 몬스터타입의 몬스터목록 Get

public: // Public Function
	void HostCreateCommand();												// Host 전용
	void GuestCreateCommand();												// Guest 전용

public: // Public Monster Batch Processing Related Function(몬스터일괄처리관련)
	void HostMonsterFirstAppearBatchProcessing(MonsterType _MonsterType);	// 몬스터타입별 첫등장 일괄처리(Host 전용)
	void GuestMonsterFirstAppearBatchProcessing(MonsterType _MonsterType);	// 몬스터타입별 첫등장 일괄처리(Guest 전용)

public: // Public Monster Specific Processing Related Function(특정몬스터처리관련)


public: // Create MonsterInfo
	void CreateMonsterInfo();												// Host 전용(현재 맵에 배치하려는 몬스터들을 미리 셋팅)
																			// 몬스터가 스폰되는 지역과 위치가 해당 함수에서 모두 결정난다.

private: // Command
	void HostAllCreationCommand();											// Host 전용
	void GuestAllCreationCommand();											// Guest 전용

private: // Command
	void MapCreationCommand();												// Host & Guest 공통
	void MonsterCreationCommand();											// Host & Guest 공통
	void CharacterCreationCommand();										// Host & Guest 공통

private: // Resource Load
	void MapResourceLoad();													// Current Map Resource Load
	void CharacterResourceLoad();											// InGame Play Character Resource Load

private: // Basic Initalize Function
	void CreateBasicActor();												// LumiaLevel Basic Actore Creation Function
	void CreateLevelInput();												// LumiaLevel Input Creation Function
	void AddSocketHandle();													// LumiaLevel Network Handle Add Function
	void InitIMGUIWindow();													// LumiaLevel IMGUI Window Setting Function
	void CameraAdjustment();												// LumiaLevel Main Camera Adjustment Function

private: // GameTime Update Function
	void GameTimeUpdatePacketSend();										// GameTime Update Packet Send Function

private: // State Update Function
	void CharacterStateUpdatePacketSend(float _deltaTime);					// Character State Update Packet Send Function
	void CharactersTransformUpdate();										// Character ManagerList Transform Update Function
	void MonsterStateUpdatePacketSend();									// Monster State Update Packet Send Function
	void MonstersTransformUpdate();											// Monster ManagerList Transform Update Function
	void UpdateCharacterVisibility();

private: // Debug Function
	void DebugWindowUpdate();												// Debug Window Update Function

private: // Check Level Related InputKey
	void CheckLevelRelatedInputKey();										// Check InputKey

private: // 1104 박종원
	void CreateProhibitedSystem();											// Create Prohibited Area Function

#pragma region 테스트용함수(추후삭제예정)
	// LevelControlWindow에의해 강제 레벨이동한경우 사용하는 함수
	// 서버 생성이 없이 해당 레벨로 체인지한경우
private:
	void Test_loadResource();
	void Test_initRenderWindow();
	void Test_createActor();
	void Test_adjustCamera();
	void Test_serverCheck();
	void Test_releaseRenderWindow();
	void Test_releaseResource();

private:
	void Test_GenerateCharactor();
#pragma endregion

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;

public:
	LumiaLevel();
	~LumiaLevel();

protected:
	LumiaLevel(const LumiaLevel& _other) = delete;
	LumiaLevel(LumiaLevel&& _other) = delete;

private:
	LumiaLevel& operator=(const LumiaLevel& _other) = delete;
	LumiaLevel& operator=(const LumiaLevel&& _other) = delete;

private:
	std::vector<Character*> CharacterActorList_;													// 캐릭터 목록
	std::vector<Monsters*> MonsterActorList_;														// 몬스터 목록
	std::vector<ProhibitedArea*> prohitivedAreaList_;												// 금지구역 목록

private:
	std::vector<Monsters*> MonsterActorByTypeList_[static_cast<int>(MonsterType::MAX)];				// 몬스터 목록(같은 타입의 몬스터를 일괄처리할때 편하도록 목록작성)

private:
	LumiaMap* CurMap_;																				// 맵
	ItemBoxManager* ItemBoxManager_;																// 아이템박스 매니저
	LightActor* MainLight_;																			// 메인 광원(빛)

private:
	GameEngineLevelControlWindow* DebugAndControlWindow_;											// 레벨관련 디버그 & 강제레벨체인지 윈도우
	MonsterDebugWindow* MonsterDebugWindow_;														// 몬스터관련 디버그 윈도우
	ItemListWindow* ItemListWindow_;																// 아이템목록 윈도우

private:
	int SurvivorCount_;																				// 플레이어 죽음 카운트 (인원수-1 일 시 최후 생존자 캐릭터 승리 판정)
};
