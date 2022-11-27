#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// ¼³¸í :
class GameEngineUIRenderer;
class ItemBase;
class UI_ItemRoute : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_ItemRoute();
	~UI_ItemRoute();

	// delete Function
	UI_ItemRoute(const UI_ItemRoute& _Other) = delete;
	UI_ItemRoute(UI_ItemRoute&& _Other) noexcept = delete;
	UI_ItemRoute& operator=(const UI_ItemRoute& _Other) = delete;
	UI_ItemRoute& operator=(UI_ItemRoute&& _Other) noexcept = delete;

	bool UIOn;

public:

	void SlotMouseCollisionCheck();

	void SetJobType(JobType _myjob);

	JobType myjob_;

	int EquipType;

	bool isOn() 
	{
		return UIOn;
	}

protected:
	GameEngineUIRenderer* RouteRenderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

private:

#pragma region ½½·Ô float4 ÇÇº¿°ª

	float4 BackGroundPos;
	float4 BasicSlotPos;
	float4 SlotXPivot;
	float4 SlotSize;

#pragma endregion

#pragma region ½½·Ô °ü·Ã ¸ðÀ½

	GameEngineCollision* Slot0Collision;
	GameEngineCollision* Slot1Collision;
	GameEngineCollision* Slot2Collision;
	GameEngineCollision* Slot3Collision;
	GameEngineCollision* Slot4Collision;
	GameEngineCollision* Slot5Collision;

	GameEngineUIRenderer* Slot0_IconRenderer;
	GameEngineUIRenderer* Slot1_IconRenderer;
	GameEngineUIRenderer* Slot2_IconRenderer;
	GameEngineUIRenderer* Slot3_IconRenderer;
	GameEngineUIRenderer* Slot4_IconRenderer;
	GameEngineUIRenderer* Slot5_IconRenderer;

	GameEngineUIRenderer* Select_IconRenderer;

#pragma endregion
	
};

