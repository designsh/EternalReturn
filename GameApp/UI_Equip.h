#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// ���� :
class GameEngineUIRenderer;
class ItemBase;
class EquipmentItem;
class ItemBox;

class UI_Equip : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Equip();
	~UI_Equip();

	// delete Function
	UI_Equip(const UI_Equip& _Other) = delete;
	UI_Equip(UI_Equip&& _Other) noexcept = delete;
	UI_Equip& operator=(const UI_Equip& _Other) = delete;
	UI_Equip& operator=(UI_Equip&& _Other) noexcept = delete;

	bool UIOn;

public:

	void EmptySlot();
	void PushItem(ItemBase* _OriginItemBase, EquipmentType _Type);
	void GetEquipInfo(vector<EquipmentItem*> _ItemVector);

	//���Կ� �������� ��������� �̹��� ������ ���� �Լ�
	void ItemRenderCheck();

	int SlotMouseCollisionCheck();

protected:
	GameEngineUIRenderer* Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

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

	GameEngineCollision* Slot0Collision;
	GameEngineCollision* Slot1Collision;
	GameEngineCollision* Slot2Collision;
	GameEngineCollision* Slot3Collision;
	GameEngineCollision* Slot4Collision;
	GameEngineCollision* Slot5Collision;

	GameEngineUIRenderer* Slot0_BGRenderer;
	GameEngineUIRenderer* Slot1_BGRenderer;
	GameEngineUIRenderer* Slot2_BGRenderer;
	GameEngineUIRenderer* Slot3_BGRenderer;
	GameEngineUIRenderer* Slot4_BGRenderer;
	GameEngineUIRenderer* Slot5_BGRenderer;

	GameEngineUIRenderer* Slot0_IconRenderer;
	GameEngineUIRenderer* Slot1_IconRenderer;
	GameEngineUIRenderer* Slot2_IconRenderer;
	GameEngineUIRenderer* Slot3_IconRenderer;
	GameEngineUIRenderer* Slot4_IconRenderer;
	GameEngineUIRenderer* Slot5_IconRenderer;

#pragma endregion
	
};

