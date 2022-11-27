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
	// 현재 게임에 참여한 모든 플레이어들의 준비상태를 체크
	for (int PlayerNum = 0; PlayerNum < static_cast<int>(serverPlayerList_.size()); ++PlayerNum)
	{
		// 0 == IsReady_ 이라면 준비가 되지않은 상태인 플레이어가 존재한다는 의미
		if (0 == serverPlayerList_[PlayerNum].isReady_)
		{
			return false;
		}
	}

	return true;
}

bool PlayerInfoManager::AllPlayerLoadingStateCheck()
{
	// 현재 게임에 참여한 모든 플레이어들의 로딩상태를 체크
	for (int PlayerNum = 0; PlayerNum < static_cast<int>(serverPlayerList_.size()); ++PlayerNum)
	{
		// 0 == IsLoading_ 이라면 로딩이 완료되지않은 플레이어가 존재한다는 의미
		if (0 == serverPlayerList_[PlayerNum].IsLoading_)
		{
			return false;
		}
	}

	return true;
}
