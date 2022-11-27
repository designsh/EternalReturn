#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineImageRenderer.h>

// Equipment	장비
// Useable		소비
// Misc			기타

class ItemBase : public GameEngineActor
{
	friend class ItemBoxManager;
public:
	ItemBase();
	~ItemBase();

	ItemBase(const ItemBase& _other) = delete; 
	ItemBase(ItemBase&& _other) noexcept = delete;
	ItemBase& operator=(const ItemBase& _other) = delete;
	ItemBase& operator=(const ItemBase&& _other) = delete;

	//ItemBase* CombineItem(ItemBase* _Other);

	void SetItemType(ItemType _type)
	{
		type_ = _type;
	}
	void SetItemTier(ItemTier _tier)
	{
		tier_ = _tier;
	}
	void SetIndex(int _index)
	{
		index_ = _index;
	}

	
	ItemType GetItemType()
	{
		return type_;
	}
	ItemTier GetItemTier()
	{
		return tier_;
	}
	int GetIndex()
	{
		return index_;
	}

	void SetImage(const std::string& _imageName, const std::string& _sampler = "");

	// ItemType 따라서 Copy 할 정보가 다름 virtual
	virtual ItemBase* Copy();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	ItemType type_;
	ItemTier tier_;
	int index_;

	GameEngineImageRenderer* renderer_;
};

