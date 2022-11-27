#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "Character.h"

void Monsters::StartAppearState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::APPEAR;

	// ���� ������ �°� ���� ������
	StateInfo_.HP_ = StateInfo_.HPMax_;										// ���� �ִ�ü������ �ʱ�ȭ
	StateInfo_.HomingInstinctValue_ = StateInfo_.HomingInstinctValueMax_;	// �ִ� �ͼҺ��ɼ�ġ�� �ʱ�ȭ
	StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;						// �ִ� �����ð����� �ʱ�ȭ
}

void Monsters::UpdateAppearState(float _DeltaTime)
{
	// ù����ִϸ��̼� �������� �����·� ����
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
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::REGEN;

	// ���� �ʱ������� �ʱ�ȭ
	StateInfo_.Level_ = StateInfo_.LevelMin_;								// ���۷����� �ʱ�ȭ
	StateInfo_.HP_ = StateInfo_.HPMax_;										// ���� �ִ�ü������ �ʱ�ȭ
	StateInfo_.HomingInstinctValue_ = StateInfo_.HomingInstinctValueMax_;	// �ִ� �ͼҺ��ɼ�ġ�� �ʱ�ȭ
	StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;						// �ִ� �����ð����� �ʱ�ȭ

	// �ʱ⽺����ġ�� ���� �� ������� ���� �غ�
	GetTransform()->SetWorldPosition(StateInfo_.NestPosition_);
	MainRenderer_->On();

	// ��� Flag Off
	IsDeath_ = false;
}

void Monsters::UpdateRegenState(float _DeltaTime)
{
	// �����Ϸ�� ������ ����
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
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::IDLE;
}

void Monsters::UpdateIdleState(float _DeltaTime)
{
	// Ÿ������ ���� �Ǵ�
	if (nullptr != CurTarget_)
	{
		// ���� Ÿ���� ����ߴٸ�
		if (true == CurTarget_->IsDead())
		{
			// ��� ������ ��ȯ�ϱ����� �ͼҺ��ɼ�ġ�� 0.0f�� �ʱ�ȭ�ϰ� ����
			StateInfo_.HomingInstinctValue_ = 0.0f;
			return;
		}

		// Ÿ���� �����Ҷ� Ÿ�ٰ� ���� �Ÿ� ����
		float4 TargetPosition = CurTarget_->GetTransform()->GetWorldPosition();
		float4 MyPosition = transform_.GetWorldPosition();
		float Dist = (TargetPosition - MyPosition).Len3D();

		// �Ÿ��� ���ݹ������� �϶� ���ݻ��·� ����
		if (Dist <= StateInfo_.AttackRange_)
		{
			// ����ó��
			AttackProcessing();
		}
		// �Ÿ��� ���ݹ����� ����ִٸ� �������·� ����
		else
		{
			// ����������ȯ
			ChangeAnimationAndState(MonsterStateType::CHASE);
		}
	}

	// Ÿ���������� ���

}

void Monsters::EndIdleState()
{
}

void Monsters::StartChaseState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::CHASE;

	// �̵�����
	StartMove(CurTarget_->GetTransform()->GetWorldPosition());
}

void Monsters::UpdateChaseState(float _DeltaTime)
{
	// �̵�����
	UpdateMove(_DeltaTime);
}

void Monsters::EndChaseState()
{
}

void Monsters::StartHomingInstinctState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::HOMINGINSTINCT;

	// ����ó��
	if (nullptr != CurTarget_)
	{
		CurTarget_ = nullptr;
		CurTargetIndex_ = -1;
	}

	// ��ȯ�� �ǰݹ���
	IsDeath_ = true;

	// �̵�����
	StartMove(StateInfo_.NestPosition_);
}

void Monsters::UpdateHomingInstinctState(float _DeltaTime)
{
	UpdateMove(_DeltaTime);
}

void Monsters::EndHomingInstinctState()
{
	// ��ȯ�Ϸ�� �ǰݻ��� üũ�� ���� �浹üũ On
	IsDeath_ = false;
}
