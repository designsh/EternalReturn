#pragma once
#include <GameEngine/GameEngineFSM.h>

#include "LevelUPData.h"
#include "IUnit.h"
#include "MonsterStateInfo.h"

// �з� : ����
// �뵵 : ���� �⺻(�θ�)Ŭ����
// ���� : ���� �ʿ� ��ġ�Ǵ� ���͵��� ��������
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
	void LevelUP(LevelUPData _Data);													// ���ӽð��� ���� ������

public: // Functions related to outline activation and deactivation
	void PickingOutLineOn();															// ���콺�� �浹�� ���� �ܰ��� Ȱ��ȭ
	void PickingOutLineOff();															// ���콺�� �浹������ ���� �ܰ��� ��Ȱ��ȭ

public: // Public Pure Virtual Function
	int GetIndex() override;															// �ε��� Get
	void Damage(float _Amount, IUnit* _Target) override;								// �ܺο��� ȣ���ϸ� �ش� ���Ϳ� �������ٶ� ȣ��Ǵ� �Լ�

public: // Public Monster Show & Hide Function
	void Show();																		// ���Ͱ� ������ ��� �������� On
	void Hide();																		// ���Ͱ� ������ ��� �������� Off

public: // ��Ŷ���Ž� ȣ��Ǵ� �Լ���
	void rcvAttack01(MonsterStateInfo _rcvStatInfo);									// ��Ŷ�������� ���� ó��(����ȭó��)
	void rcvAttack02(MonsterStateInfo _rcvStatInfo);									// ��Ŷ�������� ���� ó��(����ȭó��)
	void rcvDamage(MonsterStateInfo _rcvStatInfo, int _TargetIndex);					// ��Ŷ�������� ������ ó��(����ȭó��)
	void rcvDeath(MonsterStateInfo _rcvStatInfo);										// ��Ŷ�������� �����ó��(����ȭó��)
	void rcvDead(MonsterStateInfo _rcvStatInfo);										// ��Ŷ�������� �������ó��(����ȭó��)
	void rcvRegen(MonsterStateInfo _rcvStatInfo);										// ��Ŷ�������� �����ó��(����ȭó��)
	void rcvHomingInstinct(MonsterStateInfo _rcvStatInfo);								// ��Ŷ�������� ��ȯó��(����ȭó��)
	void rcvDamage();																	// ��Ŷ�������� ������ó��(����ȭó��)
	void rcvStun(float _StunTime);														// ��Ŷ�������� ����ó��(����ȭó��)
	void rcvKnockback(float _KnockbackTime, float4 _KnockbackSpeed);					// ��Ŷ�������� �˹�ó��(����ȭó��)
	void rcvWallSlam(float _KnockbackTime, float4 _KnockbackSpeed, float _StunTime);	// ��Ŷ�������� ������ó��(����ȭó��)

public: // Initalize Function
	void InitalizeSpawnPosition(const float4& _SpawnPosition);							// ���ڷ� ������ _SpawnPosition�� �ش� ������ ������ġ�� ����(Ŭ���̾�Ʈ����)

public: // public ChangeAnimation Function
	void ChangeAnimationAndState(MonsterStateType _StateType);							// ���ͻ���Ÿ������ �ִϸ��̼� �� ���� ��ȯ

public: // Public About Stat Update Function
	void UpdateSpecialStat(MonsterStateInfo _UpdateStat);								// Ư�� ������ �����Ͽ� ����
	void UpdateAllStat(MonsterStateInfo _UpdateStat);									// ��ü ������ �����Ͽ� ����

public: // Public CC Related Function
	void Stun(float _stunTime) override;
	void Knockback(float _knockbackTime, float4 _knockbackSpeed) override;
	void WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime) override;
	void Slow(float _slowTime, float _slowRatio) override;
public:
protected: // Protected Pure Virtual Function
	virtual void InitalizeStateInfo() = 0;												// ���� �������� �ʱ�ȭ
	virtual void InitalizeResourceLoad() = 0;											// ���� ���ҽ� �ε�
	virtual void InitalizeRenderAndAnimation() = 0;										// ������ & �ִϸ��̼� ����
	virtual void InitalizeCollider() = 0;												// �浹ü ����

protected: // Protected SkillAttack Pure Virtual Function
	virtual void StartSkillAttackProcessing() = 0;										// ���� Ÿ�Ժ� ��ų���ݽ���ó��
	virtual void UpdateSkillAttackProcessing(float _DeltaTime) = 0;						// ���� Ÿ�Ժ� ��ų���ݰ���ó��
	virtual void EndSkillAttackProcessing() = 0;										// ���� Ÿ�Ժ� ��ų��������ó��

private: // Private Initalize Function
	void InitalizeFSMState();															// FSM ���� ����(����)

private: // Private Update Function
	void CheckMonsterStateInfo(float _DeltaTime);										// �������� üũ(���º� ����������ȯ üũ)
	void CheckAllCollision(float _DeltaTime);											// �浹ü �浹üũ

private: // Collision Related Function
	void CheckBodyCollision(float _DeltaTime);											// ��ü�浹(�ǰ�����)
	void CheckAttackCollision(float _DeltaTime);										// �Ϲݰ���ü�浹(��������)
	void CheckSkillAttackCollision(float _DeltaTime);									// ��ų����ü�浹(��������)

private: // GlobalTime or Global Value Update Function
	void HomingInstinctValueUpdate(float _DeltaTime);									// �ͼҺ��� ��ġ ����
	void UpdateSkillCoolDown(float _DeltaTime);											// ��ų��Ÿ�� ����

private: // Private Move Related Function
	void StartMove(const float4& _Position);											// �̵�����
	void UpdateMove(float _DeltaTime);													// �̵�����
	void EndMove();																		// �̵�����
	void MoveProcessing(float _DeltaTime, const float4& _Position);						// �������� �̵�ó��
	void CalcMoveDir(const float4& _Position);											// �̵����� ���� �� �̵����⿡ �°� ������ ȸ��

private: // Private Attack Related Function
	void AttackProcessing();															// �������� ����ó��

private: // FSM State Function
	//==================================== Main State
	void StartNormalState();															// �Ϲݻ��� ����
	void UpdateNormalState(float _DeltaTime);											// �Ϲݻ��� ����
	void EndNormalState();																// �Ϲݻ��� ����

	void StartCrowdControlState();														// ����������� ����
	void UpdateCrowdControlState(float _DeltaTime);										// ����������� ����
	void EndCrowdControlState();														// ����������� ����

	void StartAttackState();															// ���ݻ��� ����
	void UpdateAttackState(float _DeltaTime);											// ���ݻ��� ����
	void EndAttackState();																// ���ݻ��� ����

	//==================================== Normal State
	void StartAppearState();															// ù������� ����(���� ��Ŷ���Ž�(������ ��ý���))
	void UpdateAppearState(float _DeltaTime);											// ù������� ����
	void EndAppearState();																// ù������� ����

	void StartRegenState();																// �������� ����(�����ʱ�ȭ)
	void UpdateRegenState(float _DeltaTime);											// �������� ����
	void EndRegenState();																// �������� ����

	void StartIdleState();																// ������ ����
	void UpdateIdleState(float _DeltaTime);												// ������ ����
	void EndIdleState();																// ������ ����

	void StartHomingInstinctState();													// �ͼҺ��ɻ��� ����(Ÿ������)
	void UpdateHomingInstinctState(float _DeltaTime);									// �ͼҺ��ɻ��� ����
	void EndHomingInstinctState();														// �ͼҺ��ɻ��� ����

	void StartChaseState();																// Ÿ���������� ����(Ÿ������)
	void UpdateChaseState(float _DeltaTime);											// Ÿ���������� ����
	void EndChaseState();																// Ÿ���������� ����

	//==================================== CrowdControl State
	void StartHitState();																// �ǰݻ��� ����(Ÿ����������)
	void UpdateHitState(float _DeltaTime);												// �ǰݻ��� ����
	void EndHitState();																	// �ǰݻ��� ����

	void StartDeathState();																// ����߻��� ����(�ǰ��浹üoff, ���콺�浹üon)
	void UpdateDeathState(float _DeltaTime);											// ����߻��� ����
	void EndDeathState();																// ����߻��� ����

	void StartDeadState();																// ���(�������)���� ����
	void UpdateDeadState(float _DeltaTime);												// ���(�������)���� ����
	void EndDeadState();																// ���(�������)���� ����

	void StartStunState();																// ���ϻ��� ����
	void UpdateStunState(float _DeltaTime);												// ���ϻ��� ����
	void EndStunState();																// ���ϻ��� ����

	void StartKnockbackState();															// �˹���� ����
	void UpdateKnockbackState(float _DeltaTime);										// �˹���� ����
	void EndKnockbackState();															// �˹���� ����

	void StartWallSlamState();															// �����ϻ��� ����
	void UpdateWallSlamState(float _DeltaTime);											// �����ϻ��� ����
	void EndWallSlamState();															// �����ϻ��� ����

	//==================================== Attack State(���� : Ÿ�������� �Ǿ�������)
	void StartAttack01State();															// �Ϲݰ���1���� ����
	void UpdateAttack01State(float _DeltaTime);											// �Ϲݰ���1���� ����
	void EndAttack01State();															// �Ϲݰ���1���� ����

	void StartAttack02State();															// �Ϲݰ���2���� ����
	void UpdateAttack02State(float _DeltaTime);											// �Ϲݰ���2���� ����
	void EndAttack02State();															// �Ϲݰ���2���� ����

	void StartSkillAttackState();														// ��ų���ݻ��� ����
	void UpdateSkillAttackState(float _DeltaTime);										// ��ų���ݻ��� ����
	void EndSkillAttackState();															// ��ų���ݻ��� ����

private: // Private DebugWindow Update Function
	void DebugWindowUpdate();															// ���� �⺻���� ǥ�ÿ�(IMGUI)

private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	Monsters();
	virtual ~Monsters() = 0;															// �ش� Ŭ������ ��� ������ ����(�θ�) Ŭ������ 
																						// �ݵ�� ��ӹ޾Ƽ� ��üȭ�ؾ��ϱ⶧���� ��������Ҹ��ڸ� ���

protected:
	Monsters(const Monsters& _other) = delete;
	Monsters(Monsters&& _other) = delete;

private:
	Monsters& operator=(const Monsters& _other) = delete;
	Monsters& operator=(const Monsters&& _other) = delete;

public:
protected: // �⺻ ������Ʈ
	GameEngineFBXRenderer* MainRenderer_;												// ���� ������
	GameEngineFBXRenderer* EffectRenderer_;												// ����Ʈ ������
	GameEngineCollision* BodyCollider_;													// ��ü �浹ü(���� -> �ǰ��浹ü)
	GameEngineCollision* ItemCollider_;													// ����� ��������۸��ǥ���浹ü
	GameEngineCollision* AtkCollider_;													// �Ϲݰ��� �浹ü(���� -> �Ϲݰ����浹ü)
	GameEngineCollision* SkillAtkReadyCollider_;										// ��ų�����غ� �浹ü(���� -> ��ų���ݰ���(�غ�))
	GameEngineCollision* SkillAtkCollider_;												// ��ų���� �浹ü(���� -> ��ų�����浹ü)

protected: // ��ó�� ������Ʈ
	GameEnginePreprocessingRenderer* MainOutLineRenderer_;								// ���η������� ��ó�� ������(�ܰ���) : ���η������� Off ���¶�� �ܰ��������� Off����
	GameEnginePreprocessingRenderer* MainSilhouetteRenderer_;							// ���η������� ��ó�� ������(�Ƿ翧) : ���η������� Off ���¶�� �Ƿ翧������ Off����

protected: // ��������(Ž����)
	int Index_;																			// ���� �ε���
	MonsterType Type_;																	// ������ Ÿ��
	Location AreaType_;																	// ���� ���� ���� Ÿ��(StateInfo_.AppearAreaList_ �� �������� ���õ� ����)

protected: // �� �� ��ã��
	NavFace* CurrentNavFace_;															// ���� ������
	NavMesh* CurrentNavMesh_;															// �׺�޽�
	LumiaMap* CurrentMap_;																// ���� ���Ͱ� ��ġ�� ��
	
protected: // ��������(���ſ�)
	MonsterStateInfo StateInfo_;														// ������ ��������
	float4 MoveDir_;																	// ���� �̵�����
	float4 MoveTarget_;																	// �̵�������
	std::vector<float4> MovePath_;														// ���� �̵����

protected: // FSM State
	GameEngineFSM MainState_;															// ���λ���(�⺻: NormalState ����)
	GameEngineFSM NormalState_;															// �⺻����(CC, ATK �� ���ƿ��� ����)
	GameEngineFSM CrowdControlState_;													// �����������(�ܺ� Callback������ ����Ǵ� ����)
	GameEngineFSM AttackState_;															// ���ݻ���(�Ϲݰ���, ��ų���� ���¸� ���� �� ����)
	
protected: // Ÿ��
	Character* CurTarget_;																// ���� ������ Ÿ��������(�ǰݽ� Ÿ������, ��ȯ����(�ͼҺ��� ��ġ�� 0.0f�϶�)���� �ʱ�ȭ)
	int CurTargetIndex_;																// ���� ������ Ÿ���ε���(�ǰݽ� Ÿ������, ��ȯ����(�ͼҺ��� ��ġ�� 0.0f�϶�)���� �ʱ�ȭ)

protected: // ��������
	MonsterStateBasicType CurStateBasicType_;											// ������ ������� �⺻Ÿ��(Normal, CrowdControl, Attack)
	MonsterStateType PrevStateType_;													// ������ ��������� �ٷ������� ����
	MonsterStateType CurStateType_;														// ������ ������� ��Ÿ��(Run, Idle, Hit, ...)

protected: // ��Ÿ
	bool IsDeath_;																		// ���ͻ���� �ǰ��������� Flag(����� On)
	bool IsAttack_;																		// ATK01, ATK02, SKILLATTACK ���½� Ȱ��ȭ
	bool bHidden_;

protected: // CC Value
	float StunTime_;																	// ���Ͻð�
	float KnockbackTime_;																// �˹�ð�
	float4 KnockbackSpeed_;																// �˹�ӵ�(�˹���� * �˹�ӵ�)

private:
	ItemBox* itemBox_;																	// ���ͻ���� ǥ�õǴ� ������ �ڽ�
};

