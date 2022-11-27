#pragma once
#include "ItemBase.h"

// Ό³Έν : 
class MiscItem : public ItemBase
{
public:
	MiscItem();
	~MiscItem();

	MiscItem(const MiscItem& _other) = delete; 
	MiscItem(MiscItem&& _other) noexcept = delete;
	MiscItem& operator=(const MiscItem& _other) = delete;
	MiscItem& operator=(const MiscItem&& _other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

