#include "PreCompile.h"
#include "Bear.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "CharCrowdControlPacket.h"

#include "LumiaLevel.h"
#include "Character.h"

bool Bear::ResourceLoadFlag = false;

void Bear::InitalizeStateInfo()
{
	// �⺻����
	Type_ = MonsterType::BEAR;											// ����Ÿ��

	//====================================== ������ �ִ�ġ(����)
	StateInfo_.LevelMin_ = 5;											// ���� ����(������ �ش� �����ͷ� �����ʱ�ȭ)
	StateInfo_.LevelMax_ = 24;											// �ִ� ����
	StateInfo_.HomingInstinctValueMax_ = 5.0f;							// �ִ� �ͼҺ��� ��ġ
	StateInfo_.RegenTimeMax_ = 300.0f;									// ����Ÿ��
	StateInfo_.SkillCoolDownMax_ = 10.0f;								// �ִ� ��ų��Ÿ��

	//====================================== ������ ������(����)
	StateInfo_.OffencePowerIncrement_ = 35;								// ������ ���ݷ� ������
	StateInfo_.DefenseIncrement_ = 4;									// ������ ���� ������
	StateInfo_.MoveSpeedIncrement_ = 0.03f;								// ������ �̵��ӵ� ������
	StateInfo_.HPIncrement_ = 120.0f;									// ������ ������ ü�� ������

	//====================================== �⺻����(����)
	StateInfo_.NestPosition_ = float4::ZERO;							// ������ġ(= ������ġ)
	StateInfo_.AttackSpeed_ = 0.6f;										// ���ݼӵ�
	StateInfo_.AttackRange_ = 100.0f;									// ���� �þ߰Ÿ�
	StateInfo_.DetectRange_ = 300.0f;									// ���� �þ߰Ÿ�

	//====================================== �⺻����(����) - �ʱ�: 5�������� ����
	StateInfo_.Level_ = 5;												// ����(�ʰ� ������ ����(�߻�����)�ϼ��� ������ ���� ����) - ������ ���ÿ� ����(�������� ������ ������ ��� �⺻ 1��������)
	StateInfo_.OffencePower_ = 140;										// ���ݷ�
	StateInfo_.Defense_ = 59;											// ����
	StateInfo_.HPMax_ = 1100.0f;										// �ִ� ü��(������ ��ȭ)
	StateInfo_.HP_ = 1100.0f;											// ����ü��
	StateInfo_.MoveSpeed_ = 2.92f;										// �̵��ӵ�
	StateInfo_.HomingInstinctValue_ = 5.0f;								// �ͼҺ��� ��ġ(0.0f���� �۰ų��������� ���ʹ� ������ġ�� �̵��ϸ� ü���� ȸ�� -> Ÿ���� DetectRange_�� ��� �ð��� ���� ����)
	StateInfo_.RegenTime_ = 300.0f;										// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ

	//====================================== ��ų����
	StateInfo_.IsSkill_ = 1;											// ��ų����(0: ��ų����)
	StateInfo_.SkillCoolDown_ = 10.0f;									// ��ų����ð�

	//====================================== �����۰���
	StateInfo_.DropItems_[ItemName::MEAT] = 40.0f;
	StateInfo_.DropItems_[ItemName::LEATHER] = 60.0f;

	if (true == GameServer::GetInstance()->IsOpened())					// �����̸� ���������� ����
	{
		// �ϴ� ����
		StateInfo_.CurDropItems_.push_back(ItemName::MEAT);
		StateInfo_.CurDropItems_.push_back(ItemName::LEATHER);
	}
}

void Bear::InitalizeResourceLoad()
{
	// ���ҽ� �ε�� �ѹ��� ó��
	if (false == ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("EternalReturn");
		MeshDir / "Resources" / "FBX" / "Monster" / "Bear";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Bear_BaseMesh.fbx"));
		Basemesh->CreateRenderingBuffer();

		std::vector<GameEngineFile> allFile = MeshDir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		}

		// Monster Resource Load Flag On
		ResourceLoadFlag = true;
	}
}

void Bear::InitalizeRenderAndAnimation()
{
	//============================== ������ ����

	// ������ ����
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Bear_BaseMesh.fbx", "TextureDeferredLightAni");

	PivotPos_ = float4(-380.0f, 0.0f);
	MainRenderer_->GetTransform()->SetLocalPosition(PivotPos_);									// ������ �޽���ü�� �ǹ��� ������ ������ ��������Ƿ� ����
	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	// �ִϸ��̼� ����
	MainRenderer_->CreateFBXAnimation("APPEAR", "Bear_appear.UserAnimation", 0, false);			// ù��������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("REGEN", "Bear_appear.UserAnimation", 0, false);			// ��������(���� ��� �� ����Ÿ�ӿ� ���� ������ ����)�� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("IDLE", "Bear_wait.UserAnimation", 0);					// �������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("CHASE", "Bear_run.UserAnimation", 0);					// ���������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Bear_run.UserAnimation", 0);			// ��ȯ������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("HIT", "Bear_wait.UserAnimation", 0);						// �ǰݻ����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEATH", "Bear_death.UserAnimation", 0, false);			// ����߻����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEAD", "Bear_death.UserAnimation", 0, false);			// ���(�������)������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("ATK01", "Bear_atk01.UserAnimation", 0, false);			// �Ϲݰ���01������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("ATK02", "Bear_atk02.UserAnimation", 0, false);			// �Ϲݰ���02������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Bear_skill.UserAnimation", 0, false);		// ��ų���ݻ����� �ִϸ��̼�
	MainRenderer_->ChangeFBXAnimation("IDLE");
	MainRenderer_->LightShadowOn();

	// �⺻���� ����
	ChangeAnimationAndState(MonsterStateType::APPEAR);

	//============================== ��ó�� ������ ����

	// �ܰ���
	{
		// Main
		MainOutLineRenderer_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		MainOutLineRenderer_->SetBaseRenderer(MainRenderer_, "PreprocessingAni", true, false);

		OutLineData MainOutLineData = MainOutLineRenderer_->GetOutLineData();
		float4 MainThickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * MainOutLineData.LineThickness;
		float4 MainCalcScale = MainRenderer_->GetTransform()->GetLocalScaling() + MainThickness;
		MainOutLineRenderer_->GetTransform()->SetLocalScaling(MainCalcScale);
		MainOutLineRenderer_->GetTransform()->SetLocalRotationDegree(MainRenderer_->GetTransform()->GetLocalRotation());
		MainOutLineRenderer_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
		MainOutLineRenderer_->Off();
	}

	// �Ƿ翧
	{
		// Main
		MainSilhouetteRenderer_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		MainSilhouetteRenderer_->SetBaseRenderer(MainRenderer_, "PreprocessingAni", true);

		OutLineData MainOutLineData = MainSilhouetteRenderer_->GetOutLineData();
		float4 MainThickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * MainOutLineData.LineThickness;
		float4 MainCalcScale = MainRenderer_->GetTransform()->GetLocalScaling() + MainThickness;
		MainSilhouetteRenderer_->GetTransform()->SetLocalScaling(MainCalcScale);
		MainSilhouetteRenderer_->GetTransform()->SetLocalRotationDegree(MainRenderer_->GetTransform()->GetLocalRotation());
		MainSilhouetteRenderer_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	}
}

void Bear::InitalizeCollider()
{
	// �⺻: ��ü �浹ü ����
	BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	BodyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition() - PivotPos_);			// �ǹ� ����

	// �߰�: ���� �浹ü ����(�ɼ�)
	AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	AtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition() - PivotPos_);			// �ǹ� ����
	AtkCollider_->Off();

	// �߰�: ��ų���� �浹ü ����(�ɼ�)
	SkillAtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	SkillAtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	SkillAtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	SkillAtkCollider_->GetTransform()->SetLocalScaling({ 400.0f, 400.0f, 400.0f });												// �����Ÿ� - 4m = 400.0f�� ����
	SkillAtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition() - PivotPos_);			// �ǹ� ����
	SkillAtkCollider_->Off();
}

void Bear::StartSkillAttackProcessing()
{
}

void Bear::UpdateSkillAttackProcessing(float _DeltaTime)
{
	// ���鰭Ÿ : �ֺ������� ���ظ� ������ 1�ʰ� ����
	// ���ط� - �⺻(170) + ���ݷ��� 40%
	// �����Ÿ� - 4m
	// �����ð� - 0.5��
	// ��ٿ� - 10��
	// 1m == 100.0f�� ���
	if (false == SkillAtk_)
	{
		SkillAtk_CastTime_ -= _DeltaTime;
		if (0.0f >= SkillAtk_CastTime_)
		{
			// ��ų���� �ִϸ��̼� ����
			MainRenderer_->ChangeFBXAnimation("SKILLATTACK");

			// ��ų����ó���� ���� �غ�
			SkillAtk_ = true;

			// ��ų�����浹ü On
			SkillAtkCollider_->On();

			// ��ų�����浹ü�� �浹�� ��� ĳ���� �˻�
			std::list<GameEngineCollision*> SkillAtkList = SkillAtkCollider_->GetCollisionList(static_cast<int>(eCollisionGroup::Player));
			if (true == SkillAtkList.empty())
			{
				GameEngineDebug::OutPutDebugString("��ų�� ���������� �浹�� ĳ���Ͱ� ���������ʽ��ϴ�!!!!\n");
			}
			else
			{
				// �浹�� ��� ĳ���Ϳ��� ���� �� ������ ����
				for (auto& AtkTarget : SkillAtkList)
				{
					Character* AtkCharacter = dynamic_cast<Character*>(AtkTarget->GetActor());

					// ����
					AtkCharacter->Stun(SkillAtk_StunTime_);

					// ������Ŷ����
					CharCrowdControlPacket ccPacket;
					ccPacket.SetTargetIndex(AtkCharacter->GetIndex());
					ccPacket.SetStun(SkillAtk_StunTime_);
					FT::SendPacket(ccPacket);

					// ������
					float CurDamage = SkillAtk_FixedDamage_ + (static_cast<float>(StateInfo_.OffencePower_) * 0.4f);
					AtkCharacter->Damage(CurDamage, this);
				}
			}

			// �����ð� �ʱ�ȭ
			SkillAtk_CastTime_ = SkillAtk_CastTimeMax_;
		}
	}

	if (true == SkillAtk_)
	{
		// �������� 
		if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
		{
			// ��ų���� ��
			SkillAtk_ = false;
			SkillAtkCollider_->Off();

			// �����·� ��ȯ
			ChangeAnimationAndState(MonsterStateType::IDLE);
		}
	}
}

void Bear::EndSkillAttackProcessing()
{
}

Bear::Bear()
	: SkillAtk_(false)
	, SkillAtk_Range_(400.0f)
	, SkillAtk_CastTimeMax_(0.5f)
	, SkillAtk_CastTime_(0.5f)
	, SkillAtk_StunTime_(1.0f)
	, SkillAtk_FixedDamage_(170.0f)
{
}

Bear::~Bear()
{
}
