#include "PreCompile.h"
#include "ItemListWindow.h"
#include "ItemBoxManager.h"
#include "Character.h"
#include "PlayerInfoManager.h"

ItemListWindow::ItemListWindow()
{

}

ItemListWindow::~ItemListWindow()
{

}

void ItemListWindow::OnGUI()
{
	std::list<ItemBase*> allItems = ItemBoxManager_->GetAllItemList();

	for (ItemBase* item : allItems)
	{
		std::string itemName = item->GetName();
		if (ImGui::Button(itemName.c_str()))
		{
			Character* player = PlayerInfoManager::GetInstance()->GetMainCharacter();
			player->getItem(itemName);
		}
	}
}