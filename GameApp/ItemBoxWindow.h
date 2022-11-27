#pragma once
#include <GameEngine/GameEngineGUI.h>

// Ό³Έν : 
class ItemBoxManager;
class ItemBox;
class ItemBase;
class ItemBoxWindow : public GameEngineGUIWindow
{
public:
	ItemBoxWindow();
	~ItemBoxWindow();
	ItemBoxWindow(const ItemBoxWindow& _other) = delete; 
	ItemBoxWindow(ItemBoxWindow&& _other) noexcept = delete;
	ItemBoxWindow& operator=(const ItemBoxWindow& _other) = delete;
	ItemBoxWindow& operator=(const ItemBoxWindow&& _other) = delete;

protected:
	void OnGUI() override;

private:

public:
	ItemBoxManager* ItemBoxManager_;
	ItemBox* SelectBox_;
	std::string itemName;
	int selectItemIndex;
	int selectAllItemIndex;
};

