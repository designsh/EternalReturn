#pragma once
#include <GameEngine/GameEngineActor.h>


using namespace std;
// ���� :
class GameEngineUIRenderer;
class ItemBase;
class ItemBox;


class UI_ItemBox : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_ItemBox();
	~UI_ItemBox();

	// delete Function
	UI_ItemBox(const UI_ItemBox& _Other) = delete;
	UI_ItemBox(UI_ItemBox&& _Other) noexcept = delete;
	UI_ItemBox& operator=(const UI_ItemBox& _Other) = delete;
	UI_ItemBox& operator=(UI_ItemBox&& _Other) noexcept = delete;

public:

	void RenderOff();
	
	void RenderOn();

	//������ ����Ʈ�� �޾Ƽ� pushitem�� ���ִ� �Լ�
	void GetItemBoxInfo(list<ItemBase*> _ItemList);

	void GetItemBoxInfo(ItemBox* _ItemBox);

	void PushItem(ItemBase* _OriginItemBase, int _SlotNumber = 0);

	void EmptySlot();

	int SlotMouseCollisionCheck();

protected:
	GameEngineUIRenderer* ItemBoxBackGround_Renderer;
	
	//�ڽ��� ��ü�� �����̳Ŀ� ���� �޶����� ��
	//������ � ��� �� �̹����� ��ü�� ���� ����
	GameEngineUIRenderer* BoxtypeFont_Renderer;
	
private:
	float Time;
	bool UI_On;

private:
	void Start() override;
	void Update(float _Time) override;

	//�ڵ����� �󽽷��� ã�Ƽ� ItemBase�� �������ִ� �Լ�
	void EmptySlotReturn(ItemBase* _TargetSlot);

	//���Կ� �������� ��������� �̹��� ������ ���� �Լ�
	void ItemRenderCheck();

private:

#pragma region ������ ��¿� ������ + �����ۺ��̽� ����

	ItemBase* Slot0_Item;
	ItemBase* Slot1_Item;
	ItemBase* Slot2_Item;
	ItemBase* Slot3_Item;
	ItemBase* Slot4_Item;
	ItemBase* Slot5_Item;
	ItemBase* Slot6_Item;
	ItemBase* Slot7_Item;

	GameEngineUIRenderer* Slot0_BGRenderer;
	GameEngineUIRenderer* Slot1_BGRenderer;
	GameEngineUIRenderer* Slot2_BGRenderer;
	GameEngineUIRenderer* Slot3_BGRenderer;
	GameEngineUIRenderer* Slot4_BGRenderer;
	GameEngineUIRenderer* Slot5_BGRenderer;
	GameEngineUIRenderer* Slot6_BGRenderer;
	GameEngineUIRenderer* Slot7_BGRenderer;

	GameEngineUIRenderer* Slot0_IconRenderer;
	GameEngineUIRenderer* Slot1_IconRenderer;
	GameEngineUIRenderer* Slot2_IconRenderer;
	GameEngineUIRenderer* Slot3_IconRenderer;
	GameEngineUIRenderer* Slot4_IconRenderer;
	GameEngineUIRenderer* Slot5_IconRenderer;
	GameEngineUIRenderer* Slot6_IconRenderer;
	GameEngineUIRenderer* Slot7_IconRenderer;

	//�ʹ� ���� �������ٰ� �������� �ڷᱸ�� �������
	//list<ItemBase*> SlotList_;

#pragma endregion 

#pragma region ���Ժ� �浹 �ݸ��� ����

	GameEngineCollision* Slot0Collision;
	GameEngineCollision* Slot1Collision;
	GameEngineCollision* Slot2Collision;
	GameEngineCollision* Slot3Collision;
	GameEngineCollision* Slot4Collision;
	GameEngineCollision* Slot5Collision;
	GameEngineCollision* Slot6Collision;
	GameEngineCollision* Slot7Collision;

#pragma endregion 

#pragma region ���� float4 �Ǻ���

	float4 BackgroundPos;
	float4 BasicSlotPos;
	float4 SlotXPivot;
	float4 SlotYPivot;
	float4 SlotSize;

#pragma endregion

#pragma region �������
	/*

	������ �ڽ��� ��ġ�� �׻� ����, ���� ������ �ڽ��� �׻� �����̴�


	�޾ƾ� �ϴ� �͵�

	*������ �ڽ��� ������ �ƴϳ��� ����

	*������ �ڽ��� ��� �ִ� �����۵��� ����
	-> ������ ���� �������� ���� �������� ����ִ��� üũ

	*������ �ڽ��� ����->�⺻����/��ü/����
	->��ü�� ��� ���ĭ 8ĭ+�κ��丮 8ĭ�ؼ� 16ĭ¥�� ������ ������ ��� �ڽ��� ����
	-> �켱���� ����, �ϴ��� 8ĭ¥�� �ʵ忡�� �����ϴ� 8ĭ ¥�� �ڽ����� ����

	*UI�� ������ �־���� ������ ���Ѽ�
	1. ������ ������ �޾Ƽ� UI���� ����Ұ� ã�Ƽ� ���ϴ� ���
	�̰��� SendItemData_Slot1(����������)
	�̷����� �Լ��� �����͸� �����ָ�
	UI���� �����͸� �޾Ƽ� �����ͳ� ����������� �̿��ؼ� ������ �ڽ��� ���
	->����� ItemBase �����͸� �ް� �ִ� ���̴�

	2. UI�� �޴°� �ܼ� enum�̳� index, bool
	��� �����ʹ� ItemBoxManager�� �����ϰ�,
	UI�� ������ ���� enum/switch���� �̿��� �ܼ�ó������
	




	�� ���

	* UI_ItemBox�� ItemRemove() �Լ�
	�ڽ����� �����۷������� Off�Ѵ�


	* UI_Inventory�� ItemAttach() �Լ�
	�κ��丮�� ������ �������� On�Ѵ�


	* Box_to_Inventory() �Լ�?
	�ڽ����� �������� �̵�������
	�̰� ���� UI���� ���� �˰� �ϴ��� �ƴϸ� ItemBoxManager�� �����ϴ����� ����
	
	*���콺���� �浹���θ� Ȯ���Ҽ� �ִ� �浹ü �����
	1. �޹��� ���콺�� �浹����
	-> ���� ���콺�� �浹���� ���� ���¿��� ����Ŭ���ϸ� �����۹ڽ��� �������ߴ�

	2. �ܵ� �����۵��� �浹
	-> ���콺�� �������� �浹���϶�, Ŭ���� �κ��丮�� �Ű����� ���鿹��
	-> �巡�װ����� �ϴ°� �ļ���
	

	�����۹ڽ��� �������� ��Ŭ���ϸ� �ٷ� �κ��丮������
	�̵��� �ʿ��� ��Ŭ���� ������ ��ȣ�ۿ����� ����
	�����۹ڽ����� �κ��丮�� �巡�״� �ȵȴ�
	�ΰ��ӿ����� �κ��丮���� �����۹ڽ��� �巡���ϸ� �����۹ڽ� ���Կ� �����ϴ��� ���������� ���� �ʿ��ұ�?

	
	*/
#pragma endregion 

};

