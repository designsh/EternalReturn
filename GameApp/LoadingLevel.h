#pragma once
#include <GameEngine/GameEngineLevel.h>

#include "Enums.h"

// �з� : ����(��)
// �뵵 : 
// ���� : �÷����Ϸ��� ������ ��� ��, ������Ʈ, ����, �÷��̾� ���� ������ɱ�� ����
class GameEngineThreadQueue;
class SynchronizationInfoManager;
class LoadingLevel_BackDrop;
class LoadingLevel_LoadPercent;
class LoadingLevel : public GameEngineLevel
{
public: // Static Value & Function
	static bool ResourceLoadEndCheck;
	static bool ReadyCreationCommand;
	static bool ThreadLoadingEnd;

public:
protected:
private:
	void LoadingLevelInitalize();

private:
	void CreationCommand();

private:
	void CheckThreadCreationInfoSettingEnd();
	void CheckThreadLoadingEnd();

private:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

public:
	LoadingLevel();
	~LoadingLevel();

protected:
	LoadingLevel(const LoadingLevel& _other) = delete;
	LoadingLevel(LoadingLevel&& _other) noexcept = delete;

private:
	LoadingLevel& operator=(const LoadingLevel& _other) = delete;
	LoadingLevel& operator=(const LoadingLevel&& _other) = delete;

public:
protected:
private:
	LoadingLevel_BackDrop* BackDrop_;
	LoadingLevel_LoadPercent* LoadPercent_;
};

