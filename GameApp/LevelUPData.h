#pragma once

// 분류 : 데이터
// 용도 : 
// 설명 : 각 유닛 타입별 레벨업 정보를 정의
struct LevelUPData
{
	std::string TypeName_;						// 각 유닛의 고유명(Ex. Yuki, Rio, ...)
	float AttackPower_;							// 공격력
	float HP_;									// 체력
	float HPRegeneration_;						// 체력 재생
	float SP_;									// 스태미나
	float SPRegeneration_;						// 스태미나 재생
	float Defence_;								// 방어력
	float AttackSpeed_;							// 공격속도
	float CriticalChance_;						// 치명타
	float MovementSpeed_;						// 이동속도
	float VisionRange_;							// 시야
};
