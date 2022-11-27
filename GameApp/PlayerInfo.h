#pragma once
#include "CharacterStat.h"
/// <summary>
// ���� �������̽��� ���� ������ ��� ���� ĳ���� �����Դϴ�.
// �� ��Ŷ���� ������ Execute() ���� �ش� PlayerInfo �� ������ ä�� �ִ� ������ ����� �����Դϴ�.
/// </summary>
struct PlayerInfo
{
	int playerNumber_;
	int character_;
	int startPoint_;
	int isReady_;
	int IsLoading_;						// 220927 SJH ADD
	std::string playerNickname_;

	// Lumia Level Only
	float4 curPos_;
	float4 curRotation_;
	std::string curAnimation_;
	std::string overrideAnimationName_;
	std::string overrideAnimationBoneName_;


	// ĳ���� ����, �ɷ�ġ
	CharacterStat* stat_;
};