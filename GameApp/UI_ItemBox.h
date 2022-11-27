#pragma once
#include <GameEngine/GameEngineActor.h>


using namespace std;
// 설명 :
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

	//아이템 리스트를 받아서 pushitem을 해주는 함수
	void GetItemBoxInfo(list<ItemBase*> _ItemList);

	void GetItemBoxInfo(ItemBox* _ItemBox);

	void PushItem(ItemBase* _OriginItemBase, int _SlotNumber = 0);

	void EmptySlot();

	int SlotMouseCollisionCheck();

protected:
	GameEngineUIRenderer* ItemBoxBackGround_Renderer;
	
	//박스냐 시체냐 보급이냐에 따라 달라지는 것
	//종류가 몇개 없어서 통 이미지로 대체될 수도 있음
	GameEngineUIRenderer* BoxtypeFont_Renderer;
	
private:
	float Time;
	bool UI_On;

private:
	void Start() override;
	void Update(float _Time) override;

	//자동으로 빈슬롯을 찾아서 ItemBase를 세팅해주는 함수
	void EmptySlotReturn(ItemBase* _TargetSlot);

	//슬롯에 아이템이 들어있을때 이미지 렌더를 위한 함수
	void ItemRenderCheck();

private:

#pragma region 아이템 출력용 렌더러 + 아이템베이스 모음

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

	//너무 양이 많아진다고 느껴지면 자료구조 사용하자
	//list<ItemBase*> SlotList_;

#pragma endregion 

#pragma region 슬롯별 충돌 콜리전 모음

	GameEngineCollision* Slot0Collision;
	GameEngineCollision* Slot1Collision;
	GameEngineCollision* Slot2Collision;
	GameEngineCollision* Slot3Collision;
	GameEngineCollision* Slot4Collision;
	GameEngineCollision* Slot5Collision;
	GameEngineCollision* Slot6Collision;
	GameEngineCollision* Slot7Collision;

#pragma endregion 

#pragma region 슬롯 float4 피봇값

	float4 BackgroundPos;
	float4 BasicSlotPos;
	float4 SlotXPivot;
	float4 SlotYPivot;
	float4 SlotSize;

#pragma endregion

#pragma region 참고사항
	/*

	아이템 박스의 위치는 항상 고정, 같은 종류의 박스면 항상 고정이다


	받아야 하는 것들

	*아이템 박스를 열었냐 아니냐의 여부

	*아이템 박스에 들어 있는 아이템들의 정보
	-> 정보를 토대로 아이템이 무슨 아이템이 들어있는지 체크

	*아이템 박스의 종류->기본상자/시체/보급
	->시체의 경우 장비칸 8칸+인벤토리 8칸해서 16칸짜리 아이템 정보가 담긴 박스가 나옴
	-> 우선순위 낮음, 일단은 8칸짜리 필드에서 루팅하는 8칸 짜리 박스부터 제작

	*UI가 가지고 있어야할 정보의 상한선
	1. 아이템 정보를 받아서 UI에서 출력할걸 찾아서 정하는 경우
	이경우는 SendItemData_Slot1(아이템정보)
	이런식의 함수로 데이터를 보내주면
	UI에서 데이터를 받아서 데이터내 멤버변수들을 이용해서 아이템 박스에 출력
	->현재는 ItemBase 포인터를 받고 있는 중이다

	2. UI가 받는건 단순 enum이나 index, bool
	모든 데이터는 ItemBoxManager가 관리하고,
	UI가 가진건 오직 enum/switch등을 이용한 단순처리과정
	




	들어갈 기능

	* UI_ItemBox의 ItemRemove() 함수
	박스에서 아이템렌더러를 Off한다


	* UI_Inventory의 ItemAttach() 함수
	인벤토리에 아이템 렌더러를 On한다


	* Box_to_Inventory() 함수?
	박스에서 아이템이 이동햇을때
	이건 이제 UI끼리 서로 알게 하느냐 아니면 ItemBoxManager가 관리하느냐의 여부
	
	*마우스와의 충돌여부를 확인할수 있는 충돌체 만들기
	1. 뒷배경과 마우스의 충돌여부
	-> 배경과 마우스가 충돌하지 않은 상태에서 좌측클릭하면 아이템박스가 꺼지게했다

	2. 단독 아이템들의 충돌
	-> 마우스와 아이템이 충돌중일때, 클릭시 인벤토리로 옮겨지게 만들예정
	-> 드래그가능케 하는건 후순위
	

	아이템박스의 아이템을 좌클릭하면 바로 인벤토리에들어옴
	이동에 필요한 우클릭을 누르면 상호작용하지 않음
	아이템박스에서 인벤토리로 드래그는 안된다
	인게임에서는 인벤토리에서 아이템박스로 드래그하면 아이템박스 슬롯에 들어가긴하던데 포폴에서는 굳히 필요할까?

	
	*/
#pragma endregion 

};

