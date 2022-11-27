#pragma once
#include "ItemBase.h"

enum class UseableItemType
{
	HP,
	SP
};

// Ό³Έν : 
class UseableItem : public ItemBase
{
	friend class ItemBoxManager;
	friend class Character;
public:
	UseableItem();
	~UseableItem();

	UseableItem(const UseableItem& _other) = delete; 
	UseableItem(UseableItem&& _other) noexcept = delete;
	UseableItem& operator=(const UseableItem& _other) = delete;
	UseableItem& operator=(const UseableItem&& _other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	UseableItemType RegenType_;

	float RegenValue_;
	float RegenTime_;
};

