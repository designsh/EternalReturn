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
	// 기본정보
	Type_ = MonsterType::BEAR;											// 몬스터타입

	//====================================== 스텟의 최대치(고정)
	StateInfo_.LevelMin_ = 5;											// 시작 레벨(리젠시 해당 데이터로 레벨초기화)
	StateInfo_.LevelMax_ = 24;											// 최대 레벨
	StateInfo_.HomingInstinctValueMax_ = 5.0f;							// 최대 귀소본능 수치
	StateInfo_.RegenTimeMax_ = 300.0f;									// 리젠타임
	StateInfo_.SkillCoolDownMax_ = 10.0f;								// 최대 스킬쿨타임

	//====================================== 레벨당 증가량(고정)
	StateInfo_.OffencePowerIncrement_ = 35;								// 레벨당 공격력 증가량
	StateInfo_.DefenseIncrement_ = 4;									// 레벨당 방어력 증가량
	StateInfo_.MoveSpeedIncrement_ = 0.03f;								// 레벨당 이동속도 증가량
	StateInfo_.HPIncrement_ = 120.0f;									// 레벨당 레벨당 체력 증가량

	//====================================== 기본스텟(고정)
	StateInfo_.NestPosition_ = float4::ZERO;							// 둥지위치(= 스폰위치)
	StateInfo_.AttackSpeed_ = 0.6f;										// 공격속도
	StateInfo_.AttackRange_ = 100.0f;									// 공격 시야거리
	StateInfo_.DetectRange_ = 300.0f;									// 감지 시야거리

	//====================================== 기본스텟(갱신) - 초기: 5레벨기준 스텟
	StateInfo_.Level_ = 5;												// 레벨(늦게 생성된 몬스터(야생동물)일수록 레벨이 높게 설정) - 생성과 동시에 지정(지정없이 생성된 몬스터의 경우 기본 1레벨고정)
	StateInfo_.OffencePower_ = 140;										// 공격력
	StateInfo_.Defense_ = 59;											// 방어력
	StateInfo_.HPMax_ = 1100.0f;										// 최대 체력(레벨당 변화)
	StateInfo_.HP_ = 1100.0f;											// 현재체력
	StateInfo_.MoveSpeed_ = 2.92f;										// 이동속도
	StateInfo_.HomingInstinctValue_ = 5.0f;								// 귀소본능 수치(0.0f보다 작거나같아지면 몬스터는 스폰위치로 이동하면 체력을 회복 -> 타겟이 DetectRange_에 벗어난 시간에 따라 감소)
	StateInfo_.RegenTime_ = 300.0f;										// 리젠타임(갱신) -> 0.0f이하일시 RegenTime_으로 초기화

	//====================================== 스킬관련
	StateInfo_.IsSkill_ = 1;											// 스킬여부(0: 스킬없음)
	StateInfo_.SkillCoolDown_ = 10.0f;									// 스킬재사용시간

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

void Bear::InitalizeResourceLoad()
{
	// 리소스 로드는 한번만 처리
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
	//============================== 렌더러 셋팅

	// 렌더러 셋팅
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Bear_BaseMesh.fbx", "TextureDeferredLightAni");

	PivotPos_ = float4(-380.0f, 0.0f);
	MainRenderer_->GetTransform()->SetLocalPosition(PivotPos_);									// 기존의 메쉬자체가 피벗이 액터의 원점을 벗어나있으므로 조정
	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	// 애니메이션 생성
	MainRenderer_->CreateFBXAnimation("APPEAR", "Bear_appear.UserAnimation", 0, false);			// 첫등장상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("REGEN", "Bear_appear.UserAnimation", 0, false);			// 리젠상태(몬스터 사망 후 리젠타임에 의해 리젠한 상태)의 애니메이션
	MainRenderer_->CreateFBXAnimation("IDLE", "Bear_wait.UserAnimation", 0);					// 대기상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("CHASE", "Bear_run.UserAnimation", 0);					// 추적상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Bear_run.UserAnimation", 0);			// 귀환상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("HIT", "Bear_wait.UserAnimation", 0);						// 피격상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEATH", "Bear_death.UserAnimation", 0, false);			// 사망중상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEAD", "Bear_death.UserAnimation", 0, false);			// 사망(리젠대기)상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("ATK01", "Bear_atk01.UserAnimation", 0, false);			// 일반공격01상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("ATK02", "Bear_atk02.UserAnimation", 0, false);			// 일반공격02상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Bear_skill.UserAnimation", 0, false);		// 스킬공격상태의 애니메이션
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

void Bear::InitalizeCollider()
{
	// 기본: 몸체 충돌체 생성
	BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	BodyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition() - PivotPos_);			// 피벗 적용

	// 추가: 공격 충돌체 생성(옵션)
	AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	AtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition() - PivotPos_);			// 피벗 적용
	AtkCollider_->Off();

	// 추가: 스킬공격 충돌체 생성(옵션)
	SkillAtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	SkillAtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	SkillAtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	SkillAtkCollider_->GetTransform()->SetLocalScaling({ 400.0f, 400.0f, 400.0f });												// 사정거리 - 4m = 400.0f로 측정
	SkillAtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition() - PivotPos_);			// 피벗 적용
	SkillAtkCollider_->Off();
}

void Bear::StartSkillAttackProcessing()
{
}

void Bear::UpdateSkillAttackProcessing(float _DeltaTime)
{
	// 지면강타 : 주변범위에 피해를 입히고 1초간 기절
	// 피해량 - 기본(170) + 공격력의 40%
	// 사정거리 - 4m
	// 시전시간 - 0.5초
	// 쿨다운 - 10초
	// 1m == 100.0f로 계산
	if (false == SkillAtk_)
	{
		SkillAtk_CastTime_ -= _DeltaTime;
		if (0.0f >= SkillAtk_CastTime_)
		{
			// 스킬공격 애니메이션 실행
			MainRenderer_->ChangeFBXAnimation("SKILLATTACK");

			// 스킬공격처리를 위한 준비
			SkillAtk_ = true;

			// 스킬공격충돌체 On
			SkillAtkCollider_->On();

			// 스킬공격충돌체와 충돌한 모든 캐릭터 검사
			std::list<GameEngineCollision*> SkillAtkList = SkillAtkCollider_->GetCollisionList(static_cast<int>(eCollisionGroup::Player));
			if (true == SkillAtkList.empty())
			{
				GameEngineDebug::OutPutDebugString("스킬을 시전했으나 충돌한 캐릭터가 존재하지않습니다!!!!\n");
			}
			else
			{
				// 충돌한 모든 캐릭터에게 스턴 및 데미지 전달
				for (auto& AtkTarget : SkillAtkList)
				{
					Character* AtkCharacter = dynamic_cast<Character*>(AtkTarget->GetActor());

					// 스턴
					AtkCharacter->Stun(SkillAtk_StunTime_);

					// 스턴패킷전송
					CharCrowdControlPacket ccPacket;
					ccPacket.SetTargetIndex(AtkCharacter->GetIndex());
					ccPacket.SetStun(SkillAtk_StunTime_);
					FT::SendPacket(ccPacket);

					// 데미지
					float CurDamage = SkillAtk_FixedDamage_ + (static_cast<float>(StateInfo_.OffencePower_) * 0.4f);
					AtkCharacter->Damage(CurDamage, this);
				}
			}

			// 시전시간 초기화
			SkillAtk_CastTime_ = SkillAtk_CastTimeMax_;
		}
	}

	if (true == SkillAtk_)
	{
		// 모션종료시 
		if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
		{
			// 스킬공격 끝
			SkillAtk_ = false;
			SkillAtkCollider_->Off();

			// 대기상태로 전환
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
