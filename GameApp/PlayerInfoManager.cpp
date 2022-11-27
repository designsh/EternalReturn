#include "PreCompile.h"
#include "PlayerInfoManager.h"

PlayerInfoManager::PlayerInfoManager()
	: playerNumber_(-1)
	, myCharacter_(nullptr)
	, playerName_{}
{
}

PlayerInfoManager::~PlayerInfoManager()
{

}

void PlayerInfoManager::AddNewPlayer(PlayerInfo _playerInfo)
{
	serverPlayerList_.push_back(_playerInfo);
}

bool PlayerInfoManager::AllPlayerReadyStateCheck()
{
	// ���� ���ӿ� ������ ��� �÷��̾���� �غ���¸� üũ
	for (int PlayerNum = 0; PlayerNum < static_cast<int>(serverPlayerList_.size()); ++PlayerNum)
	{
		// 0 == IsReady_ �̶�� �غ� �������� ������ �÷��̾ �����Ѵٴ� �ǹ�
		if (0 == serverPlayerList_[PlayerNum].isReady_)
		{
			return false;
		}
	}

	return true;
}

bool PlayerInfoManager::AllPlayerLoadingStateCheck()
{
	// ���� ���ӿ� ������ ��� �÷��̾���� �ε����¸� üũ
	for (int PlayerNum = 0; PlayerNum < static_cast<int>(serverPlayerList_.size()); ++PlayerNum)
	{
		// 0 == IsLoading_ �̶�� �ε��� �Ϸ�������� �÷��̾ �����Ѵٴ� �ǹ�
		if (0 == serverPlayerList_[PlayerNum].IsLoading_)
		{
			return false;
		}
	}

	return true;
}
