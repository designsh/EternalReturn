#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEngineRandom.h>
#include "ItemBase.h"

//#define USERSAVE

struct CombineItem
{
	friend struct QueueItem;
public:
	ItemBase* left_;
	ItemBase* right_;

	CombineItem(ItemBase* _left, ItemBase* _right)
	{
		if (_left->GetName() <= _right->GetName())
		{
			left_ = _left;
			right_ = _right;
		}
		else
		{
			left_ = _right;
			right_ = _left;
		}
	}

	bool const operator==(const CombineItem& o)
	{
		return left_->GetName() == o.left_->GetName() &&
			right_->GetName() == o.right_->GetName();
	}

	bool const operator<(const CombineItem& o) const
	{
		if (left_->GetName() != o.left_->GetName())
		{
			return left_->GetName() < o.left_->GetName();
		}

		if (right_->GetName() != o.right_->GetName())
		{
			return right_->GetName() < o.right_->GetName();
		}

		return false;
	}

private:
	// 정렬을 위해서 막음
	CombineItem()
		: left_(nullptr)
		, right_(nullptr)
	{}
};

struct QueueItem
{
public:
	CombineItem CI_;
	bool isMyBuild_;

	QueueItem()
		: isMyBuild_(false)
	{};
};

// 설명 : 
class MiscItem;
class UseableItem;
class EquipmentItem;
class ItemBox;
class UI_ItemBox;
class GameEngineFBXRenderer;
class ItemBoxManager : public GameEngineActor
{
public:
	ItemBoxManager();
	~ItemBoxManager();

	ItemBoxManager(const ItemBoxManager& _other) = delete; 
	ItemBoxManager(ItemBoxManager&& _other) noexcept = delete;
	ItemBoxManager& operator=(const ItemBoxManager& _other) = delete;
	ItemBoxManager& operator=(const ItemBoxManager&& _other) = delete;

	void UserSave(const std::string& _Path);
	void UserLoad(const std::string& _Path);
	void UserAllLoad(GameEngineDirectory _Dir);

	void UserSave_ItemListInfo();
	void UserLoad_ItemListInfo();

	void SetItemBoxUI(UI_ItemBox* _ui)
	{
		itemBoxUI_ = _ui;
	}

	ItemBox* GetSelectBox()
	{
		return selectBox_;
	}

	UI_ItemBox* GetItemBoxUI()
	{
		return itemBoxUI_;
	}

	std::map<CombineItem, ItemBase*>& GetAllItemRecipes()
	{
		return itemRecipes_;
	}

	std::list<ItemBase*> GetAllItemList()
	{
		return allItemList_;
	}

	std::vector<std::string> GetAreaItemsName(const std::string& _area)
	{
		return areaItemsName_[_area];
	}

	bool isOpen()
	{
		return bOpen_;
	}

	void ResetSelectBox()
	{
		selectBox_ = nullptr;
	}

	void OpenItemBox();
	void CloseItemBox();
	ItemBase* GetItemFromItemBox(int _index);
	void DeleteItemFromItemBox(int _index);
	ItemBase* GetItemFromItemList(const std::string& _itemName);

	void PushRandomItem(const std::string& _area, const std::string& _item, int _amount = 1);
	void SetGatherItem();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void CreateItemBoxInfo(const std::string& _Name);
	void BoxSelectUpdate();
	void DebugRender();

	// Item 관련
	void CreateAllItemList();
	void CreateMiscItemList();
	void CreateUseableItemList();
	void CreateEquipmentItemList();
	void CreateWeaponItemList();

	ItemBase* CreateItem(const std::string _Name, ItemType _Type, ItemTier _Tier,
		const std::string _ImageName = "");

	// _ImageName 을 지정해주지 않는다면 _Name + ".png" 로 지정
	MiscItem* CreateMiscItem(const std::string _Name, 
		const std::string _ImageName, ItemTier _Tier = ItemTier::COMMON);
	MiscItem* CreateMiscItem(const std::string _Name, 
		ItemTier _Tier = ItemTier::COMMON);

	UseableItem* CreateUseableItem(const std::string _Name,
		const std::string _ImageName, ItemTier _Tier = ItemTier::COMMON);
	UseableItem* CreateUseableItem(const std::string _Name,
		ItemTier _Tier = ItemTier::COMMON);

	EquipmentItem* CreateEquipmentItem(const std::string _Name,
		EquipmentType _EquipType, 
		const std::string _ImageName, ItemTier _Tier = ItemTier::COMMON);
	EquipmentItem* CreateEquipmentItem(const std::string _Name,
		EquipmentType _EquipType, 
		ItemTier _Tier = ItemTier::COMMON);

	void CreateAllItemRecipes();
	void CreateItemRecipe(const std::string& _left, const std::string& _right,
		const std::string& _result);

private:
	std::string itemBoxInfoPath_;
	std::map<std::string, std::vector<ItemBox*>> itemBoxs_;
	std::map<std::string, std::vector<std::string>> areaItemsName_;
	ItemBox* selectBox_;
	bool bOpen_;

	std::list<ItemBase*> allItemList_;
	GameEngineRandom randomManager_;
	std::map<CombineItem, ItemBase*> itemRecipes_;

	//이건호 UI아이템 박스 작업
private:
	UI_ItemBox* itemBoxUI_;
};

