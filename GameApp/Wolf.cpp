#include "Precompile.h"
#include "Wolf.h"

#include <GameEngine/GameEngineCollision.h>

#include "LumiaLevel.h"
#include "Character.h"

bool Wolf::ResourceLoadFlag = false;

void Wolf::MusterSkill(Character* _Target)
{
	// �ش� ��ų�� ������ ���Ͱ� Ÿ������ ������ Ÿ���� ���� Ÿ������ �����Ͽ� ���� �� ���� ����
	// ��, �̹� ������ Ÿ���� �����Ѵٸ� ����
	if (nullptr == CurTarget_)
	{
		CurTarget_ = _Target;
		CurTargetIndex_ = _Target->GetIndex();
	}
}

void Wolf::InitalizeStateInfo()
{
	// �⺻����
	Type_ = MonsterType::WOLF;											// ����Ÿ��

	//====================================== ������ �ִ�ġ(����)
	StateInfo_.LevelMin_ = 3;											// ���� ����(������ �ش� �����ͷ� �����ʱ�ȭ)
	StateInfo_.LevelMax_ = 20;											// �ִ� ����
	StateInfo_.HomingInstinctValueMax_ = 5.0f;							// �ִ� �ͼҺ��� ��ġ
	StateInfo_.RegenTimeMax_ = 240.0f;									// ����Ÿ��
	StateInfo_.SkillCoolDownMax_ = 10.0f;								// �ִ� ��ų��Ÿ��

	//====================================== ������ ������(����)
	StateInfo_.OffencePowerIncrement_ = 23;								// ������ ���ݷ� ������
	StateInfo_.DefenseIncrement_ = 4;									// ������ ���� ������
	StateInfo_.MoveSpeedIncrement_ = 0.05f;								// ������ �̵��ӵ� ������
	StateInfo_.HPIncrement_ = 83.9375f;									// ������ ������ ü�� ������

	//====================================== �⺻����(����)
	StateInfo_.NestPosition_ = float4::ZERO;							// ������ġ(= ������ġ)
	StateInfo_.AttackSpeed_ = 0.9f;										// ���ݼӵ�
	StateInfo_.AttackRange_ = 100.0f;									// ���� �þ߰Ÿ�
	StateInfo_.DetectRange_ = 300.0f;									// ���� �þ߰Ÿ�

	//====================================== �⺻����(����) - �ʱ�: 3�������� ����
	StateInfo_.Level_ = 3;												// ����(�ʰ� ������ ����(�߻�����)�ϼ��� ������ ���� ����) - ������ ���ÿ� ����(�������� ������ ������ ��� �⺻ 1��������)
	StateInfo_.OffencePower_ = 44;										// ���ݷ�
	StateInfo_.Defense_ = 28;											// ����
	StateInfo_.HPMax_ = 638.0f;											// �ִ� ü��(������ ��ȭ)
	StateInfo_.HP_ = 638.0f;											// ����ü��
	StateInfo_.MoveSpeed_ = 2.9f;										// �̵��ӵ�
	StateInfo_.HomingInstinctValue_ = 5.0f;								// �ͼҺ��� ��ġ(0.0f���� �۰ų��������� ���ʹ� ������ġ�� �̵��ϸ� ü���� ȸ�� -> Ÿ���� DetectRange_�� ��� �ð��� ���� ����)
	StateInfo_.RegenTime_ = 240.0f;										// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ

	//====================================== ��ų����
	StateInfo_.IsSkill_ = 1;											// ��ų����(0: ��ų����)
	StateInfo_.SkillCoolDown_ = 0.5f;									// ��ų����ð�

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

void Wolf::InitalizeResourceLoad()
{
	// ���ҽ� �ε�� �ѹ��� ó��
	if (false == ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("EternalReturn");
		MeshDir / "Resources" / "FBX" / "Monster" / "Wolf";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_BaseMesh.fbx"));
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

void Wolf::InitalizeRenderAndAnimation()
{
	//============================== ������ ����

	// ������ ����
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Wolf_BaseMesh.fbx", "TextureDeferredLightAni");

	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	// �ִϸ��̼� ����
	MainRenderer_->CreateFBXAnimation("APPEAR", "Wolf_appear.UserAnimation", 0, false);			// ù��������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("REGEN", "Wolf_appear.UserAnimation", 0, false);			// ��������(���� ��� �� ����Ÿ�ӿ� ���� ������ ����)�� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("IDLE", "Wolf_wait.UserAnimation", 0);						// �������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("CHASE", "Wolf_run.UserAnimation", 0);						// ���������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Wolf_run.UserAnimation", 0);				// ��ȯ������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("HIT", "Wolf_wait.UserAnimation", 0);						// �ǰݻ����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEATH", "Wolf_death.UserAnimation", 0, false);				// ����߻����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEAD", "Wolf_death.UserAnimation", 0, false);				// ���(�������)������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("ATK01", "Wolf_atk01.UserAnimation", 0, false);				// �Ϲݰ���01������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("ATK02", "Wolf_atk02.UserAnimation", 0, false);				// �Ϲݰ���02������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Wolf_skill.UserAnimation", 0, false);		// ��ų���ݻ����� �ִϸ��̼�
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

void Wolf::InitalizeCollider()
{
	// �⺻: ��ü �浹ü ����
	BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	BodyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());

	// �߰�: ���� �浹ü ����(�ɼ�)
	AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	AtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	AtkCollider_->Off();

	//// �߰�: ��ų���� �浹ü ����(�ɼ�)
	//SkillAtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	//SkillAtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	//SkillAtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	//SkillAtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	//SkillAtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	//SkillAtkCollider_->Off();
}

void Wolf::StartSkillAttackProcessing()
{
}

void Wolf::UpdateSkillAttackProcessing(float _DeltaTime)
{
	if (false == SkillAtk_)
	{
		// �ִϸ��̼� ����
		MainRenderer_->ChangeFBXAnimation("SKILLATTACK");

		// �浹ü Off���¿��� ����
		// ����: �ݰ� 20m�� ������� �ҷ���Ƽ� �Բ� ���� ����(��ý���)
		// 1m == 100.0f�� ���
		std::vector<Monsters*>& WolfList = GetLevelConvert<LumiaLevel>()->GetMonsterTypeList(MonsterType::WOLF);
		if (true == WolfList.empty())
		{
			// �ֺ��� ���밡 �������������Ƿ� ó������....
			GameEngineDebug::OutPutDebugString("���밡 ������ų�� ���������� �ֺ� 20m�� ���밡 ���������ʽ��ϴ�!!!!\n");
		}
		else
		{
			int WolfCount = static_cast<int>(WolfList.size());
			for (int WolfNum = 0; WolfNum < WolfCount; ++WolfNum)
			{
				// ������ �Ÿ� �����Ͽ� ��ų�����Ÿ����� �����ϴ� ���븦 ����
				float4 MyPosition = GetTransform()->GetWorldPosition();
				float4 MusterWolfPosition = WolfList[WolfNum]->GetTransform()->GetWorldPosition();
				if ((MusterWolfPosition - MyPosition).Len3D() <= SkillAtk_Range_ && WolfList[WolfNum]->GetIndex() != Index_)
				{
					Monsters* CheckMonster = WolfList[WolfNum];
					Wolf* CheckWolf = dynamic_cast<Wolf*>(CheckMonster);
					CheckWolf->MusterSkill(CurTarget_);
				}
			}
		}

		SkillAtk_ = true;
	}

	// �������� 
	if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		// ��ų���� ��
		SkillAtk_ = false;

		// �������� ������ ��ȯ
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Wolf::EndSkillAttackProcessing()
{
}

Wolf::Wolf()
	: SkillAtk_(false)
	, SkillAtk_Range_(2000.0f)
	, SkillAtk_CastTime_(0.0f)
{

}

Wolf::~Wolf()
{
}
