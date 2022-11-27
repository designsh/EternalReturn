#pragma once
#include <vector>
#include "Enums.h"

// 분류 : 정보
// 용도 : 
// 설명 : 몬스터의 상태정보
struct MonsterStateInfo
{
//====================================== 스텟의 최대치(고정)
	int LevelMin_;							// 시작 레벨
	int LevelMax_;							// 최대 레벨
	float HomingInstinctValueMax_;			// 최대 귀소본능 수치
	float RegenTimeMax_;					// 리젠타임
	float SkillCoolDownMax_;				// 최대 스킬쿨타임

//====================================== 레벨당 증가량(고정)
	int OffencePowerIncrement_;				// 레벨당 공격력 증가량
	int DefenseIncrement_;					// 레벨당 방어력 증가량
	float MoveSpeedIncrement_;				// 레벨당 이동속도 증가량
	float HPIncrement_;						// 레벨당 체력 증가량

//====================================== 기본스텟(고정)
	float4 NestPosition_;					// 둥지위치(= 스폰위치)
	float AttackSpeed_;						// 공격속도
	float AttackRange_;						// 공격 시야거리
	float DetectRange_;						// 감지 시야거리

//====================================== 기본스텟(갱신)
	int Level_;								// 레벨(늦게 생성된 몬스터(야생동물)일수록 레벨이 높게 설정) - 생성과 동시에 지정(지정없이 생성된 몬스터의 경우 기본 1레벨고정)
	int OffencePower_;						// 공격력
	int Defense_;							// 방어력
	float HPMax_;							// 최대 체력(레벨업당 갱신)
	float HP_;								// 현재체력
	float MoveSpeed_;						// 이동속도
	float HomingInstinctValue_;				// 귀소본능 수치(0.0f보다 작거나같아지면 몬스터는 스폰위치로 이동하면 체력을 회복 -> 타겟이 DetectRange_에 벗어난 시간에 따라 감소)
	float RegenTime_;						// 리젠타임(갱신) -> 0.0f이하일시 RegenTime_으로 초기화

//====================================== 스킬관련
	int IsSkill_;							// 스킬여부(0: 스킬없음)
	float SkillCoolDown_;					// 스킬재사용시간

//====================================== 아이템관련
	std::map<ItemName, float> DropItems_;	// std::map<아이템명, 확률> : 스텟초기화시 정의
	std::vector<ItemName> CurDropItems_;	// 사망시 드랍된 아이템목록 : 사망시 Items_에서 확률을 체크하여 목록작성 -> 서버에서 결정된 후 패킷전송
};
