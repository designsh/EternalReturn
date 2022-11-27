#include "Precompile.h"
#include "UI_ItemSettingHelper.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

UI_ItemSettingHelper::UI_ItemSettingHelper()
{
}

UI_ItemSettingHelper::~UI_ItemSettingHelper()
{
}

string UI_ItemSettingHelper::GetItemTextureName(ItemBase* _Item)
{
	//�ٳ����ؼ� �ڿ� .png�� �̹��� �̸��� �ִ°�
	//UI�� �� ������ ������ �̹����� ��� ������ Getname���ϰ��� _UI.png�� �־ �̹��� �̸��� ���ϴ°ŷ� ��
	string name = _Item->GetName();
	string Ext = ".png";

	//return "KitchenKnife.png";

	return name + Ext;
}

void UI_ItemSettingHelper::SetItemRenderValue(GameEngineUIRenderer* _Renderer, ItemBase* _Item)
{
	ItemTier Tier =	_Item->GetItemTier();

	switch (Tier)
	{
	case ItemTier::NONE:
		_Renderer->SetImage("ItemBg_Common.png");
		break;
	case ItemTier::COMMON:
		_Renderer->SetImage("ItemBg_Common.png");
		break;
	case ItemTier::UNCOMMON:
		_Renderer->SetImage("ItemBg_UnCommon.png");
		break;
	case ItemTier::RARE:
		_Renderer->SetImage("ItemBg_Rare.png");
		break;
	case ItemTier::EPIC:
		_Renderer->SetImage("ItemBg_Epic.png");
		break;
	case ItemTier::LEGENDARY:
		_Renderer->SetImage("ItemBg_Legendary.png");
		break;
	case ItemTier::MAX:
		_Renderer->SetImage("ItemBg_Common.png");
		break;
	default:
		break;
	}
}