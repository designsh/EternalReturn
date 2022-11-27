#include "PreCompile.h"
#include "UI_ItemRoute.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "ItemBase.h"
#include "UI_ItemSettingHelper.h"

UI_ItemRoute::UI_ItemRoute()
	: Time(1.0f), UIOn(false), EquipType(-1)
{
}

UI_ItemRoute::~UI_ItemRoute()
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

void UI_ItemRoute::Start()
{
	BackGroundPos = { -350.0f,0.0f, 0.0f };
	BasicSlotPos = BackGroundPos + float4{ -175.0f, 130.0f, -1.0f };
	SlotXPivot = { 70.0f, 0.0f, 0.0f };
	SlotSize = { 44.0f,27.0f };
	
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
		Slot3Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 3.0f));
		Slot3Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot3Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot4Collision = CreateTransformComponent<GameEngineCollision>();
		Slot4Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 4.0f));
		Slot4Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot4Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot5Collision = CreateTransformComponent<GameEngineCollision>();
		Slot5Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 5.0f));
		Slot5Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot5Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}

	{
		RouteRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		RouteRenderer->SetImage("IconBG_UI.png", "PointSmp");
		RouteRenderer->GetTransform()->SetLocalPosition(BackGroundPos);
		RouteRenderer->GetTransform()->SetLocalScaling(RouteRenderer->GetCurrentTexture()->GetTextureSize());
	}


	{
		Slot0_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot0_IconRenderer->SetImage("rio_acc_Ico.png", "PointSmp");
		Slot0_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos);
		Slot0_IconRenderer->GetTransform()->SetLocalScaling(Slot0_IconRenderer->GetCurrentTexture()->GetTextureSize());

		Slot1_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot1_IconRenderer->SetImage("rio_acc_Ico.png", "PointSmp");
		Slot1_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + SlotXPivot);
		Slot1_IconRenderer->GetTransform()->SetLocalScaling(Slot0_IconRenderer->GetCurrentTexture()->GetTextureSize());

		Slot2_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot2_IconRenderer->SetImage("rio_acc_Ico.png", "PointSmp");
		Slot2_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 2.0f));
		Slot2_IconRenderer->GetTransform()->SetLocalScaling(Slot0_IconRenderer->GetCurrentTexture()->GetTextureSize());

		Slot3_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot3_IconRenderer->SetImage("rio_acc_Ico.png", "PointSmp");
		Slot3_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 3.0f));
		Slot3_IconRenderer->GetTransform()->SetLocalScaling(Slot0_IconRenderer->GetCurrentTexture()->GetTextureSize());

		Slot4_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot4_IconRenderer->SetImage("rio_acc_Ico.png", "PointSmp");
		Slot4_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 4.0f));
		Slot4_IconRenderer->GetTransform()->SetLocalScaling(Slot0_IconRenderer->GetCurrentTexture()->GetTextureSize());

		Slot5_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot5_IconRenderer->SetImage("rio_acc_Ico.png", "PointSmp");
		Slot5_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 5.0f));
		Slot5_IconRenderer->GetTransform()->SetLocalScaling(Slot0_IconRenderer->GetCurrentTexture()->GetTextureSize());

		Select_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		Select_IconRenderer->SetImage("Selected_Ico.png", "PointSmp");
		Select_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 5.0f));
		Select_IconRenderer->GetTransform()->SetLocalScaling(Select_IconRenderer->GetCurrentTexture()->GetTextureSize());

		Select_IconRenderer->Off();
	}

}

void UI_ItemRoute::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}

	//UI 온오프 체크
	{
		if (false == UIOn)
		{
			//선택한거 초기화
			EquipType = -1;
			RouteRenderer->SetImage("IconBG_UI.png", "PointSmp");
			RouteRenderer->Off();
			Slot0_IconRenderer->Off();
			Slot1_IconRenderer->Off();
			Slot2_IconRenderer->Off();
			Slot3_IconRenderer->Off();
			Slot4_IconRenderer->Off();
			Slot5_IconRenderer->Off();
			Select_IconRenderer->Off();
		}
		else
		{
			RouteRenderer->On();
			Slot0_IconRenderer->On();
			Slot1_IconRenderer->On();
			Slot2_IconRenderer->On();
			Slot3_IconRenderer->On();
			Slot4_IconRenderer->On();
			Slot5_IconRenderer->On();
		}
	}

	if (true == GameEngineInput::GetInst().Down("B"))
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

	switch (EquipType)
	{
		case -1:
		{
			RouteRenderer->SetImage("IconBG_UI.png", "PointSmp");
		}
			break;
		case 0:
		{
			switch (myjob_)
			{
			case JobType::NONE:
				break;
			case JobType::YUKI:
				RouteRenderer->SetImage("yuki_weapon.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot0Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::HYUNWOO:
				RouteRenderer->SetImage("hyunwoo_weapon.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot0Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::JACKIE:
				RouteRenderer->SetImage("jackie_weapon.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot0Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::RIO:
				RouteRenderer->SetImage("rio_weapon.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot0Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::AYA:
				RouteRenderer->SetImage("aya_weapon.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot0Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::DUMMY:
				break;
			case JobType::MAX:
				break;
			default:
				break;
			}
		}
			break;
		case 1:
		{
			switch (myjob_)
			{
			case JobType::NONE:
				break;
			case JobType::YUKI:
				RouteRenderer->SetImage("yuki_body.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot1Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::HYUNWOO:
				RouteRenderer->SetImage("hyunwoo_body.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot1Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::JACKIE:
				RouteRenderer->SetImage("jackie_body.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot1Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::RIO:
				RouteRenderer->SetImage("rio_body.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot1Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::AYA:
				RouteRenderer->SetImage("aya_body.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot1Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::DUMMY:
				break;
			case JobType::MAX:
				break;
			default:
				break;
			}
		}
			break;
		case 2:
		{
			switch (myjob_)
			{
			case JobType::NONE:
				break;
			case JobType::YUKI:
				RouteRenderer->SetImage("yuki_head.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::HYUNWOO:
				RouteRenderer->SetImage("hyunwoo_head.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::JACKIE:
				RouteRenderer->SetImage("jackie_head.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::RIO:
				RouteRenderer->SetImage("rio_head.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::AYA:
				RouteRenderer->SetImage("aya_head.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::DUMMY:
				break;
			case JobType::MAX:
				break;
			default:
				break;
			}
		}
			break;
		case 3:
		{
			switch (myjob_)
			{
			case JobType::NONE:
				break;
			case JobType::YUKI:
				RouteRenderer->SetImage("yuki_arm.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot3Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::HYUNWOO:
				RouteRenderer->SetImage("hyunwoo_arm.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot3Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::JACKIE:
				RouteRenderer->SetImage("jackie_arm.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot3Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::RIO:
				RouteRenderer->SetImage("rio_arm.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot3Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::AYA:
				RouteRenderer->SetImage("aya_arm.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot3Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::DUMMY:
				break;
			case JobType::MAX:
				break;
			default:
				break;
			}
		}
			break;
		case 4:
		{
			switch (myjob_)
			{
			case JobType::NONE:
				break;
			case JobType::YUKI:
				RouteRenderer->SetImage("yuki_leg.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot4Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::HYUNWOO:
				RouteRenderer->SetImage("hyunwoo_leg.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot4Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::JACKIE:
				RouteRenderer->SetImage("jackie_leg.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot4Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::RIO:
				RouteRenderer->SetImage("rio_leg.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot4Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::AYA:
				RouteRenderer->SetImage("aya_leg.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot4Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::DUMMY:
				break;
			case JobType::MAX:
				break;
			default:
				break;
			}
		}
			break;
		case 5:
		{
			switch (myjob_)
			{
			case JobType::NONE:
				break;
			case JobType::YUKI:
				RouteRenderer->SetImage("yuki_acc.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot5Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::HYUNWOO:
				RouteRenderer->SetImage("hyunwoo_acc.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot5Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::JACKIE:
				RouteRenderer->SetImage("jackie_acc.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot5Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::RIO:
				RouteRenderer->SetImage("rio_acc.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot5Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::AYA:
				RouteRenderer->SetImage("aya_acc.png", "PointSmp");
				Select_IconRenderer->On();
				Select_IconRenderer->GetTransform()->SetLocalPosition(Slot5Collision->GetTransform()->GetLocalPosition());
				break;
			case JobType::DUMMY:
				break;
			case JobType::MAX:
				break;
			default:
				break;
			}
		}
			break;
		default:
			break;
	}
}

void UI_ItemRoute::SlotMouseCollisionCheck()
{

	if (true == Slot0Collision->Collision(eCollisionGroup::MousePointer))
	{
		EquipType = 0;
	}
	else if (true == Slot1Collision->Collision(eCollisionGroup::MousePointer))
	{
		EquipType = 1;
	}
	else if (true == Slot2Collision->Collision(eCollisionGroup::MousePointer))
	{
		EquipType = 2;
	}
	else if (true == Slot3Collision->Collision(eCollisionGroup::MousePointer))
	{
		EquipType = 3;
	}
	else if (true == Slot4Collision->Collision(eCollisionGroup::MousePointer))
	{
		EquipType = 4;
	}
	else if (true == Slot5Collision->Collision(eCollisionGroup::MousePointer))
	{
		EquipType = 5;
	}

}

void UI_ItemRoute::SetJobType(JobType _myjob)
{
	myjob_ = _myjob;

	switch (myjob_)
	{
	case JobType::NONE:
		break;
	case JobType::YUKI:
	{
		Slot0_IconRenderer->SetImage("yuki_weapon_Ico.png", "PointSmp");
		Slot1_IconRenderer->SetImage("yuki_body_Ico.png", "PointSmp");
		Slot2_IconRenderer->SetImage("yuki_head_Ico.png", "PointSmp");
		Slot3_IconRenderer->SetImage("yuki_arm_Ico.png", "PointSmp");
		Slot4_IconRenderer->SetImage("yuki_leg_Ico.png", "PointSmp");
		Slot5_IconRenderer->SetImage("yuki_acc_Ico.png", "PointSmp");
	}
	break;
	case JobType::HYUNWOO:
	{
		Slot0_IconRenderer->SetImage("hyunwoo_weapon_Ico.png", "PointSmp");
		Slot1_IconRenderer->SetImage("hyunwoo_body_Ico.png", "PointSmp");
		Slot2_IconRenderer->SetImage("hyunwoo_head_Ico.png", "PointSmp");
		Slot3_IconRenderer->SetImage("hyunwoo_arm_Ico.png", "PointSmp");
		Slot4_IconRenderer->SetImage("hyunwoo_leg_Ico.png", "PointSmp");
		Slot5_IconRenderer->SetImage("hyunwoo_acc_Ico.png", "PointSmp");
	}
	break;
	case JobType::JACKIE:
	{
		Slot0_IconRenderer->SetImage("jackie_weapon_Ico.png", "PointSmp");
		Slot1_IconRenderer->SetImage("jackie_body_Ico.png", "PointSmp");
		Slot2_IconRenderer->SetImage("jackie_head_Ico.png", "PointSmp");
		Slot3_IconRenderer->SetImage("jackie_arm_Ico.png", "PointSmp");
		Slot4_IconRenderer->SetImage("jackie_leg_Ico.png", "PointSmp");
		Slot5_IconRenderer->SetImage("jackie_acc_Ico.png", "PointSmp");
	}
	break;
	case JobType::RIO:
	{
		Slot0_IconRenderer->SetImage("rio_weapon_Ico.png", "PointSmp");
		Slot1_IconRenderer->SetImage("rio_body_Ico.png", "PointSmp");
		Slot2_IconRenderer->SetImage("rio_head_Ico.png", "PointSmp");
		Slot3_IconRenderer->SetImage("rio_arm_Ico.png", "PointSmp");
		Slot4_IconRenderer->SetImage("rio_leg_Ico.png", "PointSmp");
		Slot5_IconRenderer->SetImage("rio_acc_Ico.png", "PointSmp");
	}
	break;
	case JobType::AYA:
	{
		Slot0_IconRenderer->SetImage("aya_weapon_Ico.png", "PointSmp");
		Slot1_IconRenderer->SetImage("aya_body_Ico.png", "PointSmp");
		Slot2_IconRenderer->SetImage("aya_head_Ico.png", "PointSmp");
		Slot3_IconRenderer->SetImage("aya_arm_Ico.png", "PointSmp");
		Slot4_IconRenderer->SetImage("aya_leg_Ico.png", "PointSmp");
		Slot5_IconRenderer->SetImage("aya_acc_Ico.png", "PointSmp");
	}
	break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}
}
