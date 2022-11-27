#include "PreCompile.h"
#include "EquipmentItem.h"

EquipmentItem::EquipmentItem()
	: equipType_(EquipmentType::NONE)
{

}

EquipmentItem::~EquipmentItem()
{

}

ItemBase* EquipmentItem::Copy()
{
	EquipmentItem* CopyItem = GetLevel()->CreateActor<EquipmentItem>();
	CopyItem->SetName(GetName());
	CopyItem->type_ = type_;
	CopyItem->renderer_ = renderer_;
	CopyItem->tier_ = tier_;
	CopyItem->index_ = index_;
	CopyItem->equipType_ = equipType_;
	CopyItem->stat_ = stat_;

	return CopyItem;
}

void EquipmentItem::Start()
{
	ItemBase::Start();
	stat_.Clear();
}

void EquipmentItem::Update(float _DeltaTime)
{
	ItemBase::Update(_DeltaTime);
}

