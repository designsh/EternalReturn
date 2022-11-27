#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "Character.h"

void Monsters::StartAppearState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::APPEAR;

	// 현재 레벨에 맞게 스텟 재조정
	StateInfo_.HP_ = StateInfo_.HPMax_;										// 현재 최대체력으로 초기화
	StateInfo_.HomingInstinctValue_ = StateInfo_.HomingInstinctValueMax_;	// 최대 귀소본능수치로 초기화
	StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;						// 최대 리젠시간으로 초기화
}

void Monsters::UpdateAppearState(float _DeltaTime)
{
	// 첫등장애니메이션 모션종료시 대기상태로 돌입
	if ("APPEAR" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndAppearState()
{
}

void Monsters::StartRegenState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::REGEN;

	// 스텟 초기정보로 초기화
	StateInfo_.Level_ = StateInfo_.LevelMin_;								// 시작레벨로 초기화
	StateInfo_.HP_ = StateInfo_.HPMax_;										// 현재 최대체력으로 초기화
	StateInfo_.HomingInstinctValue_ = StateInfo_.HomingInstinctValueMax_;	// 최대 귀소본능수치로 초기화
	StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;						// 최대 리젠시간으로 초기화

	// 초기스폰위치로 셋팅 후 재등장을 위한 준비
	GetTransform()->SetWorldPosition(StateInfo_.NestPosition_);
	MainRenderer_->On();

	// 사망 Flag Off
	IsDeath_ = false;
}

void Monsters::UpdateRegenState(float _DeltaTime)
{
	// 리젠완료시 대기상태 돌입
	if ("REGEN" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndRegenState()
{
}

void Monsters::StartIdleState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::IDLE;
}

void Monsters::UpdateIdleState(float _DeltaTime)
{
	// 타겟지정 유무 판단
	if (nullptr != CurTarget_)
	{
		// 현재 타겟이 사망했다면
		if (true == CurTarget_->IsDead())
		{
			// 즉시 둥지로 귀환하기위해 귀소본능수치를 0.0f로 초기화하고 리턴
			StateInfo_.HomingInstinctValue_ = 0.0f;
			return;
		}

		// 타겟이 존재할때 타겟과 나의 거리 측정
		float4 TargetPosition = CurTarget_->GetTransform()->GetWorldPosition();
		float4 MyPosition = transform_.GetWorldPosition();
		float Dist = (TargetPosition - MyPosition).Len3D();

		// 거리가 공격범위내에 일때 공격상태로 변경
		if (Dist <= StateInfo_.AttackRange_)
		{
			// 공격처리
			AttackProcessing();
		}
		// 거리가 공격범위를 벗어나있다면 추적상태로 변경
		else
		{
			// 추적상태전환
			ChangeAnimationAndState(MonsterStateType::CHASE);
		}
	}

	// 타겟지정까지 대기

}

void Monsters::EndIdleState()
{
}

void Monsters::StartChaseState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::CHASE;

	// 이동시작
	StartMove(CurTarget_->GetTransform()->GetWorldPosition());
}

void Monsters::UpdateChaseState(float _DeltaTime)
{
	// 이동갱신
	UpdateMove(_DeltaTime);
}

void Monsters::EndChaseState()
{
}

void Monsters::StartHomingInstinctState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::HOMINGINSTINCT;

	// 예외처리
	if (nullptr != CurTarget_)
	{
		CurTarget_ = nullptr;
		CurTargetIndex_ = -1;
	}

	// 귀환중 피격무시
	IsDeath_ = true;

	// 이동시작
	StartMove(StateInfo_.NestPosition_);
}

void Monsters::UpdateHomingInstinctState(float _DeltaTime)
{
	UpdateMove(_DeltaTime);
}

void Monsters::EndHomingInstinctState()
{
	// 귀환완료시 피격상태 체크를 위해 충돌체크 On
	IsDeath_ = false;
}
