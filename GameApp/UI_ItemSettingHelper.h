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

	//렌더러와 등급enum같은걸 넣으면 등급에 맞춘 뒷배경을 SetImage해주는 함수
	static void SetItemRenderValue(GameEngineUIRenderer* _Renderer, ItemBase* _Item);

private:
	UI_ItemSettingHelper(const UI_ItemSettingHelper& _Other) = delete;
	UI_ItemSettingHelper(UI_ItemSettingHelper&& _Other) noexcept = delete;
	UI_ItemSettingHelper& operator=(const UI_ItemSettingHelper& _Other) = delete;
	UI_ItemSettingHelper& operator=(UI_ItemSettingHelper&& _Other) noexcept = delete;
};

