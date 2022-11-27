#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// ���� :
class GameEngineUIRenderer;
class ItemBase;
class UI_Inventory : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Inventory();
	~UI_Inventory();

	// delete Function
	UI_Inventory(const UI_Inventory& _Other) = delete;
	UI_Inventory(UI_Inventory&& _Other) noexcept = delete;
	UI_Inventory& operator=(const UI_Inventory& _Other) = delete;
	UI_Inventory& operator=(UI_Inventory&& _Other) noexcept = delete;

	bool UIOn;

public:
	void EmptySlot();
	void PushItem(ItemBase* _OriginItemBase, int _SlotNumber = 0);
	void GetInventoryInfo(vector<ItemBase*> _ItemVector);

	//���Կ� �������� ��������� �̹��� ������ ���� �Լ�
	void ItemRenderCheck();

	int SlotMouseCollisionCheck();
	
	bool CraftCollisionMouseCheck();

	vector<GameEngineUIRenderer*> GetBGRenderers()
	{
		return BGRenderers;
	}
	vector<GameEngineUIRenderer*> GetIconRenderers()
	{
		return IconRenderers;
	}
protected:
	GameEngineUIRenderer* Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//�ڵ����� �󽽷��� ã�Ƽ� ItemBase�� �������ִ� �Լ�
	void EmptySlotReturn(ItemBase* _TargetSlot);

private:

#pragma region ���� float4 �Ǻ���

	float4 BackGroundPos;
	float4 BasicSlotPos;
	float4 SlotXPivot;
	float4 SlotYPivot;
	float4 SlotSize;

#pragma endregion

#pragma region ���� ���� ����

	ItemBase* Slot0_Item;
	ItemBase* Slot1_Item;
	ItemBase* Slot2_Item;
	ItemBase* Slot3_Item;
	ItemBase* Slot4_Item;
	ItemBase* Slot5_Item;
	ItemBase* Slot6_Item;
	ItemBase* Slot7_Item;
	ItemBase* Slot8_Item;
	ItemBase* Slot9_Item;

	vector<GameEngineCollision*> SlotCollisions;
	GameEngineCollision* Slot0Collision;
	GameEngineCollision* Slot1Collision;
	GameEngineCollision* Slot2Collision;
	GameEngineCollision* Slot3Collision;
	GameEngineCollision* Slot4Collision;
	GameEngineCollision* Slot5Collision;
	GameEngineCollision* Slot6Collision;
	GameEngineCollision* Slot7Collision;
	GameEngineCollision* Slot8Collision;
	GameEngineCollision* Slot9Collision;


	GameEngineCollision* CraftCollision;


	vector<GameEngineUIRenderer*> BGRenderers;
	GameEngineUIRenderer* Slot0_BGRenderer;
	GameEngineUIRenderer* Slot1_BGRenderer;
	GameEngineUIRenderer* Slot2_BGRenderer;
	GameEngineUIRenderer* Slot3_BGRenderer;
	GameEngineUIRenderer* Slot4_BGRenderer;
	GameEngineUIRenderer* Slot5_BGRenderer;
	GameEngineUIRenderer* Slot6_BGRenderer;
	GameEngineUIRenderer* Slot7_BGRenderer;
	GameEngineUIRenderer* Slot8_BGRenderer;
	GameEngineUIRenderer* Slot9_BGRenderer;

	vector<GameEngineUIRenderer*> IconRenderers;
	GameEngineUIRenderer* Slot0_IconRenderer;
	GameEngineUIRenderer* Slot1_IconRenderer;
	GameEngineUIRenderer* Slot2_IconRenderer;
	GameEngineUIRenderer* Slot3_IconRenderer;
	GameEngineUIRenderer* Slot4_IconRenderer;
	GameEngineUIRenderer* Slot5_IconRenderer;
	GameEngineUIRenderer* Slot6_IconRenderer;
	GameEngineUIRenderer* Slot7_IconRenderer;
	GameEngineUIRenderer* Slot8_IconRenderer;
	GameEngineUIRenderer* Slot9_IconRenderer;

#pragma endregion
	
};

