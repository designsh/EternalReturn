#include "Precompile.h"
#include "Monsters.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "MonsterInfoManager.h"
#include "GameServer.h"
#include "GameClient.h"
#include "MonsterStateChangePacket.h"
#include "MonsterDamagePacket.h"
#include "MonsterCrowdControlPacket.h"

#include "LumiaLevel.h"
#include "LumiaMap.h"
#include "NavMesh.h"

#include "Character.h"
#include "ItemBox.h"
#include "PlayerInfoManager.h"
#include "PlayerUIController.h"
#include "UI_ItemBox.h"

void Monsters::GiveItemToPlayer(int _index)
{
	ItemBase* Item = itemBox_->GetItem(_index);

	if (nullptr == Item)
	{
		return;
	}

	std::list<ItemBase*> itemlist = GetLevelConvert<LumiaLevel>()->GetItemBoxManager()->GetAllItemList();

	Character* player = PlayerInfoManager::GetInstance()->GetMainCharacter();

	int Count = 0;

	for (auto& invenItem : player->GetInventory())
	{
		if (nullptr != invenItem)
		{
			++Count;
			continue;
		}

		player->SetInventoryItem(Item, Count);

		player->GetUIController()->GetInventoryUI()->EmptySlot();
		player->GetUIController()->GetInventoryUI()->GetInventoryInfo(player->GetInventory());

		itemBox_->DeleteItem(_index);

		itemBox_->GetItemBoxUI()->EmptySlot();
		itemBox_->GetItemBoxUI()->GetItemBoxInfo(itemBox_);

		std::list<ItemBase*>::iterator iter = player->GetAllMyBuildItems().begin();

		for (; iter != player->GetAllMyBuildItems().end(); ++iter)
		{
			if ((*iter)->GetName() != Item->GetName())
			{
				continue;
			}

			break;
		}

		if (iter != player->GetAllMyBuildItems().end())
		{
			player->GetAllMyBuildItems().erase(iter);
		}

		break;
	}

	player->checkItemRecipes();
}

void Monsters::LevelUP(LevelUPData _Data)
{
	StateInfo_.Level_ += 1;
	StateInfo_.OffencePower_ += static_cast<int>(_Data.AttackPower_);	// ���ݷ�

	// ���� ������ ����ü���� ���� �ִ�ü�°� ���ٸ� �ѹ��� �ǰݴ��������� ���ͷ� �Ǵ��Ͽ�
	// �ű� �ִ�ü������ ���� ü���� �ʱ�ȭ
	if (StateInfo_.HP_ == StateInfo_.HPMax_)
	{
		StateInfo_.HPMax_ += _Data.HP_;									// ü��(�ִ�ü��)
		StateInfo_.HP_ = StateInfo_.HPMax_;
	}
	// �ƴ϶�� �ѹ��̶� �ǰݴ������Ƿ� �ִ�ü�¸� ����
	else
	{
		StateInfo_.HPMax_ += _Data.HP_;									// ü��(�ִ�ü��)
	}

	StateInfo_.Defense_ += static_cast<int>(_Data.Defence_);			// ����
	StateInfo_.MoveSpeed_ += _Data.MovementSpeed_;						// �̵��ӵ�

	// �ش� ������ ����ü���� �ִ�
}

void Monsters::PickingOutLineOn()
{
	if (nullptr != MainOutLineRenderer_)
	{
		MainOutLineRenderer_->On();
	}
}

void Monsters::PickingOutLineOff()
{
	if (nullptr != MainOutLineRenderer_)
	{
		MainOutLineRenderer_->Off();
	}
}

int Monsters::GetIndex()
{
	return Index_;
}

void Monsters::Damage(float _Amount, IUnit* _Target)
{
	// ����ó��
	if (0.0f >= _Amount)
	{
		GameEngineDebug::OutPutDebugString("�ش� ���͸� �����ϴ� �������� �߸��� ������ �����Ǿ����ϴ�!!!!\n");
		GameEngineDebug::OutPutDebugString("���� ������: " + std::to_string(_Amount) + "\n");
		return;
	}

	// ����ó��
	if (nullptr == _Target)
	{
		GameEngineDebug::OutPutDebugString("�ش� ������ Ÿ���� �����Ҽ������ϴ�!!!!\n");
		return;
	}

	// ����ó��
	if (true == IsDeath_)
	{
		GameEngineDebug::OutPutDebugString("�ش� ���ʹ� �̹� ������·� ���ݴ���� �ƴմϴ�!!!!\n");
		return;
	}

	// ����ó��
	if (UnitType::MONSTER == _Target->UnitType_)
	{
		GameEngineDebug::OutPutDebugString("���� ���Ͱ� ���� ���������Ƿ� �����մϴ�!!!!!!\n");
		return;
	}

	// ���� API�� ������ ü�°���
	StateInfo_.HP_ -= _Amount;
	if (0.0f >= StateInfo_.HP_)
	{
		// 0.0f ���ϸ� ���ó��
		StateInfo_.HP_ = 0.0f;

		// ����߻��·� ��ȯ
		// ��, �����̸� ������ȯ �� ��Ŷ �۽�
		//     Ŭ���̸� ��Ŷ �۽� �� �����κ��� ��Ŷ���Ž� ������ȯ
		if (true == GameServer::GetInstance()->IsOpened())
		{
			ChangeAnimationAndState(MonsterStateType::DEATH);
		}

		// ���� API�� ���� ������ȯ ��Ŷ����(����ȭó��)
		MonsterStateChangePacket ChangePacket;
		ChangePacket.SetIndex(Index_);
		ChangePacket.SetMonsterType(Type_);
		ChangePacket.SetMonsterStateType(MonsterStateType::DEATH);
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

		return;
	}

	// ���� API�� ���͸� �ǰ��� Ÿ���� ����
	// ��, �̹� Ÿ���� �����Ǿ��ִٸ� Ÿ�ٺ������ ó��
	if (nullptr == CurTarget_)
	{
		Character* CurTarget = dynamic_cast<Character*>(_Target);
		CurTarget_ = CurTarget;
		CurTargetIndex_ = CurTarget->GetIndex();
	}

	// 0.0f���ϰ� �ƴ϶�� �ܼ� �ǰ�ó��
	// ���� API�� ���͸� �ǰݻ��·� ��ȯ
	if (true == GameServer::GetInstance()->IsOpened())
	{
		ChangeAnimationAndState(MonsterStateType::HIT);
	}

	// ���� API�� ���� ������ȯ ��Ŷ����(����ȭó��)
	MonsterStateChangePacket ChangePacket;
	ChangePacket.SetIndex(Index_);
	ChangePacket.SetMonsterType(Type_);
	ChangePacket.SetMonsterStateType(MonsterStateType::HIT);
	ChangePacket.SetMonsterStatInfo(StateInfo_);
	ChangePacket.SetTargetIndex(CurTargetIndex_);
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->Send(&ChangePacket);
	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->Send(&ChangePacket);
	}
}

void Monsters::Show()
{
	if (nullptr != MainRenderer_)
	{
		MainRenderer_->On();
	}
	
	if (nullptr != EffectRenderer_)
	{
		EffectRenderer_->On();
	}
	bHidden_ = false;
}

void Monsters::Hide()
{
	if (nullptr != MainRenderer_)
	{
		MainRenderer_->Off();
	}

	if (nullptr != EffectRenderer_)
	{
		EffectRenderer_->Off();
	}
	bHidden_ = true;
}

void Monsters::rcvAttack01(MonsterStateInfo _rcvStatInfo)
{
	// �������� ����
	UpdateSpecialStat(_rcvStatInfo);

	// �Ϲݰ���01 ���� ��ȯ
	ChangeAnimationAndState(MonsterStateType::ATK01);
}

void Monsters::rcvAttack02(MonsterStateInfo _rcvStatInfo)
{
	// �������� ����
	UpdateSpecialStat(_rcvStatInfo);

	// �Ϲݰ���02 ���� ��ȯ
	ChangeAnimationAndState(MonsterStateType::ATK02);
}

void Monsters::rcvDamage(MonsterStateInfo _rcvStatInfo, int _TargetIndex)
{
	// �������� ����
	UpdateSpecialStat(_rcvStatInfo);

	if (-1 == _TargetIndex)
	{
		GameEngineDebug::MsgBoxError("�߸��� Ÿ���ε����� ���Ź޾ҽ��ϴ�!!!!");
		return;
	}

	// �̹� Ÿ���� �����ϴ��� ���ŵ� Ÿ������ ����
	LumiaLevel* CurLevel = GetLevelConvert<LumiaLevel>();
	if (nullptr == CurLevel)
	{
		GameEngineDebug::MsgBoxError("���� ������ ������ ���������ʽ��ϴ�!!!!");
		return;
	}
	CurTarget_ = CurLevel->GetSpecificCharacter(_TargetIndex);
	CurTargetIndex_ = CurTarget_->GetIndex();

	// �ǰݻ��� ��ȯ
	ChangeAnimationAndState(MonsterStateType::HIT);
}

void Monsters::rcvDeath(MonsterStateInfo _rcvStatInfo)
{
	// �̹̻� ����� or ������¶�� ����
	if (MonsterStateType::DEATH == CurStateType_)
	{
		return;
	}

	// �������� ����
	UpdateSpecialStat(_rcvStatInfo);

	// ����߻��·� ��ȯ
	ChangeAnimationAndState(MonsterStateType::DEATH);
}

void Monsters::rcvDead(MonsterStateInfo _rcvStatInfo)
{
	// �̹̻� ����� or ������¶�� ����
	if (MonsterStateType::DEAD == CurStateType_)
	{
		return;
	}

	// �������� ����
	UpdateSpecialStat(_rcvStatInfo);

	// ����߻��·� ��ȯ
	ChangeAnimationAndState(MonsterStateType::DEAD);
}

void Monsters::rcvRegen(MonsterStateInfo _rcvStatInfo)
{
	// ��, �̹� ���������̰ų� �����Ϸ��Ͽ� �������̸� ó������
	if (CurStateType_ == MonsterStateType::REGEN || CurStateType_ == MonsterStateType::IDLE)
	{
		return;
	}

	// �������� ����
	UpdateSpecialStat(_rcvStatInfo);

	// �������·� ��ȯ
	ChangeAnimationAndState(MonsterStateType::REGEN);
}

void Monsters::rcvHomingInstinct(MonsterStateInfo _rcvStatInfo)
{
	// ��, �̹� ��ȯ�߻����̰ų� ��ȯ�Ϸ��Ͽ� �������̸� ó������
	if (CurStateType_ == MonsterStateType::HOMINGINSTINCT || CurStateType_ == MonsterStateType::IDLE)
	{
		return;
	}

	// �������� ����
	UpdateSpecialStat(_rcvStatInfo);

	// �������·� ��ȯ
	ChangeAnimationAndState(MonsterStateType::HOMINGINSTINCT);
}

void Monsters::rcvDamage()
{
	// Ÿ�ٿ� �������� ����
	// ��, ���� Ÿ���� �����Ǿ��������� ����
	if (nullptr != CurTarget_)
	{
		CurTarget_->Damage(static_cast<float>(StateInfo_.OffencePower_), this);
	}
}

void Monsters::rcvStun(float _StunTime)
{
	// �̹� ������·� �Ѿ���ϴ� ��� ����
	if (0.0f >= StateInfo_.HP_)
	{
		return;
	}

	// �������� ����
	StunTime_ = _StunTime;

	// ���ϻ��� ��ȯ
	ChangeAnimationAndState(MonsterStateType::STUN);
}

void Monsters::rcvKnockback(float _KnockbackTime, float4 _KnockbackSpeed)
{
	// �̹� ������·� �Ѿ���ϴ� ��� ����
	if (0.0f >= StateInfo_.HP_)
	{
		return;
	}

	// �˹����� ����
	KnockbackTime_ = _KnockbackTime;
	KnockbackSpeed_ = _KnockbackSpeed;

	// �˹���� ��ȯ
	ChangeAnimationAndState(MonsterStateType::KNOCKBACK);
}

void Monsters::rcvWallSlam(float _KnockbackTime, float4 _KnockbackSpeed, float _StunTime)
{
	// �̹� ������·� �Ѿ���ϴ� ��� ����
	if (0.0f >= StateInfo_.HP_)
	{
		return;
	}

	// �˹����� ����
	KnockbackTime_ = _KnockbackTime;
	KnockbackSpeed_ = _KnockbackSpeed;
	StunTime_ = _StunTime;

	// �˹���� ��ȯ
	ChangeAnimationAndState(MonsterStateType::WALLSLAM);
}

void Monsters::InitalizeSpawnPosition(const float4& _SpawnPosition)
{
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	// ���� ���Ͱ� ��ġ�� �� ����
	CurrentMap_ = GetLevelConvert<LumiaLevel>()->GetMap();
	if (nullptr == CurrentMap_)
	{
		GameEngineDebug::MsgBoxError("���緹���� ������ ���� ���������ʽ��ϴ�!!!!!");
		return;
	}

	// ������ ��ġ ����
	float4 SpawnPos = _SpawnPosition;
	SpawnPos *= CurrentMap_->GetNavMesh()->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;
	if (true == mm->GetAllMonsterListRef()[Index_].IsGroup_)
	{
		if (0 != Index_)
		{
			SpawnPos = mm->GetAllMonsterListRef()[Index_ - 1].SpawnPosition_;
			SpawnPos *= CurrentMap_->GetNavMesh()->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;
			SpawnPos += float4(100.f, 0.f, 10.f);
		}
	}
	GetTransform()->SetLocalPosition(SpawnPos);

	// �����̵������� = ������ġ
	MoveTarget_ = SpawnPos;

	// �ش� ���� ������ġ == ���� ������ġ
	StateInfo_.NestPosition_ = SpawnPos;
}

void Monsters::ChangeAnimationAndState(MonsterStateType _StateType)
{
	// ���� ����Ÿ������ �ִϸ��̼� ��ȯ �� ������ȯ
	switch (_StateType)
	{
		case MonsterStateType::APPEAR:
		{
			NormalState_ << "APPEAR";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("APPEAR", true);
			break;
		}
		case MonsterStateType::REGEN:
		{
			NormalState_ << "REGEN";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("REGEN", true);
			break;
		}
		case MonsterStateType::IDLE:
		{
			NormalState_ << "IDLE";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("IDLE");
			break;
		}
		case MonsterStateType::HOMINGINSTINCT:
		{
			NormalState_ << "HOMINGINSTINCT";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("HOMINGINSTINCT");
			break;
		}
		case MonsterStateType::CHASE:
		{
			NormalState_ << "CHASE";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("CHASE");
			break;
		}
		case MonsterStateType::HIT:
		{
			CrowdControlState_ << "HIT";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("HIT", true);
			break;
		}
		case MonsterStateType::DEATH:
		{
			CrowdControlState_ << "DEATH";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("DEATH", true);
			break;
		}
		case MonsterStateType::DEAD:
		{
			CrowdControlState_ << "DEAD";
			MainState_ << "CROWDCONTROL";
			break;
		}
		case MonsterStateType::STUN:
		{
			CrowdControlState_ << "STUN";
			MainState_ << "CROWDCONTROL";
			break;
		}
		case MonsterStateType::KNOCKBACK:
		{
			CrowdControlState_ << "KNOCKBACK";
			MainState_ << "CROWDCONTROL";
			break;
		}
		case MonsterStateType::WALLSLAM:
		{
			CrowdControlState_ << "WALLSLAM";
			MainState_ << "CROWDCONTROL";
			break;
		}
		case MonsterStateType::ATK01:
		{
			AttackState_ << "ATK01";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("ATK01", true);
			break;
		}
		case MonsterStateType::ATK02:
		{
			AttackState_ << "ATK02";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("ATK02", true);
			break;
		}
		case MonsterStateType::SKILLATTACK:
		{
			AttackState_ << "SKILLATTACK";
			MainState_ << "ATTACK";
			break;
		}
		case MonsterStateType::MAX:
		case MonsterStateType::NONE:
			GameEngineDebug::MsgBox("�߸��� ����Ÿ���� �����Ͽ� �ִϸ��̼� �� ���� ������ �Ұ��մϴ�!!!");
			return;
	}
}

void Monsters::UpdateSpecialStat(MonsterStateInfo _UpdateStat)
{
	// ���ſ� ���ݸ��� �����Ͽ� ����
	StateInfo_.Level_ = _UpdateStat.Level_;
	StateInfo_.OffencePower_ = _UpdateStat.OffencePower_;
	StateInfo_.Defense_ = _UpdateStat.Defense_;
	StateInfo_.HPMax_ = _UpdateStat.HPMax_;
	StateInfo_.HP_ = _UpdateStat.HP_;
	StateInfo_.MoveSpeed_ = _UpdateStat.MoveSpeed_;
	StateInfo_.HomingInstinctValue_ = _UpdateStat.HomingInstinctValue_;
	StateInfo_.RegenTime_ = _UpdateStat.RegenTime_;
	StateInfo_.SkillCoolDown_ = _UpdateStat.SkillCoolDown_;
}

void Monsters::UpdateAllStat(MonsterStateInfo _UpdateStat)
{
	// ��ü ������ �����Ͽ� ����
	StateInfo_ = _UpdateStat;
}

void Monsters::Stun(float _StunTime)
{
	// �̹� ������·� �Ѿ���ϴ� ��� ����
	if (0.0f >= StateInfo_.HP_)
	{
		return;
	}

	// �������� ����
	StunTime_ = _StunTime;

	// ���ϻ��� ��ȯ
	ChangeAnimationAndState(MonsterStateType::STUN);

	// ��Ŷ����
	MonsterCrowdControlPacket Packet;
	Packet.SetStun(Index_, StunTime_);
	FT::SendPacket(Packet);
}

void Monsters::Knockback(float _KnockbackTime, float4 _KnockbackSpeed)
{
	// �̹� ������·� �Ѿ���ϴ� ��� ����
	if (0.0f >= StateInfo_.HP_)
	{
		return;
	}

	// �˹����� ����
	KnockbackTime_ = _KnockbackTime;
	KnockbackSpeed_ = _KnockbackSpeed;

	// �˹���� ��ȯ
	ChangeAnimationAndState(MonsterStateType::KNOCKBACK);

	// ��Ŷ����
	MonsterCrowdControlPacket Packet;
	Packet.SetKnockback(Index_, KnockbackTime_, KnockbackSpeed_);
	FT::SendPacket(Packet);
}

void Monsters::WallSlam(float _KnockbackTime, float4 _KnockbackSpeed, float _StunTime)
{
	// �̹� ������·� �Ѿ���ϴ� ��� ����
	if (0.0f >= StateInfo_.HP_)
	{
		return;
	}

	// �˹����� ����
	KnockbackTime_ = _KnockbackTime;
	KnockbackSpeed_ = _KnockbackSpeed;
	StunTime_ = _StunTime;

	// �˹���� ��ȯ
	ChangeAnimationAndState(MonsterStateType::WALLSLAM);

	// ��Ŷ����
	MonsterCrowdControlPacket Packet;
	Packet.SetWallSlam(Index_, KnockbackTime_, KnockbackSpeed_, StunTime_);
	FT::SendPacket(Packet);
}

void Monsters::Slow(float _slowTime, float _slowRatio)
{
}

void Monsters::InitalizeFSMState()
{
#pragma region MainState
	MainState_.CreateState("NORMAL", std::bind(&Monsters::UpdateNormalState, this, std::placeholders::_1), std::bind(&Monsters::StartNormalState, this), std::bind(&Monsters::EndNormalState, this));
	MainState_.CreateState("CROWDCONTROL", std::bind(&Monsters::UpdateCrowdControlState, this, std::placeholders::_1), std::bind(&Monsters::StartCrowdControlState, this), std::bind(&Monsters::EndCrowdControlState, this));
	MainState_.CreateState("ATTACK", std::bind(&Monsters::UpdateAttackState, this, std::placeholders::_1), std::bind(&Monsters::StartAttackState, this), std::bind(&Monsters::EndAttackState, this));
	MainState_.ChangeState("NORMAL");
#pragma endregion

#pragma region NormalState
	// ù����, ����, ���, ����, ��ȭ
	NormalState_.CreateState("APPEAR", std::bind(&Monsters::UpdateAppearState, this, std::placeholders::_1), std::bind(&Monsters::StartAppearState, this), std::bind(&Monsters::EndAppearState, this));
	NormalState_.CreateState("REGEN", std::bind(&Monsters::UpdateRegenState, this, std::placeholders::_1), std::bind(&Monsters::StartRegenState, this), std::bind(&Monsters::EndRegenState, this));
	NormalState_.CreateState("IDLE", std::bind(&Monsters::UpdateIdleState, this, std::placeholders::_1), std::bind(&Monsters::StartIdleState, this), std::bind(&Monsters::EndIdleState, this));
	NormalState_.CreateState("CHASE", std::bind(&Monsters::UpdateChaseState, this, std::placeholders::_1), std::bind(&Monsters::StartChaseState, this), std::bind(&Monsters::EndChaseState, this));
	NormalState_.CreateState("HOMINGINSTINCT", std::bind(&Monsters::UpdateHomingInstinctState, this, std::placeholders::_1), std::bind(&Monsters::StartHomingInstinctState, this), std::bind(&Monsters::EndHomingInstinctState, this));

	NormalState_.ChangeState("APPEAR");
#pragma endregion

#pragma region CrowdControlState
	// �ǰ�, �����, �������, ����, �˹�, ������
	CrowdControlState_.CreateState("HIT", std::bind(&Monsters::UpdateHitState, this, std::placeholders::_1), std::bind(&Monsters::StartHitState, this), std::bind(&Monsters::EndHitState, this));
	CrowdControlState_.CreateState("DEATH", std::bind(&Monsters::UpdateDeathState, this, std::placeholders::_1), std::bind(&Monsters::StartDeathState, this), std::bind(&Monsters::EndDeathState, this));
	CrowdControlState_.CreateState("DEAD", std::bind(&Monsters::UpdateDeadState, this, std::placeholders::_1), std::bind(&Monsters::StartDeadState, this), std::bind(&Monsters::EndDeadState, this));
	CrowdControlState_.CreateState("STUN", std::bind(&Monsters::UpdateStunState, this, std::placeholders::_1), std::bind(&Monsters::StartStunState, this), std::bind(&Monsters::EndStunState, this));
	CrowdControlState_.CreateState("KNOCKBACK", std::bind(&Monsters::UpdateKnockbackState, this, std::placeholders::_1), std::bind(&Monsters::StartKnockbackState, this), std::bind(&Monsters::EndKnockbackState, this));
	CrowdControlState_.CreateState("WALLSLAM", std::bind(&Monsters::UpdateWallSlamState, this, std::placeholders::_1), std::bind(&Monsters::StartWallSlamState, this), std::bind(&Monsters::EndWallSlamState, this));

	CrowdControlState_.ChangeState("HIT");
#pragma endregion

#pragma region AttackState
	// ����1, ����2, ��ų(��������)
	AttackState_.CreateState("ATK01", std::bind(&Monsters::UpdateAttack01State, this, std::placeholders::_1), std::bind(&Monsters::StartAttack01State, this), std::bind(&Monsters::EndAttack01State, this));
	AttackState_.CreateState("ATK02", std::bind(&Monsters::UpdateAttack02State, this, std::placeholders::_1), std::bind(&Monsters::StartAttack02State, this), std::bind(&Monsters::EndAttack02State, this));
	AttackState_.CreateState("SKILLATTACK", std::bind(&Monsters::UpdateSkillAttackState, this, std::placeholders::_1), std::bind(&Monsters::StartSkillAttackState, this), std::bind(&Monsters::EndSkillAttackState, this));

	AttackState_.ChangeState("ATK01");
#pragma endregion
}

void Monsters::CheckMonsterStateInfo(float _DeltaTime)
{
	// ���������� �̿��Ͽ� ������ȯ�ؾ��ϴ� ��츦 �Ʒ��� ���� ó��

	// 1. �ͼҺ��ɼ�ġ ����
	HomingInstinctValueUpdate(_DeltaTime);

	// 2. ��ų��Ÿ�� ����
	UpdateSkillCoolDown(_DeltaTime);

	// 3. ...

}

void Monsters::CheckAllCollision(float _DeltaTime)
{
	// ��ü �浹ü �浹üũ
	CheckBodyCollision(_DeltaTime);

	// �Ϲݰ��� �浹ü �浹üũ
	CheckAttackCollision(_DeltaTime);

	// ��ų���� �浹ü �浹üũ
	CheckSkillAttackCollision(_DeltaTime);
}

void Monsters::CheckBodyCollision(float _DeltaTime)
{
	// ��ü �浹ü �浹üũ
	if (nullptr != BodyCollider_ && true == BodyCollider_->IsUpdate())
	{
		if (false == IsDeath_)
		{
			GetLevel()->PushDebugRender(BodyCollider_->GetTransform(), CollisionType::OBBBox3D, float4::BLUE);
		}

		// ���콺 �׷���� �浹
		// ��, ���� ������������̸� �ǰ��������û��¶�� �浹üũ�ϸ�, �ش� �浹�� ����Ⱦ������� ǥ��

		
	}
}

void Monsters::CheckAttackCollision(float _DeltaTime)
{
	// �Ϲݰ��� �浹ü �浹üũ
	if (nullptr != AtkCollider_ && true == AtkCollider_->IsUpdate())
	{
//#ifdef _DEBUG
		GetLevel()->PushDebugRender(AtkCollider_->GetTransform(), CollisionType::OBBBox3D, float4::RED);
//#endif // _DEBUG

		// �÷��̾� �׷���� �浹
		std::list<GameEngineCollision*> CollisionList;
		if (true == IsAttack_)
		{
			CollisionList = AtkCollider_->GetCollisionList(static_cast<int>(eCollisionGroup::Player));

			for (GameEngineCollision* Col : CollisionList)
			{
				if (nullptr != CurTarget_)
				{
					// ������� ��� �������� ������
					// Ŭ���� ������ ��Ŷ�� �۽��ϰ� ��Ŷ���Ž� �������� ������
					if (true == GameServer::GetInstance()->IsOpened())
					{
						CurTarget_->Damage(static_cast<float>(StateInfo_.OffencePower_), this);
					}

					// ��Ŷ����
					MonsterDamagePacket DamagePacket;
					DamagePacket.SetIndex(Index_);
					FT::SendPacket(DamagePacket);

					// ���ݿϷ�� ���ݻ��� Flag Off
					IsAttack_ = false;
					AtkCollider_->Off();
				}
				else
				{
					// Ÿ���� �����Ƿ� ���ݻ��� Flag Off
					IsAttack_ = false;
					AtkCollider_->Off();
				}
			}
		}

		if (true == CollisionList.empty())
		{
			IsAttack_ = false;
			AtkCollider_->Off();
		}
	}
}

void Monsters::CheckSkillAttackCollision(float _DeltaTime)
{
	// ��ų�����غ� �浹ü �浹üũ
	if (nullptr != SkillAtkReadyCollider_ && true == SkillAtkReadyCollider_->IsUpdate())
	{
//#ifdef _DEBUG
		GetLevel()->PushDebugRender(SkillAtkReadyCollider_->GetTransform(), CollisionType::OBBBox3D, float4::GREEN);
//#endif

		// ��ų�����غ��浹ü�� ���� ������ ��ų����ó������ üũ

	}

	// ��ų���� �浹ü �浹üũ
	if (nullptr != SkillAtkCollider_ && true == SkillAtkCollider_->IsUpdate())
	{
//#ifdef _DEBUG
		GetLevel()->PushDebugRender(SkillAtkCollider_->GetTransform(), CollisionType::OBBBox3D, float4::RED);
//#endif // _DEBUG

		// ��ų�����浹ü�� ���� ������ ��ų����ó������ üũ

	}
}

void Monsters::HomingInstinctValueUpdate(float _DeltaTime)
{
	// Ÿ���� �����Ǿ��ִٸ� �ͼҺ��ɼ�ġ ����
	if (nullptr != CurTarget_ && CurStateType_ == MonsterStateType::CHASE && CurStateType_ != MonsterStateType::SKILLATTACK)
	{
		// ���� Ÿ�ٰ��� �����Ÿ� �־����� �ͼҺ��� ��ġ ���ҽ���
		float4 TargetPos = CurTarget_->GetTransform()->GetWorldPosition();
		float4 MyPos = GetTransform()->GetWorldPosition();
		float TargetDist = (TargetPos - MyPos).Len3D();
		if (StateInfo_.DetectRange_ <= TargetDist)
		{
			StateInfo_.HomingInstinctValue_ -= _DeltaTime;
			if (0.0f >= StateInfo_.HomingInstinctValue_)
			{
				// ��ȯ���·� ��ȯ
				ChangeAnimationAndState(MonsterStateType::HOMINGINSTINCT);

				// �ͼҺ��ɼ�ġ �ʱ�ȭ
				StateInfo_.HomingInstinctValue_ = StateInfo_.HomingInstinctValueMax_;

				// Ÿ����������
				CurTarget_ = nullptr;
				CurTargetIndex_ = -1;

				// ��Ŷ����
				MonsterStateChangePacket Packet;
				Packet.SetIndex(Index_);
				Packet.SetMonsterType(Type_);
				Packet.SetMonsterStateType(MonsterStateType::HOMINGINSTINCT);
				Packet.SetMonsterStatInfo(StateInfo_);
				Packet.SetTargetIndex(-1);
				if (true == GameServer::GetInstance()->IsOpened())
				{
					GameServer::GetInstance()->Send(&Packet);
				}
				else if (true == GameClient::GetInstance()->IsConnected())
				{
					GameClient::GetInstance()->Send(&Packet);
				}
			}
		}
	}
}

void Monsters::UpdateSkillCoolDown(float _DeltaTime)
{
	// ��, �ش� ���Ͱ� ��ų������ �������� ��Ÿ�� ����
	if (1 == StateInfo_.IsSkill_)
	{
		StateInfo_.SkillCoolDown_ -= _DeltaTime;
		if (0.0f >= StateInfo_.SkillCoolDown_ && MonsterStateType::SKILLATTACK != CurStateType_)
		{
			StateInfo_.SkillCoolDown_ = 0.0f;
		}
	}
}

void Monsters::StartMove(const float4& _Position)
{
	// �̵���� Get
	if (nullptr == CurrentMap_)
	{
		GameEngineDebug::MsgBoxError("���� ���� �����Ǿ������ʽ��ϴ�!!!!!!!");
		return;
	}

	MovePath_.clear();
	MovePath_ = CurrentMap_->FindPath(GetTransform()->GetWorldPosition(), _Position);
	if (true == MovePath_.empty())
	{
		GameEngineDebug::OutPutDebugString("�̵���� ������ �����Ͽ����ϴ�.");
		return;
	}

	// ���� �̵������ �̵��������� ����
	MoveTarget_ = MovePath_.back();
	MovePath_.pop_back();
}

void Monsters::UpdateMove(float _DeltaTime)
{
	// ���� �̵������� - ������ġ�� �Ÿ� ����
	float4 MyPosition = transform_.GetWorldPosition();
	MyPosition.y = MoveTarget_.y;
	if ((MoveTarget_ - MyPosition).Len3D() > 10.0f)
	{
		// �̵�ó��
		MoveProcessing(_DeltaTime, MyPosition);
	}
	else
	{
		// �����ִ� ��ΰ� �����Ҷ� ���ο� ������ ����
		if (false == MovePath_.empty())
		{
			MoveTarget_ = MovePath_.back();
			MovePath_.pop_back();
		}
		// �����ִ� ��ΰ� �������������� �̵�����
		else
		{
			EndMove();
		}
	}
}

void Monsters::EndMove()
{
	// ��ΰ��� �ʱ�ȭ
	MoveTarget_ = float4::ZERO;
	MovePath_.clear();

	// ������ ����
	ChangeAnimationAndState(MonsterStateType::IDLE);
}

void Monsters::MoveProcessing(float _DeltaTime, const float4& _Position)
{
	// �̵����� ��� �� ȸ��
	CalcMoveDir(_Position);

	// �̵���ġ ���
	float4 MoveSpeed = MoveDir_ * StateInfo_.MoveSpeed_ * _DeltaTime * 100.0f;		// �̵��ӵ� �⺻����� �ʹ� �۱⶧���� *100�� �ؼ� ���δ�
	float4 NextMovePos = _Position + MoveSpeed;

	// �̵�����üũ �� �̵������ϴٸ� �̵�ó��
	float Dist = 0.0f;
	if (true == CurrentMap_->GetNavMesh()->CheckIntersects(NextMovePos + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, Dist))
	{
		NextMovePos.y = FT::Map::MAX_HEIGHT - Dist;
		GetTransform()->SetWorldPosition(NextMovePos);
	}
	else
	{
		MoveTarget_ = _Position;
	}
}

void Monsters::CalcMoveDir(const float4& _Position)
{
	// �̵����� ��� �� �������ȭ
	MoveDir_ = MoveTarget_ - _Position;
	MoveDir_.Normalize3D();

	// ȸ��ó��
	float4 Cross = float4::Cross3D(MoveDir_, { 0.0f, 0.0f, 1.0f });
	Cross.Normalize3D();

	float Angle = float4::DegreeDot3DToACosAngle(MoveDir_, {0.0f, 0.0f, 1.0f});
	GetTransform()->SetLocalRotationDegree({ 0.0f, Angle * -Cross.y, 0.0f });

	// ���ܻ���
	// 1. Dog(�鰳)�ΰ�� �������� ȸ�����ϹǷ� ���⼳���� �������� ȸ������ ����� �����
	if (MonsterType::DOG == Type_)
	{
		MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });
		MainOutLineRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });
		MainSilhouetteRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });
	}
}

void Monsters::AttackProcessing()
{
	// ��ų���� �켱����
	if (1 == StateInfo_.IsSkill_)										// ��ų������ �ִ°��
	{
		if (0.0f >= StateInfo_.SkillCoolDown_)							// Update(float _DeltaTime)���� ������
		{
			// ��Ÿ�� �ʱ�ȭ
			StateInfo_.SkillCoolDown_ = StateInfo_.SkillCoolDownMax_;

			// ��ų���� ��ȯ
			ChangeAnimationAndState(MonsterStateType::SKILLATTACK);
		}
		else															// ��ų������Ÿ�����̹Ƿ� �Ϲݰ���
		{
			GameEngineRandom Rand;
			int NormalAtkType = Rand.RandomInt(static_cast<int>(MonsterStateType::ATK01), static_cast<int>(MonsterStateType::ATK02));
			if (MonsterStateType::ATK01 == static_cast<MonsterStateType>(NormalAtkType))
			{
				ChangeAnimationAndState(MonsterStateType::ATK02);
			}
			else
			{
				ChangeAnimationAndState(MonsterStateType::ATK01);
			}
		}
	}
	else									// �Ϲݰ��ݸ� ������ ���
	{
		GameEngineRandom Rand;
		int NormalAtkType = Rand.RandomInt(static_cast<int>(MonsterStateType::ATK01), static_cast<int>(MonsterStateType::ATK02));
		if (MonsterStateType::ATK01 == static_cast<MonsterStateType>(NormalAtkType))
		{
			ChangeAnimationAndState(MonsterStateType::ATK02);
		}
		else
		{
			ChangeAnimationAndState(MonsterStateType::ATK01);
		}
	}
}

void Monsters::DebugWindowUpdate()
{
	// MonsterDebugWindow



}

void Monsters::Start()
{
	// �ʱ�ȭ
	InitalizeFSMState();					// Initalize FSM State
	InitalizeStateInfo();					// Initalize StateInfo
	InitalizeResourceLoad();				// Initalize Resource Load
	InitalizeRenderAndAnimation();			// Initalize Renderer And Animation Setting
	InitalizeCollider();					// Initalize Collider

	ItemCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	ItemCollider_->SetCollisionGroup(eCollisionGroup::ItemBox);
	ItemCollider_->SetCollisionType(CollisionType::OBBBox3D);
	ItemCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	ItemCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());

	std::list<ItemBase*> allItemList = GetLevelConvert<LumiaLevel>()->GetItemBoxManager()->GetAllItemList();
	
	itemBox_ = GetLevel()->CreateActor<ItemBox>();

	for (const auto& item : allItemList)
	{
		if (std::string::npos != item->GetName().find("Meat") ||
			std::string::npos != item->GetName().find("Leather"))
		{
			if (std::string::npos != item->GetName().find("Armor"))
			{
				continue;
			}

			itemBox_->itemList.push_back(item->Copy());
		}
	}

	// ...
}

void Monsters::Update(float _deltaTime)
{
	// ���������� ���� ��ȯüũ
	CheckMonsterStateInfo(_deltaTime);

	// �浹ü �浹üũ
	CheckAllCollision(_deltaTime);

	// Main State Update
	MainState_.Update(_deltaTime);
}

Monsters::Monsters()
	: MainRenderer_(nullptr)
	, EffectRenderer_(nullptr)
	, BodyCollider_(nullptr)
	, ItemCollider_(nullptr)
	, AtkCollider_(nullptr)
	, SkillAtkReadyCollider_(nullptr)
	, SkillAtkCollider_(nullptr)
	, MainOutLineRenderer_(nullptr)
	, MainSilhouetteRenderer_(nullptr)
	, Index_(-1)
	, Type_(MonsterType::NONE)
	, AreaType_(Location::NONE)
	, CurrentNavFace_(nullptr)
	, CurrentNavMesh_(nullptr)
	, CurrentMap_(nullptr)
	, StateInfo_{}
	, MoveDir_(float4::ZERO)
	, MoveTarget_(float4::ZERO)
	, MainState_()
	, NormalState_()
	, CrowdControlState_()
	, AttackState_()
	, CurTarget_(nullptr)
	, CurTargetIndex_(-1)
	, CurStateBasicType_(MonsterStateBasicType::NONE)
	, PrevStateType_(MonsterStateType::NONE)
	, CurStateType_(MonsterStateType::NONE)
	, IsDeath_(false)
	, IsAttack_(false)
	, StunTime_(0.0f)
	, KnockbackTime_(0.0f)
	, KnockbackSpeed_(float4::ZERO)
	, bHidden_(false)
	, itemBox_(nullptr)
{
	// ������ ���ÿ� ����Ÿ�� ����
	UnitType_ = UnitType::MONSTER;
}

Monsters::~Monsters()
{
}
