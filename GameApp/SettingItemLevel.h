#pragma once
#include <GameEngine/GameEngineLevel.h>

// Ό³Έν : 
class ItemBoxWindow;
class ItemBoxManager;
class MousePointer;
class SettingItemLevel : public GameEngineLevel
{
public:
	SettingItemLevel();
	~SettingItemLevel();
	SettingItemLevel(const SettingItemLevel& _other) = delete; 
	SettingItemLevel(SettingItemLevel&& _other) noexcept = delete;
	SettingItemLevel& operator=(const SettingItemLevel& _other) = delete;
	SettingItemLevel& operator=(const SettingItemLevel&& _other) = delete;

	MousePointer* GetMousePointer() { return mouse_; }

protected:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

private:
	void CreateActorLevel();

	ItemBoxManager* ItemBox_;
	ItemBoxWindow* Window_;
	MousePointer* mouse_;
};

