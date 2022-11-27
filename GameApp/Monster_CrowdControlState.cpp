#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "MonsterStateChangePacket.h"

#include "LumiaLevel.h"
#include "LumiaMap.h"
#include "NavMesh.h"
#include "ItemBox.h"
#include "UI_ItemBox.h"
#include "PlayerInfoManager.h"

void Monsters::StartHitState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::HIT;
}

void Monsters::UpdateHitState(float _DeltaTime)
{
	// 모션 종료시(피격모션이 따로 없으므로 바로 대기상태로 전환
	ChangeAnimationAndState(MonsterStateType::IDLE);
}

void Monsters::EndHitState()
{
}

void Monsters::StartDeathState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::DEATH;

	// 사망시작시 타겟제거
	CurTarget_ = nullptr;
	CurTargetIndex_ = -1;

	// 사망 Flag On
	IsDeath_ = true;
}

void Monsters::UpdateDeathState(float _DeltaTime)
{
	// 현재 애니메이션이 종료되면 리젠대기(사망)상태로 전환
	if ("DEATH" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		// 서버라면 상태 전환 후 패킷전송
		// 클라라면 패킷전송 후 패킷수신시 상태전환
		if (true == GameServer::GetInstance()->IsOpened())
		{
			ChangeAnimationAndState(MonsterStateType::DEAD);
		}
		
		// 완전사망상태전환 패킷 전송
		MonsterStateChangePacket ChangePacket;
		ChangePacket.SetIndex(Index_);
		ChangePacket.SetMonsterType(Type_);
		ChangePacket.SetMonsterStateType(MonsterStateType::DEAD);
		ChangePacket.SetMonsterStatInfo(StateInfo_);
		ChangePacket.SetTargetIndex(-1);
		if (true == GameServer::GetInstance()->IsOpened())
		{
			GameServer::GetInstance()->Send(&ChangePacket);
		}
		else if (true == GameClient::GetInstance()->IsConnected())
		{
			GameClient::GetInstance()->Send(&ChangePacket);
		}
	}
}

void Monsters::EndDeathState()
{
}

void Monsters::StartDeadState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::DEAD;
	IsDeath_ = true;
	BodyCollider_->Off();
}

void Monsters::UpdateDeadState(float _DeltaTime)
{
	// 해당 몬스터가 사망하였으므로 리젠대기시간체크
	StateInfo_.RegenTime_ -= _DeltaTime;
	if (0.0f >= StateInfo_.RegenTime_)
	{
		// 리젠시간 초기화
		StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;

		// 서버라면 바로 리젠상태로 전환 후 패킷 전송
		// 클라라면 패킷전송 후 패킷수신시 상태전환
		if (true == GameServer::GetInstance()->IsOpened())
		{
			ChangeAnimationAndState(MonsterStateType::REGEN);
		}

		// 리젠상태전환 패킷 전송
		MonsterStateChangePacket ChangePacket;
		ChangePacket.SetIndex(Index_);
		ChangePacket.SetMonsterType(Type_);
		ChangePacket.SetMonsterStateType(MonsterStateType::REGEN);
		ChangePacket.SetMonsterStatInfo(StateInfo_);
		ChangePacket.SetTargetIndex(-1);
		if (true == GameServer::GetInstance()->IsOpened())
		{
			GameServer::GetInstance()->Send(&ChangePacket);
		}
		else if (true == GameClient::GetInstance()->IsConnected())
		{
			GameClient::GetInstance()->Send(&ChangePacket);
		}
	}
	
	//============================================ 아이템박스 처리
	if (nullptr != ItemCollider_)
	{
//#ifdef _DEBUG
		GetLevel()->PushDebugRender(ItemCollider_->GetTransform(), CollisionType::OBBBox3D, float4::BLACK);
//#endif // _DEBUG

		Character* player = PlayerInfoManager::GetInstance()->GetMainCharacter();

		if (nullptr == player)
		{
			return;
		}

		GameEngineCollision* playerCol = player->GetCollision();

		if (false == ItemCollider_->Collision(playerCol))
		{
			itemBox_->Close();
			return;
		}

		if (true == ItemCollider_->Collision(static_cast<int>(eCollisionGroup::MouseRay)) && true == GameEngineInput::GetInst().Down("RButton"))
		{
			itemBox_->Open();
		}

		if (nullptr == itemBox_->GetItemBoxUI())
		{
			return;
		}

		int slotIndex = itemBox_->GetItemBoxUI()->SlotMouseCollisionCheck();

		if (-1 == slotIndex)
		{
			return;
		}

		if (true == GameEngineInput::GetInst().Down("LButton"))
		{
			GiveItemToPlayer(slotIndex);
		}
	}
}

void Monsters::EndDeadState()
{
	// MainRenderer Off
	// -> 잠시 Off
	MainRenderer_->Off();
	IsDeath_ = false;

	if (itemBox_->isOpen())
	{
		itemBox_->Close();
	}
	ItemCollider_->Off();
	BodyCollider_->On();
}

void Monsters::StartStunState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::STUN;
}

void Monsters::UpdateStunState(float _DeltaTime)
{
	StunTime_ -= _DeltaTime;
	if (0.0f >= StunTime_)
	{
		// 상태전환
		ChangeAnimationAndState(MonsterStateType::IDLE);

		// 스턴시간 초기화
		StunTime_ = 0.0f;
	}
}

void Monsters::EndStunState()
{
}

void Monsters::StartKnockbackState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::KNOCKBACK;
}

void Monsters::UpdateKnockbackState(float _DeltaTime)
{
	KnockbackTime_ -= _DeltaTime;
	if (0.0f >= KnockbackTime_)
	{
		// 상태전환
		ChangeAnimationAndState(MonsterStateType::IDLE);
		
		// 넉백정보 초기화
		KnockbackTime_ = 0.0f;
		KnockbackSpeed_ = float4::ZERO;

		return;
	}

	transform_.AddWorldPosition(KnockbackSpeed_ * _DeltaTime);
}

void Monsters::EndKnockbackState()
{
}

void Monsters::StartWallSlamState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::WALLSLAM;
}

void Monsters::UpdateWallSlamState(float _DeltaTime)
{
	KnockbackTime_ -= _DeltaTime;
	if (0.0f >= KnockbackTime_)
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);

		// 벽스턴관련 정보 초기화
		KnockbackTime_ = 0.0f;
		KnockbackSpeed_ = float4::ZERO;

		return;
	}

	float4 moveSpeed = KnockbackSpeed_ * _DeltaTime;
	float4 nextMovePosition = transform_.GetWorldPosition() + moveSpeed;

	float temp = 0.0f;
	if (true == CurrentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		// 스턴상태 전환
		Stun(StunTime_);
	}
}

void Monsters::EndWallSlamState()
{
}
