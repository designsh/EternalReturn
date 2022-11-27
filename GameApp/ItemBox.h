#pragma once
#include <GameEngine/GameEngineActor.h>

class ItemBoxInfo
{
public:
	std::string BoxType;
	int Index;

	float4 Pos;
	float4 Scale;

	ItemBoxInfo()
		: Index(0)
	{}
};

// Ό³Έν : 
class ItemBase;
class GameEngineCollision;
class UI_ItemBox;
class ItemBox : public GameEngineActor
{
	friend class ItemBoxManager;
	friend class ItemBoxWindow;
	friend class Monsters;
public:
	ItemBox();
	~ItemBox();
	ItemBox(const ItemBox& _other) = delete; 
	ItemBox(ItemBox&& _other) noexcept = delete;
	ItemBox& operator=(const ItemBox& _other) = delete;
	ItemBox& operator=(const ItemBox&& _other) = delete;

	UI_ItemBox* GetItemBoxUI()
	{
		return itemBoxUI_;
	}

	GameEngineCollision* GetCollision() { return col; }
	ItemBase* GetItem(int _index);
	bool DeleteItem(int _index);

	bool IsGatherBox()
	{
		return isGatherBox;
	}

	bool isOpen()
	{
		return bOpen_;
	}

	void Open();
	void Close();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
public:
	void playOpenSound();

private:

	ItemBoxInfo info;
	GameEngineCollision* col;
	std::string area;

	std::list<ItemBase*> itemList;

	bool isGatherBox;

	UI_ItemBox* itemBoxUI_;
	bool bOpen_;
};

