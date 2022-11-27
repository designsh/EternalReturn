#include "PreCompile.h"
#include "Bat.h"

#include <GameEngine/GameEngineCollision.h>

bool Bat::ResourceLoadFlag = false;

void Bat::InitalizeStateInfo()
{
	// �⺻����
	Type_ = MonsterType::BAT;											// ����Ÿ��

	//====================================== ������ �ִ�ġ(����)
	StateInfo_.LevelMin_ = 1;											// ���� ����(������ �ش� �����ͷ� �����ʱ�ȭ)
	StateInfo_.LevelMax_ = 11;											// �ִ� ����
	StateInfo_.HomingInstinctValueMax_ = 5.0f;							// �ִ� �ͼҺ��� ��ġ
	StateInfo_.RegenTimeMax_ = 195.0f;									// ����Ÿ��
	StateInfo_.SkillCoolDownMax_ = 10.0f;								// �ִ� ��ų��Ÿ��

	//====================================== ������ ������(����)
	StateInfo_.OffencePowerIncrement_ = 27;								// ������ ���ݷ� ������
	StateInfo_.DefenseIncrement_ = 5;									// ������ ���� ������
	StateInfo_.MoveSpeedIncrement_ = 0.04f;								// ������ �̵��ӵ� ������
	StateInfo_.HPIncrement_ = 114.0f;									// ������ ������ ü�� ������

	//====================================== �⺻����(����)
	StateInfo_.NestPosition_ = float4::ZERO;							// ������ġ(= ������ġ)
	StateInfo_.AttackSpeed_ = 0.6f;										// ���ݼӵ�
	StateInfo_.AttackRange_ = 100.0f;									// ���� �þ߰Ÿ�
	StateInfo_.DetectRange_ = 300.0f;									// ���� �þ߰Ÿ�

	//====================================== �⺻����(����) - �ʱ�: 1�������� ����
	StateInfo_.Level_ = 1;												// ����(�ʰ� ������ ����(�߻�����)�ϼ��� ������ ���� ����) - ������ ���ÿ� ����(�������� ������ ������ ��� �⺻ 1��������)
	StateInfo_.OffencePower_ = 38;										// ���ݷ�
	StateInfo_.Defense_ = 24;											// ����
	StateInfo_.HPMax_ = 360.0f;											// �ִ� ü��(������ ��ȭ)
	StateInfo_.HP_ = 360.0f;											// ����ü��
	StateInfo_.MoveSpeed_ = 3.5f;										// �̵��ӵ�
	StateInfo_.HomingInstinctValue_ = 5.0f;								// �ͼҺ��� ��ġ(0.0f���� �۰ų��������� ���ʹ� ������ġ�� �̵��ϸ� ü���� ȸ�� -> Ÿ���� DetectRange_�� ��� �ð��� ���� ����)
	StateInfo_.RegenTime_ = 195.0f;										// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ

	//====================================== ��ų����
	StateInfo_.IsSkill_ = 0;											// ��ų����(0: ��ų����)
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

void Bat::InitalizeResourceLoad()
{
	// ���ҽ� �ε�� �ѹ��� ó��
	if (false == ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("EternalReturn");
		MeshDir / "Resources" / "FBX" / "Monster" / "Bat";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Bat_BaseMesh.fbx"));
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

void Bat::InitalizeRenderAndAnimation()
{
	//============================== ������ ����

	// ������ ����
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Bat_BaseMesh.fbx", "TextureDeferredLightAni");

	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	// �ִϸ��̼� ����
	MainRenderer_->CreateFBXAnimation("APPEAR", "Bat_appear.UserAnimation", 0, false);			// ù��������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("REGEN", "Bat_appear.UserAnimation", 0, false);				// ��������(���� ��� �� ����Ÿ�ӿ� ���� ������ ����)�� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("IDLE", "Bat_wait.UserAnimation", 0);						// �������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("CHASE", "Bat_run.UserAnimation", 0);						// ���������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Bat_run.UserAnimation", 0);				// ��ȯ������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("HIT", "Bat_wait.UserAnimation", 0);						// �ǰݻ����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEATH", "Bat_death.UserAnimation", 0, false);				// ����߻����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEAD", "Bat_death.UserAnimation", 0, false);				// ���(�������)������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("ATK01", "Bat_atk01.UserAnimation", 0, false);				// �Ϲݰ���01������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("ATK02", "Bat_atk02.UserAnimation", 0, false);				// �Ϲݰ���02������ �ִϸ��̼�
	//MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Bat_skill.UserAnimation", 0, false);		// ��ų���ݻ����� �ִϸ��̼�(������)
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

void Bat::InitalizeCollider()
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
}

void Bat::StartSkillAttackProcessing()
{
}

void Bat::UpdateSkillAttackProcessing(float _DeltaTime)
{
	// ��ų�����Ƿ� ��ų���ݸ�� ����� �����·� ��ȯ
	if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		// �������� ������ ��ȯ
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Bat::EndSkillAttackProcessing()
{
}

Bat::Bat()
{
}

Bat::~Bat()
{
}
