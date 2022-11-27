#include "PreCompile.h"
#include "Weekline.h"

#include <GameEngine/GameEngineCollision.h>

bool Weekline::ResourceLoadFlag = false;

void Weekline::InitalizeStateInfo()
{
	//// �⺻����
	//Type_ = MonsterType::WEEKLINE;										// ����Ÿ��

	////====================================== ������ �ִ�ġ(����)
	//StateInfo_.LevelMin_ = 1;											// ���� ����(������ �ش� �����ͷ� �����ʱ�ȭ)
	//StateInfo_.LevelMax_ = 17;											// �ִ� ����
	//StateInfo_.HomingInstinctValueMax_ = 100.0f;						// �ִ� �ͼҺ��� ��ġ
	//StateInfo_.RegenTimeMax_ = 180.0f;									// ����Ÿ��
	//StateInfo_.SkillCoolDownMax_ = 10.0f;								// �ִ� ��ų��Ÿ��

	////====================================== ������ ������(����)
	//StateInfo_.OffencePowerIncrement_ = 18;								// ������ ���ݷ� ������
	//StateInfo_.DefenseIncrement_ = 4;									// ������ ���� ������
	//StateInfo_.MoveSpeedIncrement_ = 0.06f;								// ������ �̵��ӵ� ������
	//StateInfo_.HPIncrement_ = 82.0f;									// ������ ������ ü�� ������

	////====================================== �⺻����(����)
	//StateInfo_.NestPosition_ = float4::ZERO;							// ������ġ(= ������ġ)
	//StateInfo_.AttackSpeed_ = 0.9f;										// ���ݼӵ�
	//StateInfo_.AttackRange_ = 100.0f;									// ���� �þ߰Ÿ�
	//StateInfo_.DetectRange_ = 300.0f;									// ���� �þ߰Ÿ�

	////====================================== �⺻����(����) - �ʱ�: 3�������� ����
	//StateInfo_.Level_ = 3;												// ����(�ʰ� ������ ����(�߻�����)�ϼ��� ������ ���� ����) - ������ ���ÿ� ����(�������� ������ ������ ��� �⺻ 1��������)
	//StateInfo_.OffencePower_ = 36;										// ���ݷ�
	//StateInfo_.Defense_ = 29;											// ����
	//StateInfo_.HPMax_ = 564.0f;											// �ִ� ü��(������ ��ȭ)
	//StateInfo_.HP_ = 564.0f;											// ����ü��
	//StateInfo_.MoveSpeed_ = 2.9f;										// �̵��ӵ�
	//StateInfo_.HomingInstinctValue_ = 100.0f;							// �ͼҺ��� ��ġ(0.0f���� �۰ų��������� ���ʹ� ������ġ�� �̵��ϸ� ü���� ȸ�� -> Ÿ���� DetectRange_�� ��� �ð��� ���� ����)
	//StateInfo_.RegenTime_ = 180.0f;										// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ

	////====================================== ��ų����
	//StateInfo_.IsSkill_ = 0;											// ��ų����(0: ��ų����)
	//StateInfo_.SkillCoolDown_ = 0.5f;									// ��ų����ð�

	////====================================== �����۰���
	//StateInfo_.DropItems_[ItemName::MEAT] = 40.0f;
	//StateInfo_.DropItems_[ItemName::LEATHER] = 60.0f;

	//if (true == GameServer::GetInstance()->IsOpened())					// �����̸� ���������� ����
	//{
	//	// �ϴ� ����
	//	StateInfo_.CurDropItems_.push_back(ItemName::MEAT);
	//	StateInfo_.CurDropItems_.push_back(ItemName::LEATHER);
	//}
}

void Weekline::InitalizeResourceLoad()
{
	// ���ҽ� �ε�� �ѹ��� ó��
	if (false == ResourceLoadFlag)
	{
		//// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		//GameEngineDirectory MeshDir;
		//MeshDir.MoveParent("EternalReturn");
		//MeshDir / "Resources" / "FBX" / "Monster" / "Weekline";

		//GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Weekline_BaseMesh.fbx"));
		//Basemesh->CreateRenderingBuffer();

		//std::vector<GameEngineFile> allFile = MeshDir.GetAllFile("UserAnimation");
		//for (GameEngineFile& file : allFile)
		//{
		//	GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		//}

		// Monster Resource Load Flag On
		ResourceLoadFlag = true;
	}
}

void Weekline::InitalizeRenderAndAnimation()
{
	//// ������ ����
	//MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	//MainRenderer_->SetFBXMesh("Weekline_BaseMesh.fbx", "TextureDeferredLightAni");

	//MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	//MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	//// �ִϸ��̼� ����
	//MainRenderer_->CreateFBXAnimation("APPEAR", "Weekline_appear.UserAnimation", 0, false);			// ù��������� �ִϸ��̼�
	//MainRenderer_->CreateFBXAnimation("REGEN", "Weekline_appear.UserAnimation", 0, false);			// ��������(���� ��� �� ����Ÿ�ӿ� ���� ������ ����)�� �ִϸ��̼�
	//MainRenderer_->CreateFBXAnimation("IDLE", "Weekline_wait.UserAnimation", 0);						// �������� �ִϸ��̼�
	//MainRenderer_->CreateFBXAnimation("CHASE", "Weekline_run.UserAnimation", 0);						// ���������� �ִϸ��̼�
	//MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Weekline_run.UserAnimation", 0);				// ��ȯ������ �ִϸ��̼�
	// 
	//MainRenderer_->CreateFBXAnimation("HIT", "Weekline_wait.UserAnimation", 0);						// �ǰݻ����� �ִϸ��̼�
	//MainRenderer_->CreateFBXAnimation("DEATH", "Weekline_death.UserAnimation", 0, false);				// ����߻����� �ִϸ��̼�
	//MainRenderer_->CreateFBXAnimation("DEAD", "Weekline_death.UserAnimation", 0, false);				// ���(�������)������ �ִϸ��̼�
	// 
	//MainRenderer_->CreateFBXAnimation("ATK01", "Weekline_atk01.UserAnimation", 0, false);				// �Ϲݰ���01������ �ִϸ��̼�
	//MainRenderer_->CreateFBXAnimation("ATK02", "Weekline_atk02.UserAnimation", 0, false);				// �Ϲݰ���02������ �ִϸ��̼�
	//MainRenderer_->ChangeFBXAnimation("IDLE");

	//// �⺻���� ����
	//ChangeAnimationAndState(MonsterStateType::APPEAR);
}

void Weekline::InitalizeCollider()
{
	//// �⺻: ��ü �浹ü ����
	//BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	//BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	//BodyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	//BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	//BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());

	// �߰�: ���� �浹ü ����(�ɼ�)
	//AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	//AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	//AtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	//AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	//AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	//AtkCollider_->Off();
}

void Weekline::StartSkillAttackProcessing()
{
}

void Weekline::UpdateSkillAttackProcessing(float _DeltaTime)
{
	// ��ų���ݸ�� ����� �����·� ��ȯ
	if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		// �������� ������ ��ȯ
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Weekline::EndSkillAttackProcessing()
{
}

Weekline::Weekline()
{
}

Weekline::~Weekline()
{
}
