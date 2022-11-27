#pragma once
#include "ItemBase.h"
#include "GameEngine/GameEngineUIRenderer.h"

using namespace std;

//

class UI_ItemSettingHelper
{

public:
	UI_ItemSettingHelper();
	~UI_ItemSettingHelper();

public:
	static string GetItemTextureName(ItemBase* _Item);

	//�������� ���enum������ ������ ��޿� ���� �޹���� SetImage���ִ� �Լ�
	static void SetItemRenderValue(GameEngineUIRenderer* _Renderer, ItemBase* _Item);

private:
	UI_ItemSettingHelper(const UI_ItemSettingHelper& _Other) = delete;
	UI_ItemSettingHelper(UI_ItemSettingHelper&& _Other) noexcept = delete;
	UI_ItemSettingHelper& operator=(const UI_ItemSettingHelper& _Other) = delete;
	UI_ItemSettingHelper& operator=(UI_ItemSettingHelper&& _Other) noexcept = delete;
};

