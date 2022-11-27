#pragma once

#include <GameEngine/GameEngineGUI.h>

class ItemBoxManager;
class ItemBox;
class ItemBase;
class ItemListWindow : public GameEngineGUIWindow
{
public:
	ItemBoxManager* ItemBoxManager_;

public:
	ItemListWindow(); 
	~ItemListWindow();

	ItemListWindow(const ItemListWindow& _other) = delete; 
	ItemListWindow(ItemListWindow&& _other) = delete; 

	ItemListWindow& operator=(const ItemListWindow& _other) = delete;
	ItemListWindow& operator=(const ItemListWindow&& _other) = delete;

protected:
	virtual void OnGUI() override;

};

