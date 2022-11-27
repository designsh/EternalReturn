#include "PreCompile.h"
#include "ItemBoxWindow.h"
#include "ItemBoxManager.h"
#include "ItemBase.h"
#include "ItemBox.h"
#include "UI_ItemBox.h"
#include <GameEngine/GameEngineCore.h>
#include "PlayerInfoManager.h"
#include "Character.h"
#include <GameEngine/GameEngineCore.h>
#include "LumiaLevel.h"
#include "LumiaMap.h"

ItemBoxWindow::ItemBoxWindow()
	: ItemBoxManager_(nullptr)
	, SelectBox_(nullptr)
	, selectItemIndex(-1)
	, selectAllItemIndex(-1)
{
}

ItemBoxWindow::~ItemBoxWindow()
{

}

void ItemBoxWindow::OnGUI()
{
	ImGui::Text("Inventory");

	std::vector<const char*> invenListName;

	Character* CurMainPlayer =  PlayerInfoManager::GetInstance()->GetMainCharacter();
	if (nullptr == CurMainPlayer)
	{
		return;
	}

	std::vector<ItemBase*> vecInven = CurMainPlayer->GetInventory();

	for (auto& item : vecInven)
	{
		if (nullptr == item)
		{
			continue;
		}

		invenListName.push_back(item->GetName().c_str());
	}

	if (invenListName.empty())
	{
		ImGui::Text("Inventory Empty");
	}
	else
	{
		int selectIndex = -1;
		ImGui::PushItemWidth(200);
		ImGui::ListBox("##invenlist", &selectIndex, &invenListName[0],
			static_cast<ImGuiID>(invenListName.size()));
	}

	ImGui::Text("Build Items Current Location");

	CurMainPlayer = PlayerInfoManager::GetInstance()->GetMainCharacter();
	int mapIndex = static_cast<int>(CurMainPlayer->GetCurLocation());

	LumiaLevel* level = dynamic_cast<LumiaLevel*>(GameEngineCore::CurrentLevel());

	if (nullptr == level ||
		"LumiaLevel" != level->GetName())
	{
		return;
	}

	std::vector<std::string> vecstr = ItemBoxManager_->GetAreaItemsName(level->GetMap()->GetMapName(mapIndex));
	vecstr.push_back("Pebbles");
	vecstr.push_back("Flower");
	vecstr.push_back("Branch");

	std::vector<const char*> builditemList;

	for (const auto& cstr : vecstr)
	{
		for (ItemBase* builditem : CurMainPlayer->GetAllMyBuildItems())
		{
			if (cstr == builditem->GetName())
			{
				builditemList.push_back(cstr.c_str());
			}
		}
	}

	
	ImGui::PushItemWidth(200);
	ImGui::ListBox("##builditemlist", &selectItemIndex, &builditemList[0],
		static_cast<ImGuiID>(builditemList.size()));

	if (-1 != selectItemIndex)
	{
		CurMainPlayer = PlayerInfoManager::GetInstance()->GetMainCharacter();
		CurMainPlayer->getItem(std::string(builditemList[selectItemIndex]));
		selectItemIndex = -1;
	}


	ImGui::Text("Build Items All Location");

	builditemList.clear();

	for (ItemBase* builditem : CurMainPlayer->GetAllMyBuildItems())
	{
		builditemList.push_back(builditem->GetName().c_str());
	}

	ImGui::PushItemWidth(200);
	ImGui::ListBox("##builditemlistAll", &selectAllItemIndex, &builditemList[0],
		static_cast<ImGuiID>(builditemList.size()));

	if (-1 != selectAllItemIndex)
	{
		CurMainPlayer = PlayerInfoManager::GetInstance()->GetMainCharacter();
		CurMainPlayer->getItem(std::string(builditemList[selectAllItemIndex]));
		selectAllItemIndex = -1;
	}

	//char buf[255]{};
	//memcpy(buf, itemName.c_str(), sizeof(buf));
	//ImGui::InputText("##getiteminput", buf, sizeof(buf));
	//itemName = buf;


	//if (true == ImGui::Button("GetItem"))
	//{
	//	CurMainPlayer = PlayerInfoManager::GetInstance()->GetMainCharacter();
	//	CurMainPlayer->getItem(itemName);
	//}

	SelectBox_ = ItemBoxManager_->GetSelectBox();

	if (nullptr != SelectBox_)
	{
		const char* Arr = SelectBox_->area.c_str();
		ImGui::Text(Arr);
		ImGui::SameLine();
		ImGui::Text(std::to_string(SelectBox_->info.Index).c_str());

		Arr = SelectBox_->info.BoxType.c_str();
		ImGui::Text(Arr);

		std::vector<const char*> listName;

		for (auto& Item : SelectBox_->itemList)
		{
			//listName.push_back(GameEngineString::AnsiToUTF8Return(Item->GetName()).c_str());
			if (true == Item->IsDestroyed_)
			{
				continue;
			}

			listName.push_back(Item->GetName().c_str());
		}

		if (listName.empty())
		{
			ImGui::Text("ItemBox Empty");
		}
		else
		{
			int selectIndex = -1;
			ImGui::PushItemWidth(200);
			ImGui::ListBox("##ItemList", &selectIndex, &listName[0],
				static_cast<ImGuiID>(listName.size()));
		}
	}

}
