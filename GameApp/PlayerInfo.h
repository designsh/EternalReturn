#pragma once
#include "CharacterStat.h"
/// <summary>
// 서버 인터페이스가 벡터 값으로 들고 있을 캐릭터 슬롯입니다.
// 각 패킷들의 변수는 Execute() 에서 해당 PlayerInfo 의 값들을 채워 주는 식으로 운용할 예정입니다.
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


	// 캐릭터 상태, 능력치
	CharacterStat* stat_;
};