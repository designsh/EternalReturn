#pragma once
#include <GameEngine/GameEngineLevel.h>

#include "MousePointer.h"
#include "ItemBoxManager.h"
#include "ItemListWindow.h"

#define SERVER

// �з� : ����(��)
// �뵵 : 
// ���� : �÷��� ����
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
	Character* GetSpecificCharacter(int _Index);							// Ư�� ĳ���� Get
	Monsters* GetSpecificMonster(int _Index);								// Ư�� ���� Get
	std::vector<Monsters*>& GetMonsterTypeList(MonsterType _MonsterType);	// Ư�� ����Ÿ���� ���͸�� Get

public: // Public Function
	void HostCreateCommand();												// Host ����
	void GuestCreateCommand();												// Guest ����

public: // Public Monster Batch Processing Related Function(�����ϰ�ó������)
	void HostMonsterFirstAppearBatchProcessing(MonsterType _MonsterType);	// ����Ÿ�Ժ� ù���� �ϰ�ó��(Host ����)
	void GuestMonsterFirstAppearBatchProcessing(MonsterType _MonsterType);	// ����Ÿ�Ժ� ù���� �ϰ�ó��(Guest ����)

public: // Public Monster Specific Processing Related Function(Ư������ó������)


public: // Create MonsterInfo
	void CreateMonsterInfo();												// Host ����(���� �ʿ� ��ġ�Ϸ��� ���͵��� �̸� ����)
																			// ���Ͱ� �����Ǵ� ������ ��ġ�� �ش� �Լ����� ��� ��������.

private: // Command
	void HostAllCreationCommand();											// Host ����
	void GuestAllCreationCommand();											// Guest ����

private: // Command
	void MapCreationCommand();												// Host & Guest ����
	void MonsterCreationCommand();											// Host & Guest ����
	void CharacterCreationCommand();										// Host & Guest ����

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

private: // 1104 ������
	void CreateProhibitedSystem();											// Create Prohibited Area Function

#pragma region �׽�Ʈ���Լ�(���Ļ�������)
	// LevelControlWindow������ ���� �����̵��Ѱ�� ����ϴ� �Լ�
	// ���� ������ ���� �ش� ������ ü�����Ѱ��
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
	std::vector<Character*> CharacterActorList_;													// ĳ���� ���
	std::vector<Monsters*> MonsterActorList_;														// ���� ���
	std::vector<ProhibitedArea*> prohitivedAreaList_;												// �������� ���

private:
	std::vector<Monsters*> MonsterActorByTypeList_[static_cast<int>(MonsterType::MAX)];				// ���� ���(���� Ÿ���� ���͸� �ϰ�ó���Ҷ� ���ϵ��� ����ۼ�)

private:
	LumiaMap* CurMap_;																				// ��
	ItemBoxManager* ItemBoxManager_;																// �����۹ڽ� �Ŵ���
	LightActor* MainLight_;																			// ���� ����(��)

private:
	GameEngineLevelControlWindow* DebugAndControlWindow_;											// �������� ����� & ��������ü���� ������
	MonsterDebugWindow* MonsterDebugWindow_;														// ���Ͱ��� ����� ������
	ItemListWindow* ItemListWindow_;																// �����۸�� ������

private:
	int SurvivorCount_;																				// �÷��̾� ���� ī��Ʈ (�ο���-1 �� �� ���� ������ ĳ���� �¸� ����)
};
