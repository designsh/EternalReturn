#include "PreCompile.h"
#include "UI_Equip.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "ItemBase.h"
#include "UI_ItemSettingHelper.h"
#include "EquipmentItem.h"

UI_Equip::UI_Equip()
	: Time(1.0f), UIOn(true) ,
	Slot0_Item(nullptr), Slot1_Item(nullptr), Slot2_Item(nullptr), Slot3_Item(nullptr),
	Slot4_Item(nullptr), Slot5_Item(nullptr)
{
}

UI_Equip::~UI_Equip()
{
	//for (auto& UIRenderer : UIRendererMap)
	//{
	//	if (true)
	//	{
	//		delete UIRenderer.second;
	//		UIRenderer.second = nullptr;
	//	}
	//}
}

void UI_Equip::Start()
{

	BackGroundPos = { -215.0f, -319.0f, 0.0f };
	BasicSlotPos = { -255.0f, -303.0f, -1.0f };
	SlotXPivot = { 40.0f, 0.0f, 0.0f };
	SlotYPivot = { 0.0f, -33.0f, 0.0f };
	SlotSize = { 36.f,23.f };

	{
		Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Renderer->SetImage("Equip_BackGround.png", "PointSmp");
		Renderer->GetTransform()->SetLocalPosition(BackGroundPos);
		Renderer->GetTransform()->SetLocalScaling(Renderer->GetCurrentTexture()->GetTextureSize());
	}
	
	{
		{
			//마우스와의 충돌을 위해서 aabbbox3d로 충돌

			Slot0Collision = CreateTransformComponent<GameEngineCollision>();
			Slot0Collision->GetTransform()->SetLocalPosition(BasicSlotPos);
			Slot0Collision->GetTransform()->SetLocalScaling(SlotSize);
			Slot0Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

			Slot1Collision = CreateTransformComponent<GameEngineCollision>();
			Slot1Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotXPivot);
			Slot1Collision->GetTransform()->SetLocalScaling(SlotSize);
			Slot1Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

			Slot2Collision = CreateTransformComponent<GameEngineCollision>();
			Slot2Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 2.0f));
			Slot2Collision->GetTransform()->SetLocalScaling(SlotSize);
			Slot2Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

			Slot3Collision = CreateTransformComponent<GameEngineCollision>();
			Slot3Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotYPivot);
			Slot3Collision->GetTransform()->SetLocalScaling(SlotSize);
			Slot3Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

			Slot4Collision = CreateTransformComponent<GameEngineCollision>();
			Slot4Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotYPivot + SlotXPivot);
			Slot4Collision->GetTransform()->SetLocalScaling(SlotSize);
			Slot4Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

			Slot5Collision = CreateTransformComponent<GameEngineCollision>();
			Slot5Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotYPivot + (SlotXPivot * 2.0f));
			Slot5Collision->GetTransform()->SetLocalScaling(SlotSize);
			Slot5Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
		}
	}

	{
		Slot0_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot1_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot2_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot3_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot4_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot5_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
	}

	{
		Slot0_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot1_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot2_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot3_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot4_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot5_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
	}


}

void UI_Equip::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	ItemRenderCheck();

	//UI 온오프 체크
	{
		if (false == UIOn)
		{
			Renderer->Off();
		}
		else
		{
			Renderer->On();
			//GetLevel()->PushDebugRenderUI(Slot0Collision->GetTransform(), CollisionType::Rect);
			//GetLevel()->PushDebugRenderUI(Slot1Collision->GetTransform(), CollisionType::Rect);
			//GetLevel()->PushDebugRenderUI(Slot2Collision->GetTransform(), CollisionType::Rect);
			//GetLevel()->PushDebugRenderUI(Slot3Collision->GetTransform(), CollisionType::Rect);
			//GetLevel()->PushDebugRenderUI(Slot4Collision->GetTransform(), CollisionType::Rect);
			//GetLevel()->PushDebugRenderUI(Slot5Collision->GetTransform(), CollisionType::Rect);
		}
	}

	if (true == GameEngineInput::GetInst().Down("Esc"))
	{
		if (UIOn == true)
		{
			UIOn = false;
		}
		else
		{
			UIOn = true;
		}
	}
}


void UI_Equip::EmptySlot()
{

	if (Slot0_Item != nullptr)
	{
		Slot0_Item->Release();
		Slot0_Item = nullptr;
	}

	if (Slot1_Item != nullptr)
	{
		Slot1_Item->Release();
		Slot1_Item = nullptr;
	}

	if (Slot2_Item != nullptr)
	{
		Slot2_Item->Release();
		Slot2_Item = nullptr;
	}

	if (Slot3_Item != nullptr)
	{
		Slot3_Item->Release();
		Slot3_Item = nullptr;
	}

	if (Slot4_Item != nullptr)
	{
		Slot4_Item->Release();
		Slot4_Item = nullptr;
	}

	if (Slot5_Item != nullptr)
	{
		Slot5_Item->Release();
		Slot5_Item = nullptr;
	}
}

void UI_Equip::PushItem(ItemBase* _OriginItemBase, EquipmentType _Type)
{
	switch (_Type)
	{
	case EquipmentType::NONE:
	{
		GameEngineDebug::MsgBoxError("장비가 아닌 아이템을 장비하려고 했습니다");
		return;
	}
		break;
	case EquipmentType::WEAPON:
	{
		Slot0_Item = _OriginItemBase->Copy();
		return;
	}
		break;
	case EquipmentType::HEAD:
	{
		Slot2_Item = _OriginItemBase->Copy();
		return;

	}
		break;
	case EquipmentType::CHEST:
	{
		Slot1_Item = _OriginItemBase->Copy();
		return;
	}
		break;
	case EquipmentType::ARM:
	{
		Slot3_Item = _OriginItemBase->Copy();
		return;
	}
		break;
	case EquipmentType::LEG:
	{
		Slot4_Item = _OriginItemBase->Copy();
		return;
	}
		break;
	case EquipmentType::ACCESSORY:
	{
		Slot5_Item = _OriginItemBase->Copy();
		return;
	}

		break;
	case EquipmentType::MAX:
	{
		GameEngineDebug::MsgBoxError("장비가 아닌 아이템을 장비하려고 했습니다");
		return;
	}
		break;
	default:
	{
		GameEngineDebug::MsgBoxError("장비타입이 제대로 세팅되지 않았습니다");
		return;
	}
		break;
	}
}

void UI_Equip::GetEquipInfo(vector<EquipmentItem*> _ItemVector)
{
	vector<EquipmentItem*>::iterator listiter = _ItemVector.begin();
	
	for (; listiter != _ItemVector.end(); listiter++)
	{
		if (nullptr == (*listiter))
		{
			continue;
		}
		
		PushItem(*listiter, (*listiter)->GetEquipType());
	}
}

void UI_Equip::ItemRenderCheck()
{
	float4 Iconsize = { 0.25f,0.25f, 1.f, 1.f };

	if (Slot0_Item != nullptr)
	{
		Slot0_BGRenderer->GetTransform()->SetLocalPosition(Slot0Collision->GetTransform()->GetLocalPosition());
		Slot0_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot0_BGRenderer, Slot0_Item);

		Slot0_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot0_Item));
		Slot0_IconRenderer->GetTransform()->SetLocalScaling(Slot0_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize);
		Slot0_IconRenderer->GetTransform()->SetLocalPosition((Slot0_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot0_BGRenderer->On();
		Slot0_IconRenderer->On();
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

		Slot1_BGRenderer->On();
		Slot1_IconRenderer->On();
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

		Slot2_BGRenderer->On();
		Slot2_IconRenderer->On();
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

		Slot3_BGRenderer->On();
		Slot3_IconRenderer->On();
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
		Slot4_IconRenderer->GetTransform()->SetLocalScaling(Slot4_IconRenderer->GetCurrentTexture()->GetTextureSize() * Iconsize);
		Slot4_IconRenderer->GetTransform()->SetLocalPosition((Slot4_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot4_BGRenderer->On();
		Slot4_IconRenderer->On();
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

		Slot5_BGRenderer->On();
		Slot5_IconRenderer->On();
	}
	else
	{
		Slot5_BGRenderer->Off();
		Slot5_IconRenderer->Off();
	}
}

int UI_Equip::SlotMouseCollisionCheck()
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

	//충돌중이 아님
	return -1;

}