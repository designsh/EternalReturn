#include "PreCompile.h"
#include "UI_ItemBox.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

#include "ItemBox.h"
#include "ItemBase.h"
#include "UI_ItemSettingHelper.h"


UI_ItemBox::UI_ItemBox()
	: Time(1.0f), UI_On(false),
	Slot0_Item(nullptr), Slot1_Item(nullptr), Slot2_Item(nullptr), Slot3_Item(nullptr),
	Slot4_Item(nullptr), Slot5_Item(nullptr), Slot6_Item(nullptr), Slot7_Item(nullptr)
{
}

UI_ItemBox::~UI_ItemBox()
{
	//for (auto& ItemSlot : SlotList_)
	//{
	//	if (true)
	//	{
	//		delete ItemSlot;
	//		ItemSlot = nullptr;
	//	}
	//}
}

void UI_ItemBox::Start()
{
	//리소스들을 SetImage하고 위치를 조정합니다
	//위치를 조정한 리소스들은 UIRenderMap에 String과 같이 insert됩니다.
	//인게임정보(HP나 스태미너 상태, 착용장비여부)등을 받으면, UIRendererMap에서 이미지이름으로 Find해서 리소스를 바꿀 예정업니다.

	BackgroundPos = { 0.0f, -100.0f, 0.0f };
	BasicSlotPos = { -78.0f, -93.0f, 0.0f };
	SlotXPivot = { 52.0f, 0.0f, 0.0f };
	SlotYPivot = { 0.0f, -37.0f, 0.0f };
	SlotSize = { 44.0f,27.0f, 1.f };

		//z값을 이용해 앞에오는 이미지/뒤에오는 이미지 순서를 정하고 있습니다.
		//위치정보가 될 float도 양이 늘어나면 map이나 vector로 관리할 예정입니다.
	{
		ItemBoxBackGround_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		ItemBoxBackGround_Renderer->SetImage("ItemBox_BackGround.png", "PointSmp");
		ItemBoxBackGround_Renderer->GetTransform()->SetLocalPosition(BackgroundPos);
		ItemBoxBackGround_Renderer->GetTransform()->SetLocalScaling(ItemBoxBackGround_Renderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		BoxtypeFont_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
	//	BoxtypeFont_Renderer->TextSetting("굴림", "상자", 30);
	//	BoxtypeFont_Renderer->GetTransform()->SetLocalPosition(BackgroundPos);
	}

	ItemBoxBackGround_Renderer->Off();
	BoxtypeFont_Renderer->Off();

	{
		Slot0Collision = CreateTransformComponent<GameEngineCollision>();
		Slot0Collision->GetTransform()->SetLocalPosition(BasicSlotPos);
		Slot0Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot0Collision->SetCollisionGroup(eCollisionGroup::UI);
		Slot0Collision->SetCollisionType(CollisionType::Rect);

		Slot1Collision = CreateTransformComponent<GameEngineCollision>();
		Slot1Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotXPivot);
		Slot1Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot1Collision->SetCollisionGroup(eCollisionGroup::UI);
		Slot1Collision->SetCollisionType(CollisionType::Rect);

		Slot2Collision = CreateTransformComponent<GameEngineCollision>();
		Slot2Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 2.0f));
		Slot2Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot2Collision->SetCollisionGroup(eCollisionGroup::UI);
		Slot2Collision->SetCollisionType(CollisionType::Rect);

		Slot3Collision = CreateTransformComponent<GameEngineCollision>();
		Slot3Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 3.0f));
		Slot3Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot3Collision->SetCollisionGroup(eCollisionGroup::UI);
		Slot3Collision->SetCollisionType(CollisionType::Rect);

		Slot4Collision = CreateTransformComponent<GameEngineCollision>();
		Slot4Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotYPivot);
		Slot4Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot4Collision->SetCollisionGroup(eCollisionGroup::UI);
		Slot4Collision->SetCollisionType(CollisionType::Rect);

		Slot5Collision = CreateTransformComponent<GameEngineCollision>();
		Slot5Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotXPivot + SlotYPivot);
		Slot5Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot5Collision->SetCollisionGroup(eCollisionGroup::UI);
		Slot5Collision->SetCollisionType(CollisionType::Rect);

		Slot6Collision = CreateTransformComponent<GameEngineCollision>();
		Slot6Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 2.0f) + SlotYPivot);
		Slot6Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot6Collision->SetCollisionGroup(eCollisionGroup::UI);
		Slot6Collision->SetCollisionType(CollisionType::Rect);

		Slot7Collision = CreateTransformComponent<GameEngineCollision>();
		Slot7Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 3.0f) + SlotYPivot);
		Slot7Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot7Collision->SetCollisionGroup(eCollisionGroup::UI);
		Slot7Collision->SetCollisionType(CollisionType::Rect);
	}

	{
		Slot0_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		Slot1_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		Slot2_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		Slot3_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		Slot4_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		Slot5_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		Slot6_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		Slot7_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
	}

	{
		Slot0_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot1_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot2_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot3_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot4_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot5_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot6_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot7_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
	}


	//SlotList_.push_back(Slot0_Item);
	//SlotList_.push_back(Slot1_Item);
	//SlotList_.push_back(Slot2_Item);
	//SlotList_.push_back(Slot3_Item);
	//SlotList_.push_back(Slot4_Item);
	//SlotList_.push_back(Slot5_Item);
	//SlotList_.push_back(Slot6_Item);
	//SlotList_.push_back(Slot7_Item);
	


}

void UI_ItemBox::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}

	ItemRenderCheck();

	//UI 온오프 체크
	{
		if (false == UI_On)
		{
			ItemBoxBackGround_Renderer->Off();
			BoxtypeFont_Renderer->Off();
			//Slot0_BGRenderer ->Off();
			//Slot1_BGRenderer ->Off();
			//Slot2_BGRenderer ->Off();
			//Slot3_BGRenderer ->Off();
			//Slot4_BGRenderer ->Off();
			//Slot5_BGRenderer ->Off();
			//Slot6_BGRenderer ->Off();
			//Slot7_BGRenderer ->Off();
			//Slot0_IconRenderer->Off();
			//Slot1_IconRenderer->Off();
			//Slot2_IconRenderer->Off();
			//Slot3_IconRenderer->Off();
			//Slot4_IconRenderer->Off();
			//Slot5_IconRenderer->Off();
			//Slot6_IconRenderer->Off();
			//Slot7_IconRenderer->Off();
		}
		else
		{
			ItemBoxBackGround_Renderer->On();
			BoxtypeFont_Renderer->On();
			//Slot0_BGRenderer->On();
			//Slot1_BGRenderer->On();
			//Slot2_BGRenderer->On();
			//Slot3_BGRenderer->On();
			//Slot4_BGRenderer->On();
			//Slot5_BGRenderer->On();
			//Slot6_BGRenderer->On();
			//Slot7_BGRenderer->On();
			//Slot0_IconRenderer->On();
			//Slot1_IconRenderer->On();
			//Slot2_IconRenderer->On();
			//Slot3_IconRenderer->On();
			//Slot4_IconRenderer->On();
			//Slot5_IconRenderer->On();
			//Slot6_IconRenderer->On();
			//Slot7_IconRenderer->On();
		}
	}

}

void UI_ItemBox::RenderOff()
{
	UI_On = false;
}

void UI_ItemBox::RenderOn()
{
	UI_On = true;
}

void UI_ItemBox::EmptySlot()
{
	//아이템정보를 비워주는 함수

	if (Slot0_Item != nullptr)
	{
		Slot0_Item = nullptr;
	}

	if (Slot1_Item != nullptr)
	{
		Slot1_Item = nullptr;
	}

	if (Slot2_Item != nullptr)
	{
		Slot2_Item = nullptr;
	}

	if (Slot3_Item != nullptr)
	{
		Slot3_Item = nullptr;
	}

	if (Slot4_Item != nullptr)
	{
		Slot4_Item = nullptr;
	}

	if (Slot5_Item != nullptr)
	{
		Slot5_Item = nullptr;
	}

	if (Slot6_Item != nullptr)
	{
		Slot6_Item = nullptr;
	}

	if (Slot7_Item != nullptr)
	{
		Slot7_Item = nullptr;
	}

}

int UI_ItemBox::SlotMouseCollisionCheck()
{
	
	if (true == Slot0Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 0;
	}

	if (true == Slot1Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 1;
	}

	if (true == Slot2Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 2;
	}

	if (true == Slot3Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 3;
	}

	if (true == Slot4Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 4;
	}

	if (true == Slot5Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 5;
	}

	if (true == Slot6Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 6;
	}

	if (true == Slot7Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 7;
	}

	//충돌중이 아님
	return -1;

}

void UI_ItemBox::GetItemBoxInfo(list<ItemBase*> _ItemList)
{
	list<ItemBase*>::iterator listiter = _ItemList.begin();

	for (listiter = _ItemList.begin(); listiter != _ItemList.end(); listiter++)
	{
		PushItem(*listiter);
	}

	return;
}

void UI_ItemBox::GetItemBoxInfo(ItemBox* _ItemBox)
{
	for (int i = 0; i < 8; i++)
	{
		if (_ItemBox->GetItem(i) != nullptr)
		{
			PushItem(_ItemBox->GetItem(i));
		}
	}
}


void UI_ItemBox::PushItem(ItemBase* _OriginItemBase, int _SlotNumber)
{

	switch (_SlotNumber)
	{
	case 0:
	{
		EmptySlotReturn(_OriginItemBase);
		return;
	}
		break;
	case 1:
	{
		if (Slot1_Item == nullptr)
		{
			Slot1_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBox("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
		break;
	case 2:
	{
		if (Slot2_Item == nullptr)
		{
			Slot2_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBox("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
		break;
	case 3:
	{
		if (Slot3_Item == nullptr)
		{
			Slot3_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBox("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
		break;
	case 4:
	{
		if (Slot4_Item == nullptr)
		{
			Slot4_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBox("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
		break;
	case 5:
	{
		if (Slot5_Item == nullptr)
		{
			Slot5_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBox("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
		break;
	case 6:
	{
		if (Slot6_Item == nullptr)
		{
			Slot6_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBox("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
		break;
	case 7:
	{
		if (Slot7_Item == nullptr)
		{
			Slot7_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBox("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
		break;
	default:
	{
		GameEngineDebug::MsgBoxError("슬롯번호가 제대로 세팅되지 않았습니다");
		return;
	}
		break;
	}
}

void UI_ItemBox::EmptySlotReturn(ItemBase* _TargetSlot)
{

	if (Slot0_Item == nullptr)
	{
		Slot0_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot1_Item == nullptr)
	{
		Slot1_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot2_Item == nullptr)
	{
		Slot2_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot3_Item == nullptr)
	{
		Slot3_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot4_Item == nullptr)
	{
		Slot4_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot5_Item == nullptr)
	{
		Slot5_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot6_Item == nullptr)
	{
		Slot6_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot7_Item == nullptr)
	{
		Slot7_Item = _TargetSlot->Copy();
		return;
	}

	GameEngineDebug::MsgBox("아이템 박스에 빈 슬롯이 없습니다");
	return;

}

void UI_ItemBox::ItemRenderCheck()
{
	float4 Iconsize = {0.33f,0.33f, 1.f, 1.f};

	if (Slot0_Item != nullptr)
	{
		Slot0_BGRenderer->GetTransform()->SetLocalPosition(Slot0Collision->GetTransform()->GetLocalPosition());
		Slot0_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot0_BGRenderer, Slot0_Item);

		Slot0_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot0_Item));
		Slot0_IconRenderer->GetTransform()->SetLocalScaling(Slot0_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize);
		Slot0_IconRenderer->GetTransform()->SetLocalPosition((Slot0_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));
	}
	else
	{
		Slot0_BGRenderer->Off();
		Slot0_IconRenderer->Off();
	}


	if (Slot1_Item != nullptr)
	{
		Slot1_BGRenderer->GetTransform()->SetLocalPosition(Slot1Collision->GetTransform()->GetLocalPosition());
		Slot1_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot1_BGRenderer, Slot1_Item);

		Slot1_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot1_Item));
		Slot1_IconRenderer->GetTransform()->SetLocalScaling(Slot1_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize);
		Slot1_IconRenderer->GetTransform()->SetLocalPosition((Slot1_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));


	}
	else
	{
		Slot1_BGRenderer->Off();
		Slot1_IconRenderer->Off();
	}


	if (Slot2_Item != nullptr)
	{
		Slot2_BGRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
		Slot2_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot2_BGRenderer, Slot2_Item);

		Slot2_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot2_Item));
		Slot2_IconRenderer->GetTransform()->SetLocalScaling(Slot2_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize);
		Slot2_IconRenderer->GetTransform()->SetLocalPosition((Slot2_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

	}
	else
	{
		Slot2_BGRenderer->Off();
		Slot2_IconRenderer->Off();
	}

	if (Slot3_Item != nullptr)
	{		
		Slot3_BGRenderer->GetTransform()->SetLocalPosition(Slot3Collision->GetTransform()->GetLocalPosition());
		Slot3_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot3_BGRenderer, Slot3_Item);

		Slot3_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot3_Item));
		Slot3_IconRenderer->GetTransform()->SetLocalScaling(Slot3_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize);
		Slot3_IconRenderer->GetTransform()->SetLocalPosition((Slot3_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

	}		
	else	
	{		
		Slot3_BGRenderer->Off();
		Slot3_IconRenderer->Off();
	}

	if (Slot4_Item != nullptr)
	{
		Slot4_BGRenderer->GetTransform()->SetLocalPosition(Slot4Collision->GetTransform()->GetLocalPosition());
		Slot4_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot4_BGRenderer, Slot4_Item);

		Slot4_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot4_Item));
		Slot4_IconRenderer->GetTransform()->SetLocalScaling(Slot4_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize );
		Slot4_IconRenderer->GetTransform()->SetLocalPosition((Slot4_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

	}
	else
	{
		Slot4_BGRenderer->Off();
		Slot4_IconRenderer->Off();
	}

	if (Slot5_Item != nullptr)
	{
		Slot5_BGRenderer->GetTransform()->SetLocalPosition(Slot5Collision->GetTransform()->GetLocalPosition());
		Slot5_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot5_BGRenderer, Slot5_Item);

		Slot5_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot5_Item));
		Slot5_IconRenderer->GetTransform()->SetLocalScaling(Slot5_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize);
		Slot5_IconRenderer->GetTransform()->SetLocalPosition((Slot5_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

	}
	else
	{
		Slot5_BGRenderer->Off();
		Slot5_IconRenderer->Off();
	}

	if (Slot6_Item != nullptr)
	{
		Slot6_BGRenderer->GetTransform()->SetLocalPosition(Slot6Collision->GetTransform()->GetLocalPosition());
		Slot6_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot6_BGRenderer, Slot6_Item);

		Slot6_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot6_Item));
		Slot6_IconRenderer->GetTransform()->SetLocalScaling(Slot6_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize);
		Slot6_IconRenderer->GetTransform()->SetLocalPosition((Slot6_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

	}
	else
	{
		Slot6_BGRenderer->Off();
		Slot6_IconRenderer->Off();
	}

	if (Slot7_Item != nullptr)
	{
		Slot7_BGRenderer->GetTransform()->SetLocalPosition(Slot7Collision->GetTransform()->GetLocalPosition());
		Slot7_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot7_BGRenderer, Slot7_Item);

		Slot7_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot7_Item));
		Slot7_IconRenderer->GetTransform()->SetLocalScaling(Slot7_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize);
		Slot7_IconRenderer->GetTransform()->SetLocalPosition((Slot7_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

	}
	else
	{
		Slot7_BGRenderer->Off();
		Slot7_IconRenderer->Off();
	}

}