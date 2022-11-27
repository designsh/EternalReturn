#include "PreCompile.h"
#include "Dog.h"

#include <GameEngine/GameEngineCollision.h>

bool Dog::ResourceLoadFlag = false;

void Dog::InitalizeStateInfo()
{
	// 기본정보
	Type_ = MonsterType::DOG;											// 몬스터타입

	//====================================== 스텟의 최대치(고정)
	StateInfo_.LevelMin_ = 3;											// 시작 레벨(리젠시 해당 데이터로 레벨초기화)
	StateInfo_.LevelMax_ = 17;											// 최대 레벨
	StateInfo_.HomingInstinctValueMax_ = 5.0f;							// 최대 귀소본능 수치
	StateInfo_.RegenTimeMax_ = 180.0f;									// 리젠타임
	StateInfo_.SkillCoolDownMax_ = 10.0f;								// 최대 스킬쿨타임

	//====================================== 레벨당 증가량(고정)
	StateInfo_.OffencePowerIncrement_ = 18;								// 레벨당 공격력 증가량
	StateInfo_.DefenseIncrement_ = 4;									// 레벨당 방어력 증가량
	StateInfo_.MoveSpeedIncrement_ = 0.06f;								// 레벨당 이동속도 증가량
	StateInfo_.HPIncrement_ = 82.0f;									// 레벨당 레벨당 체력 증가량

	//====================================== 기본스텟(고정)
	StateInfo_.NestPosition_ = float4::ZERO;							// 둥지위치(= 스폰위치)
	StateInfo_.AttackSpeed_ = 0.9f;										// 공격속도
	StateInfo_.AttackRange_ = 100.0f;									// 공격 시야거리
	StateInfo_.DetectRange_ = 300.0f;									// 감지 시야거리

	//====================================== 기본스텟(갱신) - 초기: 3레벨기준 스텟
	StateInfo_.Level_ = 3;												// 레벨(늦게 생성된 몬스터(야생동물)일수록 레벨이 높게 설정) - 생성과 동시에 지정(지정없이 생성된 몬스터의 경우 기본 1레벨고정)
	StateInfo_.OffencePower_ = 36;										// 공격력
	StateInfo_.Defense_ = 29;											// 방어력
	StateInfo_.HPMax_ = 564.0f;											// 최대 체력(레벨당 변화)
	StateInfo_.HP_ = 564.0f;											// 현재체력
	StateInfo_.MoveSpeed_ = 2.9f;										// 이동속도
	StateInfo_.HomingInstinctValue_ = 5.0f;								// 귀소본능 수치(0.0f보다 작거나같아지면 몬스터는 스폰위치로 이동하면 체력을 회복 -> 타겟이 DetectRange_에 벗어난 시간에 따라 감소)
	StateInfo_.RegenTime_ = 180.0f;										// 리젠타임(갱신) -> 0.0f이하일시 RegenTime_으로 초기화

	//====================================== 스킬관련
	StateInfo_.IsSkill_ = 0;											// 스킬여부(0: 스킬없음)
	StateInfo_.SkillCoolDown_ = 0.5f;									// 스킬재사용시간

	//====================================== 아이템관련
	StateInfo_.DropItems_[ItemName::MEAT] = 40.0f;
	StateInfo_.DropItems_[ItemName::LEATHER] = 60.0f;

	if (true == GameServer::GetInstance()->IsOpened())					// 서버이면 아이템정보 수집
	{
		// 일단 고정
		StateInfo_.CurDropItems_.push_back(ItemName::MEAT);
		StateInfo_.CurDropItems_.push_back(ItemName::LEATHER);
	}
}

void Dog::InitalizeResourceLoad()
{
	// 리소스 로드는 한번만 처리
	if (false == ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("EternalReturn");
		MeshDir / "Resources" / "FBX" / "Monster" / "Dog";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Dog_BaseMesh.fbx"));
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

void Dog::InitalizeRenderAndAnimation()
{
	//============================== 렌더러 셋팅

	// 렌더러 셋팅
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Dog_BaseMesh.fbx", "TextureDeferredLightAni");

	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });

	// 애니메이션 생성
	MainRenderer_->CreateFBXAnimation("APPEAR", "Dog_appear.UserAnimation", 0, false);			// 첫등장상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("REGEN", "Dog_appear.UserAnimation", 0, false);				// 리젠상태(몬스터 사망 후 리젠타임에 의해 리젠한 상태)의 애니메이션
	MainRenderer_->CreateFBXAnimation("IDLE", "Dog_wait.UserAnimation", 0);						// 대기상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("CHASE", "Dog_run.UserAnimation", 0);						// 추적상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Dog_run.UserAnimation", 0);				// 귀환상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("HIT", "Dog_wait.UserAnimation", 0);						// 피격상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEATH", "Dog_death.UserAnimation", 0, false);				// 사망중상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEAD", "Dog_death.UserAnimation", 0, false);				// 사망(리젠대기)상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("ATK01", "Dog_atk01.UserAnimation", 0, false);				// 일반공격01상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("ATK02", "Dog_atk02.UserAnimation", 0, false);				// 일반공격02상태의 애니메이션
	MainRenderer_->ChangeFBXAnimation("IDLE");
	MainRenderer_->LightShadowOn();

	// 기본상태 셋팅
	ChangeAnimationAndState(MonsterStateType::APPEAR);

	//============================== 선처리 렌더러 셋팅

	// 외곽선
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

	// 실루엣
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

void Dog::InitalizeCollider()
{
	// 기본: 몸체 충돌체 생성
	BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	BodyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());

	// 추가: 공격 충돌체 생성(옵션)
	AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	AtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	AtkCollider_->Off();
}

void Dog::StartSkillAttackProcessing()
{
}

void Dog::UpdateSkillAttackProcessing(float _DeltaTime)
{
	// 스킬없으므로 스킬공격모션 종료시 대기상태로 전환
	if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		// 모션종료시 대기상태 전환
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Dog::EndSkillAttackProcessing()
{
}

Dog::Dog()
{
}

Dog::~Dog()
{
}
