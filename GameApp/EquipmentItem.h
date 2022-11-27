#pragma once
#include "ItemBase.h"
#include "CharacterStat.h"

// Ό³Έν : 
class EquipmentItem : public ItemBase
{
	friend class ItemBoxManager;
public:
	EquipmentItem();
	~EquipmentItem();

	EquipmentItem(const EquipmentItem& _other) = delete; 
	EquipmentItem(EquipmentItem&& _other) noexcept = delete;
	EquipmentItem& operator=(const EquipmentItem& _other) = delete;
	EquipmentItem& operator=(const EquipmentItem&& _other) = delete;

	void SetEquipType(EquipmentType _type)
	{
		equipType_ = _type;
	}

	EquipmentType GetEquipType()
	{
		return equipType_;
	}

	CharacterStat GetStat()
	{
		return stat_;
	}

	ItemBase* Copy() override;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	EquipmentType equipType_;
	CharacterStat stat_;
};

