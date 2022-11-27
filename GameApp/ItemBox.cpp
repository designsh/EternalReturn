#include "PreCompile.h"
#include "ItemBox.h"
#include "ItemBase.h"
#include "UI_ItemBox.h"
#include <GameEngineBase/GameEngineSoundPlayer.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineRandom.h>

ItemBox::ItemBox()
	: col(nullptr)
	, isGatherBox(false)
	, itemBoxUI_(nullptr)
	, bOpen_(false)
{

}

ItemBox::~ItemBox()
{

}

ItemBase* ItemBox::GetItem(int _index)
{
	int findIndex = 0;

	for (auto& item : itemList)
	{
		if (findIndex == _index)
		{
			return item->Copy();
		}

		++findIndex;
	}

	return nullptr;
}

bool ItemBox::DeleteItem(int _index)
{

	int findIndex = 0;
	std::list<ItemBase*>::iterator iter = itemList.begin();

	for (; iter != itemList.end(); ++iter)
	{
		if (findIndex == _index)
		{
			(*iter)->Release();
			itemList.erase(iter);
			break;
		}

		++findIndex;
	}

	return true;
}

void ItemBox::Open()
{
	if (true == bOpen_)
	{
		return;
	}

	if (itemBoxUI_ != nullptr)
	{
		itemBoxUI_->Release();
		itemBoxUI_ = nullptr;
	}
	itemBoxUI_ = GetLevel()->CreateActor<UI_ItemBox>();
	itemBoxUI_->RenderOn();
	itemBoxUI_->GetItemBoxInfo(this);

	bOpen_ = true;
}

void ItemBox::Close()
{
	if (false == bOpen_)
	{
		return;
	}

	if (itemBoxUI_ != nullptr)
	{
		itemBoxUI_->Release();
		itemBoxUI_ = nullptr;
	}

	bOpen_ = false;
}

void ItemBox::Start()
{
	
}

void ItemBox::Update(float _DeltaTime)
{
}

void ItemBox::playOpenSound()
{
	GameEngineDirectory SoundDir;
	SoundDir.MoveParent("EternalReturn");
	SoundDir / "Resources" / "Sound" / "ItemBox";

	std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("wav");

	std::list<std::string> strList;

	for (size_t i = 0; i < AllFile.size(); i++)
	{
		strList.push_back(AllFile[i].GetFileNameWithOutExtension());
	}

	GameEngineSoundPlayer player;

	std::string soundName = "";
	
	for (const auto& str : strList)
	{
		if (std::string::npos != info.BoxType.find(str))
		{
			soundName = str;
			break;
		}

	}

	if (std::string::npos != info.BoxType.find("Drum"))
	{
		GameEngineRandom random;
		int count = random.RandomInt(1, 3);
		soundName = "Drum_0" + to_string(count);
	}

	if (soundName.empty())
	{
		return;
	}

	soundName.append(".wav");

	player.Stop();
	player.ChangeSound(soundName);
	player.Play();
	player.SetVolume(0.5f);
}

