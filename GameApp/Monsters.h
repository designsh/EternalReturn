#pragma once
#include <GameEngine/GameEngineFSM.h>

#include "LevelUPData.h"
#include "IUnit.h"
#include "MonsterStateInfo.h"

// 분류 : 몬스터
// 용도 : 몬스터 기본(부모)클래스
// 설명 : 현재 맵에 배치되는 몬스터들의 공통정보
class GameEnginePreprocessingRenderer;
class GameEngineFBXRenderer;
class GameEngineCollision;
class NavFace;
class NavMesh;
class LumiaMap;
class Character;
class ItemBox;
class Monsters : public IUnit
{
public: // Static Value
	
public: // inline Get Function
	inline MonsterStateBasicType GetMonsterBasicStateType() const
	{
		return CurStateBasicType_;
	}

	inline MonsterStateType GetMonsterDetailStateType() const
	{
		return CurStateType_;
	}

	inline MonsterStateInfo GetMonsterStateInfo() const
	{
		return StateInfo_;
	}

	inline Location GetMonsterAreaType() const
	{
		return AreaType_;
	}

	inline MonsterType GetMonsterType() const
	{
		return Type_;
	}

	inline int GetTargetIndex() const
	{
		return CurTargetIndex_;
	}

	inline Character* GetCurTarget() const
	{
		return CurTarget_;
	}

	inline bool IsHidden() 
	{
		return bHidden_; 
	}

	inline bool IsDead()
	{
		return IsDeath_;
	}

public:
	void GiveItemToPlayer(int _index);

public: // inline Set Function
	inline void SetMonsterIndex(int _Index)
	{
		Index_ = _Index;
	}

	inline void SetMonsterAreaType(Location _AreaType)
	{
		AreaType_ = _AreaType;
	}

public: // Public Monster LevelUP Related Function
	void LevelUP(LevelUPData _Data);													// 게임시간에 따른 레벨업

public: // Functions related to outline activation and deactivation
	void PickingOutLineOn();															// 마우스와 충돌로 인한 외곽선 활성화
	void PickingOutLineOff();															// 마우스와 충돌해제로 인한 외곽선 비활성화

public: // Public Pure Virtual Function
	int GetIndex() override;															// 인덱스 Get
	void Damage(float _Amount, IUnit* _Target) override;								// 외부에서 호출하며 해당 몬스터에 데미지줄때 호출되는 함수

public: // Public Monster Show & Hide Function
	void Show();																		// 몬스터가 보유한 모든 렌더러를 On
	void Hide();																		// 몬스터가 보유한 모든 렌더러를 Off

public: // 패킷수신시 호출되는 함수들
	void rcvAttack01(MonsterStateInfo _rcvStatInfo);									// 패킷수신으로 공격 처리(동기화처리)
	void rcvAttack02(MonsterStateInfo _rcvStatInfo);									// 패킷수신으로 공격 처리(동기화처리)
	void rcvDamage(MonsterStateInfo _rcvStatInfo, int _TargetIndex);					// 패킷수신으로 데미지 처리(동기화처리)
	void rcvDeath(MonsterStateInfo _rcvStatInfo);										// 패킷수신으로 사망중처리(동기화처리)
	void rcvDead(MonsterStateInfo _rcvStatInfo);										// 패킷수신으로 완전사망처리(동기화처리)
	void rcvRegen(MonsterStateInfo _rcvStatInfo);										// 패킷수신으로 재등장처리(동기화처리)
	void rcvHomingInstinct(MonsterStateInfo _rcvStatInfo);								// 패킷수신으로 귀환처리(동기화처리)
	void rcvDamage();																	// 패킷수신으로 데미지처리(동기화처리)
	void rcvStun(float _StunTime);														// 패킷수신으로 스턴처리(동기화처리)
	void rcvKnockback(float _KnockbackTime, float4 _KnockbackSpeed);					// 패킷수신으로 넉백처리(동기화처리)
	void rcvWallSlam(float _KnockbackTime, float4 _KnockbackSpeed, float _StunTime);	// 패킷수신으로 벽스턴처리(동기화처리)

public: // Initalize Function
	void InitalizeSpawnPosition(const float4& _SpawnPosition);							// 인자로 수신한 _SpawnPosition을 해당 몬스터의 스폰위치로 지정(클라이언트전용)

public: // public ChangeAnimation Function
	void ChangeAnimationAndState(MonsterStateType _StateType);							// 몬스터상태타입으로 애니메이션 및 상태 전환

public: // Public About Stat Update Function
	void UpdateSpecialStat(MonsterStateInfo _UpdateStat);								// 특정 스텟을 수신하여 갱신
	void UpdateAllStat(MonsterStateInfo _UpdateStat);									// 전체 스텟을 수신하여 갱신

public: // Public CC Related Function
	void Stun(float _stunTime) override;
	void Knockback(float _knockbackTime, float4 _knockbackSpeed) override;
	void WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime) override;
	void Slow(float _slowTime, float _slowRatio) override;
public:
protected: // Protected Pure Virtual Function
	virtual void InitalizeStateInfo() = 0;												// 몬스터 상태정보 초기화
	virtual void InitalizeResourceLoad() = 0;											// 최초 리소스 로드
	virtual void InitalizeRenderAndAnimation() = 0;										// 렌더러 & 애니메이션 생성
	virtual void InitalizeCollider() = 0;												// 충돌체 생성

protected: // Protected SkillAttack Pure Virtual Function
	virtual void StartSkillAttackProcessing() = 0;										// 몬스터 타입별 스킬공격시작처리
	virtual void UpdateSkillAttackProcessing(float _DeltaTime) = 0;						// 몬스터 타입별 스킬공격갱신처리
	virtual void EndSkillAttackProcessing() = 0;										// 몬스터 타입별 스킬공격종료처리

private: // Private Initalize Function
	void InitalizeFSMState();															// FSM 상태 생성(공통)

private: // Private Update Function
	void CheckMonsterStateInfo(float _DeltaTime);										// 상태정보 체크(상태별 강제상태전환 체크)
	void CheckAllCollision(float _DeltaTime);											// 충돌체 충돌체크

private: // Collision Related Function
	void CheckBodyCollision(float _DeltaTime);											// 몸체충돌(피격판정)
	void CheckAttackCollision(float _DeltaTime);										// 일반공격체충돌(공격판정)
	void CheckSkillAttackCollision(float _DeltaTime);									// 스킬공격체충돌(공격판정)

private: // GlobalTime or Global Value Update Function
	void HomingInstinctValueUpdate(float _DeltaTime);									// 귀소본능 수치 갱신
	void UpdateSkillCoolDown(float _DeltaTime);											// 스킬쿨타임 갱신

private: // Private Move Related Function
	void StartMove(const float4& _Position);											// 이동시작
	void UpdateMove(float _DeltaTime);													// 이동갱신
	void EndMove();																		// 이동종료
	void MoveProcessing(float _DeltaTime, const float4& _Position);						// 실질적인 이동처리
	void CalcMoveDir(const float4& _Position);											// 이동방향 설정 및 이동방향에 맞게 렌더러 회전

private: // Private Attack Related Function
	void AttackProcessing();															// 실질적인 공격처리

private: // FSM State Function
	//==================================== Main State
	void StartNormalState();															// 일반상태 시작
	void UpdateNormalState(float _DeltaTime);											// 일반상태 갱신
	void EndNormalState();																// 일반상태 종료

	void StartCrowdControlState();														// 군중제어상태 시작
	void UpdateCrowdControlState(float _DeltaTime);										// 군중제어상태 갱신
	void EndCrowdControlState();														// 군중제어상태 종료

	void StartAttackState();															// 공격상태 시작
	void UpdateAttackState(float _DeltaTime);											// 공격상태 갱신
	void EndAttackState();																// 공격상태 종료

	//==================================== Normal State
	void StartAppearState();															// 첫등장상태 시작(최초 패킷수신시(서버는 즉시실행))
	void UpdateAppearState(float _DeltaTime);											// 첫등장상태 갱신
	void EndAppearState();																// 첫등장상태 종료

	void StartRegenState();																// 리젠상태 시작(정보초기화)
	void UpdateRegenState(float _DeltaTime);											// 리젠상태 갱신
	void EndRegenState();																// 리젠상태 종료

	void StartIdleState();																// 대기상태 시작
	void UpdateIdleState(float _DeltaTime);												// 대기상태 갱신
	void EndIdleState();																// 대기상태 종료

	void StartHomingInstinctState();													// 귀소본능상태 시작(타겟해제)
	void UpdateHomingInstinctState(float _DeltaTime);									// 귀소본능상태 갱신
	void EndHomingInstinctState();														// 귀소본능상태 종료

	void StartChaseState();																// 타겟추적상태 시작(타겟존재)
	void UpdateChaseState(float _DeltaTime);											// 타겟추적상태 갱신
	void EndChaseState();																// 타겟추적상태 종료

	//==================================== CrowdControl State
	void StartHitState();																// 피격상태 시작(타겟최초지정)
	void UpdateHitState(float _DeltaTime);												// 피격상태 갱신
	void EndHitState();																	// 피격상태 종료

	void StartDeathState();																// 사망중상태 시작(피격충돌체off, 마우스충돌체on)
	void UpdateDeathState(float _DeltaTime);											// 사망중상태 갱신
	void EndDeathState();																// 사망중상태 종료

	void StartDeadState();																// 사망(리젠대기)상태 시작
	void UpdateDeadState(float _DeltaTime);												// 사망(리젠대기)상태 갱신
	void EndDeadState();																// 사망(리젠대기)상태 종료

	void StartStunState();																// 스턴상태 시작
	void UpdateStunState(float _DeltaTime);												// 스턴상태 갱신
	void EndStunState();																// 스턴상태 종료

	void StartKnockbackState();															// 넉백상태 시작
	void UpdateKnockbackState(float _DeltaTime);										// 넉백상태 갱신
	void EndKnockbackState();															// 넉백상태 종료

	void StartWallSlamState();															// 벽스턴상태 시작
	void UpdateWallSlamState(float _DeltaTime);											// 벽스턴상태 갱신
	void EndWallSlamState();															// 벽스턴상태 종료

	//==================================== Attack State(조건 : 타겟지정이 되어있을때)
	void StartAttack01State();															// 일반공격1상태 시작
	void UpdateAttack01State(float _DeltaTime);											// 일반공격1상태 갱신
	void EndAttack01State();															// 일반공격1상태 종료

	void StartAttack02State();															// 일반공격2상태 시작
	void UpdateAttack02State(float _DeltaTime);											// 일반공격2상태 갱신
	void EndAttack02State();															// 일반공격2상태 종료

	void StartSkillAttackState();														// 스킬공격상태 시작
	void UpdateSkillAttackState(float _DeltaTime);										// 스킬공격상태 갱신
	void EndSkillAttackState();															// 스킬공격상태 종료

private: // Private DebugWindow Update Function
	void DebugWindowUpdate();															// 몬스터 기본정보 표시용(IMGUI)

private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	Monsters();
	virtual ~Monsters() = 0;															// 해당 클래스는 모든 몬스터의 상위(부모) 클래스로 
																						// 반드시 상속받아서 객체화해야하기때문에 순수가상소멸자를 사용

protected:
	Monsters(const Monsters& _other) = delete;
	Monsters(Monsters&& _other) = delete;

private:
	Monsters& operator=(const Monsters& _other) = delete;
	Monsters& operator=(const Monsters&& _other) = delete;

public:
protected: // 기본 컴포넌트
	GameEngineFBXRenderer* MainRenderer_;												// 메인 렌더러
	GameEngineFBXRenderer* EffectRenderer_;												// 이펙트 렌더러
	GameEngineCollision* BodyCollider_;													// 몸체 충돌체(메인 -> 피격충돌체)
	GameEngineCollision* ItemCollider_;													// 사망시 드랍아이템목록표시충돌체
	GameEngineCollision* AtkCollider_;													// 일반공격 충돌체(서브 -> 일반공격충돌체)
	GameEngineCollision* SkillAtkReadyCollider_;										// 스킬공격준비 충돌체(서브 -> 스킬공격감지(준비))
	GameEngineCollision* SkillAtkCollider_;												// 스킬공격 충돌체(서브 -> 스킬공격충돌체)

protected: // 선처리 컴포넌트
	GameEnginePreprocessingRenderer* MainOutLineRenderer_;								// 메인렌더러의 선처리 렌더러(외곽선) : 메인렌더러가 Off 상태라면 외곽선렌더러 Off상태
	GameEnginePreprocessingRenderer* MainSilhouetteRenderer_;							// 메인렌더러의 선처리 렌더러(실루엣) : 메인렌더러가 Off 상태라면 실루엣렌더러 Off상태

protected: // 생성정보(탐색용)
	int Index_;																			// 생성 인덱스
	MonsterType Type_;																	// 몬스터의 타입
	Location AreaType_;																	// 최초 스폰 지역 타입(StateInfo_.AppearAreaList_ 중 랜덤으로 선택된 지역)

protected: // 맵 및 길찾기
	NavFace* CurrentNavFace_;															// 현재 면정보
	NavMesh* CurrentNavMesh_;															// 네비메쉬
	LumiaMap* CurrentMap_;																// 현재 몬스터가 배치된 맵
	
protected: // 상태정보(갱신용)
	MonsterStateInfo StateInfo_;														// 몬스터의 상태정보
	float4 MoveDir_;																	// 현재 이동방향
	float4 MoveTarget_;																	// 이동목적지
	std::vector<float4> MovePath_;														// 현재 이동경로

protected: // FSM State
	GameEngineFSM MainState_;															// 메인상태(기본: NormalState 실행)
	GameEngineFSM NormalState_;															// 기본상태(CC, ATK 후 돌아오는 상태)
	GameEngineFSM CrowdControlState_;													// 군중제어상태(외부 Callback에의해 실행되는 상태)
	GameEngineFSM AttackState_;															// 공격상태(일반공격, 스킬공격 상태를 관리 및 실행)
	
protected: // 타겟
	Character* CurTarget_;																// 현재 몬스터의 타겟포인터(피격시 타겟지정, 귀환상태(귀소본능 수치가 0.0f일때)에서 초기화)
	int CurTargetIndex_;																// 현재 몬스터의 타겟인덱스(피격시 타겟지정, 귀환상태(귀소본능 수치가 0.0f일때)에서 초기화)

protected: // 상태정보
	MonsterStateBasicType CurStateBasicType_;											// 몬스터의 현재상태 기본타입(Normal, CrowdControl, Attack)
	MonsterStateType PrevStateType_;													// 몬스터의 현재상태의 바로직전의 상태
	MonsterStateType CurStateType_;														// 몬스터의 현재상태 상세타입(Run, Idle, Hit, ...)

protected: // 기타
	bool IsDeath_;																		// 몬스터사망시 피격판정무시 Flag(사망시 On)
	bool IsAttack_;																		// ATK01, ATK02, SKILLATTACK 상태시 활성화
	bool bHidden_;

protected: // CC Value
	float StunTime_;																	// 스턴시간
	float KnockbackTime_;																// 넉백시간
	float4 KnockbackSpeed_;																// 넉백속도(넉백방향 * 넉백속도)

private:
	ItemBox* itemBox_;																	// 몬스터사망시 표시되는 아이템 박스
};

