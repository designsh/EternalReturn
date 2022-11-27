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
	// 기본정보
	Type_ = MonsterType::BOAR;											// 몬스터타입

	//====================================== 스텟의 최대치(고정)
	StateInfo_.LevelMin_ = 2;											// 시작 레벨(리젠시 해당 데이터로 레벨초기화)
	StateInfo_.LevelMax_ = 12;											// 최대 레벨
	StateInfo_.HomingInstinctValueMax_ = 5.0f;							// 최대 귀소본능 수치
	StateInfo_.RegenTimeMax_ = 195.0f;									// 리젠타임
	StateInfo_.SkillCoolDownMax_ = 7.0f;								// 최대 스킬쿨타임

	//====================================== 레벨당 증가량(고정)
	StateInfo_.OffencePowerIncrement_ = 23;								// 레벨당 공격력 증가량
	StateInfo_.DefenseIncrement_ = 7;									// 레벨당 방어력 증가량
	StateInfo_.MoveSpeedIncrement_ = 0.05f;								// 레벨당 이동속도 증가량
	StateInfo_.HPIncrement_ = 167.0f;									// 레벨당 레벨당 체력 증가량

	//====================================== 기본스텟(고정)
	StateInfo_.NestPosition_ = float4::ZERO;							// 둥지위치(= 스폰위치)
	StateInfo_.AttackSpeed_ = 0.5f;										// 공격속도
	StateInfo_.AttackRange_ = 100.0f;									// 공격 시야거리
	StateInfo_.DetectRange_ = 300.0f;									// 감지 시야거리

	//====================================== 기본스텟(갱신) - 초기: 2레벨기준 스텟
	StateInfo_.Level_ = 2;												// 레벨(늦게 생성된 몬스터(야생동물)일수록 레벨이 높게 설정) - 생성과 동시에 지정(지정없이 생성된 몬스터의 경우 기본 1레벨고정)
	StateInfo_.OffencePower_ = 51;										// 공격력
	StateInfo_.Defense_ = 16;											// 방어력
	StateInfo_.HPMax_ = 942.0f;											// 최대 체력(레벨당 변화)
	StateInfo_.HP_ = 942.0f;											// 현재체력
	StateInfo_.MoveSpeed_ = 2.85f;										// 이동속도
	StateInfo_.HomingInstinctValue_ = 5.0f;								// 귀소본능 수치(0.0f보다 작거나같아지면 몬스터는 스폰위치로 이동하면 체력을 회복 -> 타겟이 DetectRange_에 벗어난 시간에 따라 감소)
	StateInfo_.RegenTime_ = 195.0f;										// 리젠타임(갱신) -> 0.0f이하일시 RegenTime_으로 초기화

	//====================================== 스킬관련
	StateInfo_.IsSkill_ = 1;											// 스킬여부(0: 스킬없음)
	StateInfo_.SkillCoolDown_ = 7.0f;									// 스킬재사용시간

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

void Boar::InitalizeResourceLoad()
{
	// 리소스 로드는 한번만 처리
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
	//============================= 메인렌더러 셋팅
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Boar_BaseMesh.fbx", "TextureDeferredLightAni");
	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	//============================= 메인렌더러 애니메이션 생성
	MainRenderer_->CreateFBXAnimation("APPEAR", "Boar_appear.UserAnimation", 0, false);					// 첫등장상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("REGEN", "Boar_appear.UserAnimation", 0, false);					// 리젠상태(몬스터 사망 후 리젠타임에 의해 리젠한 상태)의 애니메이션
	MainRenderer_->CreateFBXAnimation("IDLE", "Boar_wait.UserAnimation", 0);							// 대기상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("CHASE", "Boar_run.UserAnimation", 0);							// 추적상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Boar_run.UserAnimation", 0);					// 귀환상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("HIT", "Boar_wait.UserAnimation", 0);								// 피격상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEATH", "Boar_death.UserAnimation", 0, false);					// 사망중상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEAD", "Boar_death.UserAnimation", 0, false);					// 사망(리젠대기)상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("ATK01", "Boar_atk01.UserAnimation", 0, false);					// 일반공격01상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("ATK02", "Boar_atk02.UserAnimation", 0, false);					// 일반공격02상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("SKILLATTACKREADY", "Boar_skill_ready.UserAnimation", 0, false);	// 스킬시전준비상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Boar_skill_assault.UserAnimation", 0);			// 스킬시전중상태의 애니메이션(루프실행 : 타겟과 충돌할때까지 실행해야하기때문에)

	MainRenderer_->ChangeFBXAnimation("IDLE");
	MainRenderer_->LightShadowOn();

	//============================= 기본상태 셋팅
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
	// 기본: 몸체 충돌체 생성
	BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	BodyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());

	// 추가: 일반공격 충돌체 생성(옵션)
	AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	AtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	AtkCollider_->Off();

	// 추가: 스킬공격개시 충돌체 생성(옵션)
	SkillAtkReadyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	SkillAtkReadyCollider_->SetCollisionGroup(eCollisionGroup::MonsterSight);
	SkillAtkReadyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	SkillAtkReadyCollider_->GetTransform()->SetLocalScaling({ 500.0f, 500.0f, 500.0f });
	SkillAtkReadyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	SkillAtkReadyCollider_->Off();

	// 추가: 스킬공격 충돌체 생성(옵션)
	SkillAtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	SkillAtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	SkillAtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	SkillAtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	SkillAtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	SkillAtkCollider_->Off();
}

void Boar::StartSkillAttackProcessing()
{
	// 스킬시전중 피격처리무시를 위해 몸체 충돌체 Off
	BodyCollider_->Off();
}

void Boar::UpdateSkillAttackProcessing(float _DeltaTime)
{
	// 돌진 : 일직선으로 돌격하며 적을 밀어내며 피해를 입힘
	//		  단, 적이 벽에 부딪혔다면 기절
	// 피해량 - 기본(150) + 공격력의 300%
	// 사정거리 - 5m
	// 시전시간 - 1.5초
	// 쿨다운 - 7초
	// 1m == 100.0f로 계산
	// 스킬공격준비완료, 스킬공격시작
	if (false == SkillAtkReady_ && true == SkillAtk_)
	{
		// 스킬공격 시작했으므로 이펙트렌더러 Off
		HitBoxRangeOff();

		// 타겟위치까지 돌진하며 타겟과 충돌시 타겟에게 넉백, 데미지를 입히고 스킬공격성공으로 대기상태로 전환
		// 충돌실패시 스킬공격실패로 대기상태로 전환

		// 충돌체크
		std::list<GameEngineCollision*> SkillAtkList = SkillAtkCollider_->GetCollisionList(static_cast<int>(eCollisionGroup::Player));
		if (false == SkillAtkList.empty())
		{
			// 충돌한 캐릭터들 중 타겟이 존재하면 넉백 및 데미지
			for (auto& AtkTarget : SkillAtkList)
			{
				Character* AtkCharacter = dynamic_cast<Character*>(AtkTarget->GetActor());
				if (AtkCharacter->GetIndex() == CurTargetIndex_)
				{
					// 넉백(적이 벽에 부딪혔다면 기절)
					AtkCharacter->WallSlam(0.1f, SkillAtk_DetectTargetDir_ * SkillAtk_RushSpeed_, 1.0f);

					// 넉백 패킷전송
					CharCrowdControlPacket ccPacket;
					ccPacket.SetTargetIndex(AtkCharacter->GetIndex());
					ccPacket.SetWallSlam(0.1f, SkillAtk_DetectTargetDir_ * SkillAtk_RushSpeed_, 1.0f);
					FT::SendPacket(ccPacket);

					// 데미지(기본(150) + 공격력의 300%)
					float CurDamage = SkillAtk_FixedDamage_ + (static_cast<float>(StateInfo_.OffencePower_) * 3.0f);
					AtkCharacter->Damage(CurDamage, this);

					//======================== 스킬공격완료
					// 스킬공격준비 초기화
					SkillAtkReady_ = false;
					SkillAtkReadyCollider_->Off();

					// 스킬공격 초기화
					SkillAtk_ = false;
					SkillAtkCollider_->Off();

					// 모션종료시 대기상태 전환
					ChangeAnimationAndState(MonsterStateType::IDLE);

					return;
				}
			}
		}

		// 충돌하지않았다면 돌진 or 돌진완료체크
		if ((SkillAtk_DetectTargetPos_ - GetTransform()->GetWorldPosition()).Len3D() > 10.0f)
		{
			// 돌진중
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
			// 돌진완료 -> 스킬공격실패
			
			// 스킬공격을 위한 준비데이터 초기화
			SkillAtk_DetectTargetPos_ = float4::ZERO;
			SkillAtk_DetectTargetDir_ = float4::ZERO;

			// 스킬공격준비 초기화
			SkillAtkReady_ = false;
			SkillAtkReadyCollider_->Off();

			// 스킬공격 초기화
			SkillAtk_ = false;
			SkillAtkCollider_->Off();

			// 피격무시를 위해 Off처리했던 몸체충돌체 On
			BodyCollider_->On();

			// 모션종료시 대기상태 전환
			ChangeAnimationAndState(MonsterStateType::IDLE);
			return;
		}
	}

	// 스킬공격 준비시작
	if (false == SkillAtkReady_ && false == SkillAtk_)
	{
		// 시전시간 감소
		SkillAtk_CastTime_ -= _DeltaTime;
		if (0.0f >= SkillAtk_CastTime_)
		{
			//================== 스킬공격준비처리 시작
			SkillAtkReady_ = true;
			SkillAtkReadyCollider_->On();
			MainRenderer_->ChangeFBXAnimation("SKILLATTACKREADY");				// 스킬시전준비애니메이션 실행
			
			// 현재 타겟을 향한 나의 방향 및 타겟의 위치가 결정
			float4 MyPosition = GetTransform()->GetWorldPosition();
			float4 TargetPosition = CurTarget_->GetTransform()->GetWorldPosition();
			SkillAtk_DetectTargetPos_ = TargetPosition;
			SkillAtk_DetectTargetDir_ = (TargetPosition - MyPosition).NormalizeReturn3D();

			// 나의 방향을 타겟을 향한 방향으로 회전
			float4 Cross = float4::Cross3D(SkillAtk_DetectTargetDir_, { 0.0f, 0.0f, 1.0f });
			Cross.Normalize3D();
			float Angle = float4::DegreeDot3DToACosAngle(SkillAtk_DetectTargetDir_, { 0.0f, 0.0f, 1.0f });
			GetTransform()->SetLocalRotationDegree({ 0.0f, Angle * -Cross.y, 0.0f });

			// 스킬공격 이펙트렌더러 On
			HitBoxRangeOn();

			// 시전시간 초기화
			SkillAtk_CastTime_ = SkillAtk_CastTimeMax_;
		}
	}

	// 스킬공격준비완료
	if (true == SkillAtkReady_ && false == SkillAtk_)
	{
		if ("SKILLATTACKREADY" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
		{
			// 스킬공격준비 Flag 해제 및 충돌체 Off
			SkillAtkReady_ = false;
			SkillAtkReadyCollider_->Off();

			// 스킬공격이 가능하므로 스킬공격상태를 만들면서
			SkillAtk_ = true;
			SkillAtkCollider_->On();
			MainRenderer_->ChangeFBXAnimation("SKILLATTACK");
		}
	}
}

void Boar::EndSkillAttackProcessing()
{
	// 정상동작으로 상태변경 or 특정한 이유로 강제 상태변경시
	// 히트박스 렌더러 Off 처리
	HitBoxRangeOff();

	// 피격무시를 위해 Off처리했던 몸체충돌체 On
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
