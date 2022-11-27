#include "PreCompile.h"
#include "Boar.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "CharCrowdControlPacket.h"

#include "LumiaLevel.h"
#include "LumiaMap.h"
#include "Character.h"

//================================================================================= Boar
bool Boar::ResourceLoadFlag = false;

void Boar::InitalizeStateInfo()
{
	// �⺻����
	Type_ = MonsterType::BOAR;											// ����Ÿ��

	//====================================== ������ �ִ�ġ(����)
	StateInfo_.LevelMin_ = 2;											// ���� ����(������ �ش� �����ͷ� �����ʱ�ȭ)
	StateInfo_.LevelMax_ = 12;											// �ִ� ����
	StateInfo_.HomingInstinctValueMax_ = 5.0f;							// �ִ� �ͼҺ��� ��ġ
	StateInfo_.RegenTimeMax_ = 195.0f;									// ����Ÿ��
	StateInfo_.SkillCoolDownMax_ = 7.0f;								// �ִ� ��ų��Ÿ��

	//====================================== ������ ������(����)
	StateInfo_.OffencePowerIncrement_ = 23;								// ������ ���ݷ� ������
	StateInfo_.DefenseIncrement_ = 7;									// ������ ���� ������
	StateInfo_.MoveSpeedIncrement_ = 0.05f;								// ������ �̵��ӵ� ������
	StateInfo_.HPIncrement_ = 167.0f;									// ������ ������ ü�� ������

	//====================================== �⺻����(����)
	StateInfo_.NestPosition_ = float4::ZERO;							// ������ġ(= ������ġ)
	StateInfo_.AttackSpeed_ = 0.5f;										// ���ݼӵ�
	StateInfo_.AttackRange_ = 100.0f;									// ���� �þ߰Ÿ�
	StateInfo_.DetectRange_ = 300.0f;									// ���� �þ߰Ÿ�

	//====================================== �⺻����(����) - �ʱ�: 2�������� ����
	StateInfo_.Level_ = 2;												// ����(�ʰ� ������ ����(�߻�����)�ϼ��� ������ ���� ����) - ������ ���ÿ� ����(�������� ������ ������ ��� �⺻ 1��������)
	StateInfo_.OffencePower_ = 51;										// ���ݷ�
	StateInfo_.Defense_ = 16;											// ����
	StateInfo_.HPMax_ = 942.0f;											// �ִ� ü��(������ ��ȭ)
	StateInfo_.HP_ = 942.0f;											// ����ü��
	StateInfo_.MoveSpeed_ = 2.85f;										// �̵��ӵ�
	StateInfo_.HomingInstinctValue_ = 5.0f;								// �ͼҺ��� ��ġ(0.0f���� �۰ų��������� ���ʹ� ������ġ�� �̵��ϸ� ü���� ȸ�� -> Ÿ���� DetectRange_�� ��� �ð��� ���� ����)
	StateInfo_.RegenTime_ = 195.0f;										// ����Ÿ��(����) -> 0.0f�����Ͻ� RegenTime_���� �ʱ�ȭ

	//====================================== ��ų����
	StateInfo_.IsSkill_ = 1;											// ��ų����(0: ��ų����)
	StateInfo_.SkillCoolDown_ = 7.0f;									// ��ų����ð�

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

void Boar::InitalizeResourceLoad()
{
	// ���ҽ� �ε�� �ѹ��� ó��
	if (false == ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("EternalReturn");
		MeshDir / "Resources" / "FBX" / "Monster" / "Boar";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_BaseMesh.fbx"));
		Basemesh->CreateRenderingBuffer();

		// UserAnimation Resource Load
		std::vector<GameEngineFile> allFile = MeshDir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		}

		// HitBoxRange Mesh Load
		GameEngineDirectory HitBoxMeshDir;
		HitBoxMeshDir.MoveParent("EternalReturn");
		HitBoxMeshDir / "Resources" / "FBX" / "Monster" / "Boar" / "HitBoxRange";

		// HitRangeFrame Mesh
		GameEngineFBXMesh* HitBoxFrame = GameEngineFBXMeshManager::GetInst().Load(HitBoxMeshDir.PathToPlusFileName("Boar_SkillAtk_HitRange_Frame.fbx"));
		HitBoxFrame->CreateRenderingBuffer();

		// HitRangeBottom Mesh
		GameEngineFBXMesh* HitBoxBottom = GameEngineFBXMeshManager::GetInst().Load(HitBoxMeshDir.PathToPlusFileName("Boar_SkillAtk_HitRange_Bottom.fbx"));
		HitBoxBottom->CreateRenderingBuffer();

		// Monster Resource Load Flag On
		ResourceLoadFlag = true;
	}
}

void Boar::InitalizeRenderAndAnimation()
{
	//============================= ���η����� ����
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Boar_BaseMesh.fbx", "TextureDeferredLightAni");
	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	//============================= ���η����� �ִϸ��̼� ����
	MainRenderer_->CreateFBXAnimation("APPEAR", "Boar_appear.UserAnimation", 0, false);					// ù��������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("REGEN", "Boar_appear.UserAnimation", 0, false);					// ��������(���� ��� �� ����Ÿ�ӿ� ���� ������ ����)�� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("IDLE", "Boar_wait.UserAnimation", 0);							// �������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("CHASE", "Boar_run.UserAnimation", 0);							// ���������� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Boar_run.UserAnimation", 0);					// ��ȯ������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("HIT", "Boar_wait.UserAnimation", 0);								// �ǰݻ����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEATH", "Boar_death.UserAnimation", 0, false);					// ����߻����� �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("DEAD", "Boar_death.UserAnimation", 0, false);					// ���(�������)������ �ִϸ��̼�

	MainRenderer_->CreateFBXAnimation("ATK01", "Boar_atk01.UserAnimation", 0, false);					// �Ϲݰ���01������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("ATK02", "Boar_atk02.UserAnimation", 0, false);					// �Ϲݰ���02������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("SKILLATTACKREADY", "Boar_skill_ready.UserAnimation", 0, false);	// ��ų�����غ������ �ִϸ��̼�
	MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Boar_skill_assault.UserAnimation", 0);			// ��ų�����߻����� �ִϸ��̼�(�������� : Ÿ�ٰ� �浹�Ҷ����� �����ؾ��ϱ⶧����)

	MainRenderer_->ChangeFBXAnimation("IDLE");
	MainRenderer_->LightShadowOn();

	//============================= �⺻���� ����
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

	//============================= HitBoxRange Renderer
	HitBox_Frame_ = CreateTransformComponent<GameEngineFBXRenderer>();
	HitBox_Frame_->SetFBXMesh("Boar_SkillAtk_HitRange_Frame.fbx", "TextureDeferredLight");
	HitBox_Frame_->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });
	HitBox_Frame_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	for (UINT i = 0; i < HitBox_Frame_->GetRenderSetCount(); i++)
	{
		HitBox_Frame_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Red.png");
		HitBox_Frame_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerNone");
	}
	HitBox_Frame_->Off();

	HitBox_Bottom_ = CreateTransformComponent<GameEngineFBXRenderer>();
	HitBox_Bottom_->SetFBXMesh("Boar_SkillAtk_HitRange_Bottom.fbx", "TextureDeferredLight");
	HitBox_Bottom_->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });
	HitBox_Bottom_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });
	for (UINT i = 0; i < HitBox_Bottom_->GetRenderSetCount(); i++)
	{
		HitBox_Bottom_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Red.png");
		HitBox_Bottom_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerNone");
	}
	HitBox_Bottom_->Off();
}

void Boar::InitalizeCollider()
{
	// �⺻: ��ü �浹ü ����
	BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	BodyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());

	// �߰�: �Ϲݰ��� �浹ü ����(�ɼ�)
	AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	AtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	AtkCollider_->Off();

	// �߰�: ��ų���ݰ��� �浹ü ����(�ɼ�)
	SkillAtkReadyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	SkillAtkReadyCollider_->SetCollisionGroup(eCollisionGroup::MonsterSight);
	SkillAtkReadyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	SkillAtkReadyCollider_->GetTransform()->SetLocalScaling({ 500.0f, 500.0f, 500.0f });
	SkillAtkReadyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	SkillAtkReadyCollider_->Off();

	// �߰�: ��ų���� �浹ü ����(�ɼ�)
	SkillAtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	SkillAtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	SkillAtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	SkillAtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	SkillAtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	SkillAtkCollider_->Off();
}

void Boar::StartSkillAttackProcessing()
{
	// ��ų������ �ǰ�ó�����ø� ���� ��ü �浹ü Off
	BodyCollider_->Off();
}

void Boar::UpdateSkillAttackProcessing(float _DeltaTime)
{
	// ���� : ���������� �����ϸ� ���� �о�� ���ظ� ����
	//		  ��, ���� ���� �ε����ٸ� ����
	// ���ط� - �⺻(150) + ���ݷ��� 300%
	// �����Ÿ� - 5m
	// �����ð� - 1.5��
	// ��ٿ� - 7��
	// 1m == 100.0f�� ���
	// ��ų�����غ�Ϸ�, ��ų���ݽ���
	if (false == SkillAtkReady_ && true == SkillAtk_)
	{
		// ��ų���� ���������Ƿ� ����Ʈ������ Off
		HitBoxRangeOff();

		// Ÿ����ġ���� �����ϸ� Ÿ�ٰ� �浹�� Ÿ�ٿ��� �˹�, �������� ������ ��ų���ݼ������� �����·� ��ȯ
		// �浹���н� ��ų���ݽ��з� �����·� ��ȯ

		// �浹üũ
		std::list<GameEngineCollision*> SkillAtkList = SkillAtkCollider_->GetCollisionList(static_cast<int>(eCollisionGroup::Player));
		if (false == SkillAtkList.empty())
		{
			// �浹�� ĳ���͵� �� Ÿ���� �����ϸ� �˹� �� ������
			for (auto& AtkTarget : SkillAtkList)
			{
				Character* AtkCharacter = dynamic_cast<Character*>(AtkTarget->GetActor());
				if (AtkCharacter->GetIndex() == CurTargetIndex_)
				{
					// �˹�(���� ���� �ε����ٸ� ����)
					AtkCharacter->WallSlam(0.1f, SkillAtk_DetectTargetDir_ * SkillAtk_RushSpeed_, 1.0f);

					// �˹� ��Ŷ����
					CharCrowdControlPacket ccPacket;
					ccPacket.SetTargetIndex(AtkCharacter->GetIndex());
					ccPacket.SetWallSlam(0.1f, SkillAtk_DetectTargetDir_ * SkillAtk_RushSpeed_, 1.0f);
					FT::SendPacket(ccPacket);

					// ������(�⺻(150) + ���ݷ��� 300%)
					float CurDamage = SkillAtk_FixedDamage_ + (static_cast<float>(StateInfo_.OffencePower_) * 3.0f);
					AtkCharacter->Damage(CurDamage, this);

					//======================== ��ų���ݿϷ�
					// ��ų�����غ� �ʱ�ȭ
					SkillAtkReady_ = false;
					SkillAtkReadyCollider_->Off();

					// ��ų���� �ʱ�ȭ
					SkillAtk_ = false;
					SkillAtkCollider_->Off();

					// �������� ������ ��ȯ
					ChangeAnimationAndState(MonsterStateType::IDLE);

					return;
				}
			}
		}

		// �浹�����ʾҴٸ� ���� or �����Ϸ�üũ
		if ((SkillAtk_DetectTargetPos_ - GetTransform()->GetWorldPosition()).Len3D() > 10.0f)
		{
			// ������
			float4 MoveSpeed = SkillAtk_DetectTargetDir_ * SkillAtk_RushSpeed_ * _DeltaTime;
			float4 NextMovePos = GetTransform()->GetWorldPosition() + MoveSpeed;
			float Dist = 0.0f;
			if (true == CurrentMap_->GetNavMesh()->CheckIntersects(NextMovePos + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, Dist))
			{
				//NextMovePos.y = FT::Map::MAX_HEIGHT - Dist;
				GetTransform()->SetWorldPosition(NextMovePos);
			}
		}
		else
		{
			// �����Ϸ� -> ��ų���ݽ���
			
			// ��ų������ ���� �غ����� �ʱ�ȭ
			SkillAtk_DetectTargetPos_ = float4::ZERO;
			SkillAtk_DetectTargetDir_ = float4::ZERO;

			// ��ų�����غ� �ʱ�ȭ
			SkillAtkReady_ = false;
			SkillAtkReadyCollider_->Off();

			// ��ų���� �ʱ�ȭ
			SkillAtk_ = false;
			SkillAtkCollider_->Off();

			// �ǰݹ��ø� ���� Offó���ߴ� ��ü�浹ü On
			BodyCollider_->On();

			// �������� ������ ��ȯ
			ChangeAnimationAndState(MonsterStateType::IDLE);
			return;
		}
	}

	// ��ų���� �غ����
	if (false == SkillAtkReady_ && false == SkillAtk_)
	{
		// �����ð� ����
		SkillAtk_CastTime_ -= _DeltaTime;
		if (0.0f >= SkillAtk_CastTime_)
		{
			//================== ��ų�����غ�ó�� ����
			SkillAtkReady_ = true;
			SkillAtkReadyCollider_->On();
			MainRenderer_->ChangeFBXAnimation("SKILLATTACKREADY");				// ��ų�����غ�ִϸ��̼� ����
			
			// ���� Ÿ���� ���� ���� ���� �� Ÿ���� ��ġ�� ����
			float4 MyPosition = GetTransform()->GetWorldPosition();
			float4 TargetPosition = CurTarget_->GetTransform()->GetWorldPosition();
			SkillAtk_DetectTargetPos_ = TargetPosition;
			SkillAtk_DetectTargetDir_ = (TargetPosition - MyPosition).NormalizeReturn3D();

			// ���� ������ Ÿ���� ���� �������� ȸ��
			float4 Cross = float4::Cross3D(SkillAtk_DetectTargetDir_, { 0.0f, 0.0f, 1.0f });
			Cross.Normalize3D();
			float Angle = float4::DegreeDot3DToACosAngle(SkillAtk_DetectTargetDir_, { 0.0f, 0.0f, 1.0f });
			GetTransform()->SetLocalRotationDegree({ 0.0f, Angle * -Cross.y, 0.0f });

			// ��ų���� ����Ʈ������ On
			HitBoxRangeOn();

			// �����ð� �ʱ�ȭ
			SkillAtk_CastTime_ = SkillAtk_CastTimeMax_;
		}
	}

	// ��ų�����غ�Ϸ�
	if (true == SkillAtkReady_ && false == SkillAtk_)
	{
		if ("SKILLATTACKREADY" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
		{
			// ��ų�����غ� Flag ���� �� �浹ü Off
			SkillAtkReady_ = false;
			SkillAtkReadyCollider_->Off();

			// ��ų������ �����ϹǷ� ��ų���ݻ��¸� ����鼭
			SkillAtk_ = true;
			SkillAtkCollider_->On();
			MainRenderer_->ChangeFBXAnimation("SKILLATTACK");
		}
	}
}

void Boar::EndSkillAttackProcessing()
{
	// ���������� ���º��� or Ư���� ������ ���� ���º����
	// ��Ʈ�ڽ� ������ Off ó��
	HitBoxRangeOff();

	// �ǰݹ��ø� ���� Offó���ߴ� ��ü�浹ü On
	BodyCollider_->On();
}

void Boar::HitBoxRangeOn()
{
	// Renderer On
	HitBox_Frame_->On();
	HitBox_Bottom_->On();
}

void Boar::HitBoxRangeOff()
{
	// Renderer Off
	HitBox_Frame_->Off();
	HitBox_Bottom_->Off();
}

Boar::Boar()
	: SkillAtkReady_(false)
	, SkillAtk_(false)
	, SkillAtk_Range_(500.0f)
	, SkillAtk_CastTimeMax_(1.5f)
	, SkillAtk_CastTime_(1.5f)
	, SkillAtk_FixedDamage_(150.0f)
	, SkillAtk_RushSpeed_(400.0f)
	, SkillAtk_DetectTargetPos_(float4::ZERO)
	, SkillAtk_DetectTargetDir_(float4::ZERO)
	, HitBox_Frame_(nullptr)
	, HitBox_Bottom_(nullptr)
{
}

Boar::~Boar()
{
}
