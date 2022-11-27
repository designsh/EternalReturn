#include "PreCompile.h"
#include "Hyunwoo.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineCollision.h>
#include "CharStatPacket.h"
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include "GameServer.h"
#include "GameClient.h"
#include <GameEngine/GameEngineLevelControlWindow.h>
#include "MousePointer.h"
#include "LumiaMap.h"
#include "Character.h"
#include "CharCrowdControlPacket.h"
#include "PacketSoundPlay.h"
#include "CharEffectPacket.h"
#include "HyunwooQEffect.h"
#include "HyunwooEffect.h"
#include "HyunwooWEffect.h"
#include "HyunwooREffect.h"
#include "SlowEffect.h"
#include "MonsterCrowdControlPacket.h"
#include "Monsters.h"
#include "UI_DamagePopUp.h"
#include "PlayerUIController.h"

#include "UI_SkillGauge.h"
Hyunwoo::Hyunwoo()
	: timer_collision_Q(0.0f), timer_end_Q(0.0f), collision_Q(nullptr), b_Qhit_(false), timer_Dash_E(0.0f), b_Ehit_(false), collision_E(nullptr), atkFlag_(false),
	  b_Rhit_(false), collision_R(nullptr), collisionRRate_(0.0f), b_Dhit_(false), basicAttackEffect_(nullptr), QGroundCrackEffectRenderer_(nullptr), qEffect_(nullptr),
	rEffect_(nullptr), b_isW_(false), wEffect_(nullptr), rTimer_(0.0f), rShot_(false)
{

}

Hyunwoo::~Hyunwoo()
{

}

void Hyunwoo::LoadResource()
{
	static bool a = false;
	if (a)
	{
		return;
	}

	GameEngineDirectory dir;

	dir.MoveParent("EternalReturn");
	dir / "Resources" / "FBX" / "Character" / "Hyunwoo";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_run.fbx"));
	mesh->CreateRenderingBuffer();

	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_run.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_wait.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_death.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_atk0.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_atk1.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_skillq.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_skille_start.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_skille_loop.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_skille_end.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_skillr_start.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_skillr_loop.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_skillr_end.fbx"));
	//GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_weaponskill.fbx"));

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
	}

	{
		GameEngineDirectory dir;
		dir.MoveParent("EternalReturn");
		dir / "Resources" / "Sound" / "Char" / "Hyunwoo";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;

		dir.MoveParent("EternalReturn");
		dir / "Resources" / "Texture" / "Hyunwoo";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("png");
		for (GameEngineFile& file : allFile)
		{
			GameEngineTextureManager::GetInst().Load(file.FileName(), file.GetFullPath());
		}
	}


	{


		GameEngineTexture* rearBase = GameEngineTextureManager::GetInst().Find("FX_BI_WindDust_01SE1.png");
		rearBase->Cut(3, 8);


	}
	a = true;
}

void Hyunwoo::ReleaseResource()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("EternalReturn");
		dir / "Resources" / "Sound" / "Char" / "Hyunwoo";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		}
	}

	GameEngineFBXMeshManager::GetInst().Delete("Hyunwoo_run.fbx");

	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_run.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_wait.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_death.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_atk0.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_atk1.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_skillq.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_skille_start.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_skille_loop.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_skille_end.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_skillr_start.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_skillr_loop.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_skillr_end.fbx");
	//GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_weaponskill.fbx");

	{
		GameEngineDirectory dir;
		dir.MoveParent("EternalReturn");
		dir / "Resources" / "FBX" / "Character" / "Hyunwoo";
	
		std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
		}
	}
}

void Hyunwoo::Start()
{
	Character::Start();
	initHyunwooCollision();
	initHyunwooCustomState();
	initEffectRenderer();

	stat_.HPMax = 820.0f;
	stat_.HP = 820.0f;
	stat_.SPMax = 350.0f;
	stat_.SP = 350.0f;
	stat_.Defence = 28.0f;
	stat_.AttackPower = 40.0f;
	stat_.HPRegeneration = 1.8f;
	stat_.SPRegeneration = 1.8f;

	stat_.AttackSpeed = 0.86f;
	stat_.AttackStartTime = 0.15f;
	stat_.AttackEndTime = 0.4f;
	stat_.MovementSpeed = 355.0f;
	stat_.AttackRange = 280.f;

	//stat_.Cooltime_q = 8.5f;
	//stat_.Cooltime_w = 28.0f;
	//stat_.Cooltime_e = 18.0f;
	//stat_.Cooltime_r = 65.0f;
	//stat_.Cooltime_d = 10.0f;

	DefaultCool_Q_ = 8.5f;
	DefaultCool_W_ = 28.0f;
	DefaultCool_E_ = 18.0f;
	DefaultCool_R_ = 65.0f;
	DefaultCool_D_ = 10.0f;

	stat_.SP_Q = 50.0f;
	stat_.SP_W = 50.0f;
	stat_.SP_E = 70.0f;
	stat_.SP_R = 150.0f;

	SetEquipBuildItem("ImperialBurgonet", EquipmentType::HEAD);
	SetEquipBuildItem("CommandersArmor", EquipmentType::CHEST);
	SetEquipBuildItem("TindalosBand", EquipmentType::ARM);
	SetEquipBuildItem("TachyonBrace", EquipmentType::LEG);
	SetEquipBuildItem("WhiteCraneFan", EquipmentType::ACCESSORY);
	SetEquipBuildItem("PlasmaTonfa", EquipmentType::WEAPON);

	checkBuildItems();
}

void Hyunwoo::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

	static bool getWeapon = false;

	if (nullptr != uiController_ && false == getWeapon)
	{
		getItem("Bamboo");
		getWeapon = true;
	}

	if (true == b_isW_)
	{
		timer_w_ -= _deltaTime;
		timer_unstoppable_ -= _deltaTime;

		if (0.0f >= timer_unstoppable_)
		{
			stat_.isUnstoppable_ = false;

		}

		if (0.0f >= timer_w_)
		{
			timer_w_ = 2.5f;
			timer_unstoppable_ = 1.0f;
			b_isW_ = false;
		}
	}
}

void Hyunwoo::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::CHARACTER));
	renderer_->SetFBXMesh("hyunwoo_run.fbx", "TextureDeferredLightAni", true);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Hyunwoo_run.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Wait", "Hyunwoo_wait.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Death", "Hyunwoo_death.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Atk0", "Hyunwoo_atk0.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillD", "Hyunwoo_weaponskill.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Atk1", "Hyunwoo_atk1.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "Hyunwoo_skillq.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillE_start", "Hyunwoo_skille_start.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillE_loop", "Hyunwoo_skille_loop.UserAnimation", 0);
	renderer_->CreateFBXAnimation("SkillE_end", "Hyunwoo_skille_end.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillR_start", "Hyunwoo_skillR_start.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillR_loop", "Hyunwoo_skillR_loop.UserAnimation", 0);
	renderer_->CreateFBXAnimation("SkillR_end", "Hyunwoo_skillR_end.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("Craft", "Hyunwoo_Craft.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Operate", "Hyunwoo_Operate.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Arrive", "Hyunwoo_Arrive.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("RestStart", "Hyunwoo_Rest_Start.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("RestLoop", "Hyunwoo_Rest_Loop.UserAnimation", 0, true);
	renderer_->CreateFBXAnimation("RestEnd", "Hyunwoo_Rest_End.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("Collect", "Hyunwoo_Collect.UserAnimation", 0, true);

	renderer_->ChangeFBXAnimation("Wait");
	renderer_->LightShadowOn();

	renderer_->GetRenderSet(1).isRender = false;

	// 외곽선
	{
		// Main
		MainOutLineRenderer_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		MainOutLineRenderer_->SetBaseRenderer(renderer_, "PreprocessingAni", true, false);

		OutLineData MainOutLineData = MainOutLineRenderer_->GetOutLineData();
		float4 MainThickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * MainOutLineData.LineThickness;
		float4 MainCalcScale = renderer_->GetTransform()->GetLocalScaling() + MainThickness;
		MainOutLineRenderer_->GetTransform()->SetLocalScaling(MainCalcScale);
		MainOutLineRenderer_->GetTransform()->SetLocalRotationDegree(renderer_->GetTransform()->GetLocalRotation());
		MainOutLineRenderer_->GetTransform()->SetLocalPosition(renderer_->GetTransform()->GetLocalPosition());
		MainOutLineRenderer_->GetRenderSet(1).isRender = false;
		MainOutLineRenderer_->Off();
	}

	// 실루엣
	{
		// Main
		MainSilhouetteRenderer_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		MainSilhouetteRenderer_->SetBaseRenderer(renderer_, "PreprocessingAni", true);

		OutLineData MainOutLineData = MainSilhouetteRenderer_->GetOutLineData();
		float4 MainThickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * MainOutLineData.LineThickness;
		float4 MainCalcScale = renderer_->GetTransform()->GetLocalScaling() + MainThickness;
		MainSilhouetteRenderer_->GetTransform()->SetLocalScaling(MainCalcScale);
		MainSilhouetteRenderer_->GetTransform()->SetLocalRotationDegree(renderer_->GetTransform()->GetLocalRotation());
		MainSilhouetteRenderer_->GetTransform()->SetLocalPosition(renderer_->GetTransform()->GetLocalPosition());
		MainSilhouetteRenderer_->GetRenderSet(1).isRender = false;
	}
}


void Hyunwoo::initHyunwooCollision()
{
	collision_Q = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_Q->GetTransform()->SetLocalPosition({ 0.f,0.f,300.f });
	collision_Q->GetTransform()->SetLocalScaling({ 450.0f, 10.0f, 300.0f });
	collision_Q->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_Q->SetCollisionType(CollisionType::OBBBox3D);
	collision_Q->Off();

	collision_E = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_E->GetTransform()->SetLocalPosition({ 0.f,0.f,0.f });
	collision_E->GetTransform()->SetLocalScaling({ 150.f });
	collision_E->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_E->SetCollisionType(CollisionType::OBBBox3D);
	collision_E->Off();

	collision_R = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_R->GetTransform()->SetLocalPosition({ 0.f,0.f,150.f });
	collision_R->GetTransform()->SetLocalScaling({200.0f, 10.0f, 100.0f});
	collision_R->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_R->SetCollisionType(CollisionType::OBBBox3D);
	collision_R->Off();
}

void Hyunwoo::initHyunwooCustomState()
{
	customState_.CreateState(MakeStateWithEnd(Hyunwoo, CustomRSkill));

	customState_ << "CustomRSkill";
}

void Hyunwoo::initEffectRenderer()
{
	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_Hit_08.png");
	hitBase->Cut(3, 3);

	basicAttackEffect_ = GetLevel()->CreateActor<BasicAttackEffect>();
	basicAttackEffect_->GetAttackRenderer()->SetImage("FX_BI_Hit_08.png", "PointSmp");
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalRotationDegree({ 90.0f,0.0f,0.0f });
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalScaling(basicAttackEffect_->GetAttackRenderer()->GetCurrentTexture()->GetTextureSize() / 3);
	basicAttackEffect_->GetAttackRenderer()->CreateAnimation("FX_BI_Hit_08.png", "FX_BI_Hit_08", 0, 8, 0.03f, false);

	QGroundCrackEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	QGroundCrackEffectRenderer_->SetImage("FX_BI_GroundBomb_01.png", "PointSmp");
	QGroundCrackEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 100.0f });
	QGroundCrackEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	QGroundCrackEffectRenderer_->GetTransform()->SetLocalScaling(QGroundCrackEffectRenderer_->GetCurrentTexture()->GetTextureSize());
	QGroundCrackEffectRenderer_->Off();

	rearEffectRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());

	rearEffectRenderer_->SetImage("FX_BI_WindDust_01SE1.png", "PointSmp");
	rearEffectRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 100.0f, -180.0f });
	rearEffectRenderer_->GetTransform()->SetLocalRotationDegree({ 0.f,90.f,0.f });
	rearEffectRenderer_->GetTransform()->SetLocalScaling(rearEffectRenderer_->GetCurrentTexture()->GetTextureSize() / 3);
	rearEffectRenderer_->CreateAnimation("FX_BI_WindDust_01SE1.png", "FX_BI_WindDust_01SE", 0, 23, 0.02f, false);
	rearEffectRenderer_->Off();


	qEffect_ = GetLevel()->CreateActor<HyunwooQEffect>();
	qEffect_->SetParent(this);

	wEffect_ = GetLevel()->CreateActor<HyunwooWEffect>();
	wEffect_->SetParent(this);

	rEffect_ = GetLevel()->CreateActor<HyunwooREffect>();
	rEffect_->SetParent(this);
	rEffect_->SetMyCharacter(this);
}


void Hyunwoo::changeAnimationRun()
{
	curAnimationName_ = "Run";
	renderer_->ChangeFBXAnimation("Run");
}

void Hyunwoo::changeAnimationWait()
{
	curAnimationName_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Hyunwoo::changeAnimationBasicAttack()
{
	if (false == atkFlag_)
	{
		curAnimationName_ = "Atk0";
		renderer_->ChangeFBXAnimation("Atk0", true);
		atkFlag_ = true;
	}
	else
	{
		curAnimationName_ = "Atk1";
		renderer_->ChangeFBXAnimation("Atk1", true);
		atkFlag_ = false;
	}

	GameEngineSoundManager::GetInstance()->PlaySoundByName("attackGlove_Normal01.wav");
	PacketSoundPlay packet;
	packet.SetSound("attackGlove_Normal01.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

}

void Hyunwoo::changeDeathAnimation()
{
	ChangeAnimation("Death", true);
	//curAnimationName_ = "Death";
	//renderer_->ChangeFBXAnimation("Death", true);
}

void Hyunwoo::changeAnimationOperate()
{
	ChangeAnimation("Operate");
}

void Hyunwoo::changeAnimationArrive()
{
	ChangeAnimation("Arrive");
}

void Hyunwoo::changeAnimationRestStart()
{
	ChangeAnimation("RestStart");
}

void Hyunwoo::changeAnimationRestLoop()
{
	ChangeAnimation("RestLoop");
}

void Hyunwoo::changeAnimationRestEnd()
{
	ChangeAnimation("RestEnd");
}

void Hyunwoo::changeAnimationCraft()
{
	ChangeAnimation("Craft");
}

void Hyunwoo::changeAnimationGather()
{
	ChangeAnimation("Collect");
}

void Hyunwoo::onStartQSkill()
{

	timer_collision_Q = 0.0f;
	timer_end_Q = 0.0f;
	b_Qhit_ = false;

	setRotationToMouse();

	curAnimationName_ = "SkillQ";

	renderer_->ChangeFBXAnimation("SkillQ", true);
	uiController_->GetSkillGauge()->PlayAwake(0.3f, "시전 중");
	RandomSoundPlay("Hyunwoo_PlaySkill1007200Seq0_1_ko.wav", "Hyunwoo_PlaySkill1007200Seq0_2_ko.wav", "Hyunwoo_PlaySkill1007200Seq0_3_ko.wav");
}

void Hyunwoo::onUpdateQSkill(float _deltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager().GetInstance();

	if (true == collision_Q->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_Q->GetTransform(), CollisionType::OBBBox3D, float4::BLUE);
	}

	timer_collision_Q += _deltaTime;
	timer_end_Q += _deltaTime;

	if (true == b_Qhit_)
	{
		collision_Q->Off();
	}

	if (1.05f <= timer_end_Q)
	{
		// 모든 카운터 초기화
		timer_collision_Q = 0.0f;
		timer_end_Q = 0.0f;
		b_Qhit_ = false;
		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
		return;
	}

	if (0.3f <= timer_collision_Q && false == b_Qhit_)
	{
		// 여기서 피격 충돌 판정이 나옴
		collision_Q->On();

		FT::PlaySoundAndSendPacket("hyunwoo_Skill01_Hit.wav", transform_.GetWorldPosition());
		
		if (GameServer::GetInstance()->IsOpened())
		{
			float4 wp = GetTransform()->GetWorldPosition();
			qEffect_->GetTransform()->SetLocalPosition(wp);
			qEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
			qEffect_->PlayAwake();
		}

		CharEffectPacket pack;
		pack.SetTargetIndex(myIndex_);
		pack.SetAnimationName("SkillQ");
		FT::SendPacket(pack);

		auto collisionList = collision_Q->GetCollisionList(eCollisionGroup::Player);

		for (GameEngineCollision* col : collisionList)
		{
			GameEngineActor* actor = col->GetActor();
			Character* character = nullptr;
			if (nullptr != actor && actor != this)
			{
				character = dynamic_cast<Character*>(actor);

				if (nullptr != character)
				{
					character->Damage((stat_.AttackPower*0.4f) + (50.0f * stat_.Level_q), this);


					character->Slow(2.0f, 0.4f);

					CharCrowdControlPacket ccPacket;
					ccPacket.SetTargetIndex(character->GetIndex());
					ccPacket.SetSlow(2.0f, 0.4f);

					//CharEffectPacket effectPacket;
					//effectPacket.SetTargetIndex(character->GetIndex());
					//effectPacket.SetAnimationName("SlowEffect");
					

					FT::SendPacket(ccPacket);
				//	FT::SendPacket(effectPacket);
				}
			}
		}

		{
			auto collisionList = collision_Q->GetCollisionList(eCollisionGroup::Monster);

			for (GameEngineCollision* col : collisionList)
			{
				GameEngineActor* actor = col->GetActor();
				IUnit* character = nullptr;
				if (nullptr != actor)
				{
					character = dynamic_cast<IUnit*>(actor);

					if (nullptr != character)
					{
						character->Damage((stat_.AttackPower * 0.4f) + (50.0f * stat_.Level_q), this);
						character->Slow(2.0f, 0.4f);

						CharCrowdControlPacket ccPacket;
						ccPacket.SetTargetIndex(character->GetIndex());
						ccPacket.SetSlow(2.0f, 0.4f);

						//CharEffectPacket effectPacket;
						//effectPacket.SetTargetIndex(character->GetIndex());
						//effectPacket.SetAnimationName("SlowEffect");


						FT::SendPacket(ccPacket);
						//	FT::SendPacket(effectPacket);
					}
				}
			}
		}

		//// 221017 SJH ADD : 테스트용 몬스터
		//auto MonsterColList = collision_Q->GetCollisionList(eCollisionGroup::Monster);
		//for (GameEngineCollision* col : MonsterColList)
		//{
		//	GameEngineActor* actor = col->GetActor();
		//	Monsters* Monster = nullptr;
		//	if (nullptr != actor && actor != this)
		//	{
		//		Monster = dynamic_cast<Monsters*>(actor);

		//		if (nullptr != Monster)
		//		{
		//			Monster->Damage(10.0f, this);
		//		}
		//	}
		//}

		b_Qhit_ = true;
	}


}

void Hyunwoo::onStartWSkill()
{
	RandomSoundPlay("Hyunwoo_PlaySkill1007300Seq0_1_ko.wav", "Hyunwoo_PlaySkill1007300Seq0_2_ko.wav", "Hyunwoo_PlaySkill1007300Seq0_3_ko.wav");

	GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill02_Activation.wav");
	PacketSoundPlay packet;
	packet.SetSound("hyunwoo_Skill02_Activation.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);

	if (GameServer::GetInstance()->IsOpened())
	{
		wEffect_->PlayAwake();
	}

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillW");
	FT::SendPacket(pack);


	timer_unstoppable_ = 1.0f;
	timer_w_ = 2.5f;
	b_isW_ = true;
	stat_.isUnstoppable_ = true;

}

void Hyunwoo::onUpdateWSkill(float _deltaTime)
{
	// W 스킬은 일시적인 방어력 증강과
	// 모든 군중 제어기 면역을 일시적으로 부여하는 스테이트입니다.

	if ("Run" != curAnimationName_)
	{
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Run", true);
		return;
	}

	mainState_.ChangeState("NormalState", true);
	normalState_.ChangeState("Watch", true);
	return;


}

void Hyunwoo::onStartESkill()
{
	RandomSoundPlay("Hyunwoo_PlaySkill1007400Seq0_1_ko.wav", "Hyunwoo_PlaySkill1007400Seq0_2_ko.wav", "Hyunwoo_PlaySkill1007400Seq0_3_ko.wav");

	timer_Dash_E = 0.0f;
	b_Ehit_ = false;
	collision_E->On();

	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.0f);
	direction_ = mousePosition - GetTransform()->GetWorldPosition(); // 커서 위치가 돌진 방향이 된다.
	direction_.Normalize3D();

	float4 cross = float4::Cross3D(direction_, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction_, { 0.0f, 0.0f, 1.0f });

	transform_.SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });

	curAnimationName_ = "SkillE_start";
	renderer_->ChangeFBXAnimation("SkillE_start", true);


	FT::PlaySoundAndSendPacket("hyunwoo_Skill03_Slide.wav", transform_.GetWorldPosition());

	if (GameServer::GetInstance()->IsOpened())
	{
		rearEffectRenderer_->On();
		rearEffectRenderer_->SetChangeAnimation("FX_BI_WindDust_01SE", true);
		rearEffectRenderer_->AnimationPlay();
	}

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillE");
	FT::SendPacket(pack);
}

void Hyunwoo::onUpdateESkill(float _deltaTime)
{
	if (true == collision_E->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_E->GetTransform(), CollisionType::OBBBox3D, float4::WHITE);
	}

	if (true == renderer_->IsCurrentAnimationEnd())
	{
		curAnimationName_ = "SkillE_loop";
		renderer_->ChangeFBXAnimation("SkillE_loop", true);
	}

	float4 dashSpeed = direction_ * 1000.f * _deltaTime;
	float4 nextMovePosition = GetTransform()->GetWorldPosition() + dashSpeed;
	timer_Dash_E += _deltaTime;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		if (0.5f <= timer_Dash_E)
		{
			timer_Dash_E = 0.0f;
			collision_E->Off();
			// 대쉬가 끝났다.
			destination_ = GetTransform()->GetWorldPosition();
			destinations_.clear();
			// 목적지를 현 위치로 만들어 주고 클리어

			b_Ehit_ = false;
			changeAnimationWait();
			mainState_.ChangeState("NormalState", true);
			normalState_.ChangeState("Watch", true);
			return;
		}


		// 여기서 피격 충돌 판정이 나옴
		auto collisionList = collision_E->GetCollisionList(eCollisionGroup::Player);

		if (false == b_Ehit_)
		{
			for (GameEngineCollision* col : collisionList)
			{
				GameEngineActor* actor = col->GetActor();
				Character* character = nullptr;

				if (nullptr != actor && actor != this)
				{
					character = dynamic_cast<Character*>(actor);

					if (nullptr != character)
					{
						character->Damage((stat_.AttackPower * 0.7f), this);
						CharCrowdControlPacket ccPacket;
						ccPacket.SetTargetIndex(character->GetIndex());
						ccPacket.SetWallSlam(0.2f, direction_ * 3000.f, 1.0f);

						// 여기선 못하고, 월 슬램 이후 그 캐릭터 클래스 내부에서 자체적으로 스턴 이펙트를 띄울 방법...
						// 야 니네 컴퓨터의 내 캐릭터 인덱스 캐릭터에게 이팩트 띄워 줘


						GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill03_Hit.wav");
						PacketSoundPlay packet;
						packet.SetSound("hyunwoo_Skill03_Hit.wav", transform_.GetWorldPosition());

						FT::SendPacket(packet);

						if (true == GameServer::GetInstance()->IsOpened())
						{
							GameServer::GetInstance()->Send(&ccPacket);
							//GameServer::GetInstance()->Send(&packet);
						}
						else if (true == GameClient::GetInstance()->IsConnected())
						{
							GameClient::GetInstance()->Send(&ccPacket);
							//GameClient::GetInstance()->Send(&packet);
						}

						b_Ehit_ = true;
					}
				}
			}
		}

		{
			auto collisionList = collision_E->GetCollisionList(eCollisionGroup::Monster);

			if (false == b_Ehit_)
			{
				for (GameEngineCollision* col : collisionList)
				{
					GameEngineActor* actor = col->GetActor();
					IUnit* character = nullptr;

					if (nullptr != actor)
					{
						character = dynamic_cast<IUnit*>(actor);

						if (nullptr != character)
						{
							character->Damage((stat_.AttackPower * 0.7f), this);
							CharCrowdControlPacket ccPacket;
							ccPacket.SetTargetIndex(character->GetIndex());
							ccPacket.SetWallSlam(0.2f, direction_ * 3000.f, 1.0f);

							// 여기선 못하고, 월 슬램 이후 그 캐릭터 클래스 내부에서 자체적으로 스턴 이펙트를 띄울 방법...
							// 야 니네 컴퓨터의 내 캐릭터 인덱스 캐릭터에게 이팩트 띄워 줘

							FT::PlaySoundAndSendPacket("hyunwoo_Skill03_Hit.wav", transform_.GetWorldPosition());



							if (true == GameServer::GetInstance()->IsOpened())
							{
								GameServer::GetInstance()->Send(&ccPacket);
								//GameServer::GetInstance()->Send(&packet);
							}
							else if (true == GameClient::GetInstance()->IsConnected())
							{
								GameClient::GetInstance()->Send(&ccPacket);
								//GameClient::GetInstance()->Send(&packet);
							}

							b_Ehit_ = true;
						}
					}
				}
			}
		}

		//// 221024 SJH ADD : 몬스터 데미지 및 CC 상태 테스트용
		//auto MonstercollisionList = collision_E->GetCollisionList(eCollisionGroup::Monster);
		//if (false == b_Ehit_)
		//{
		//	for (GameEngineCollision* col : MonstercollisionList)
		//	{
		//		GameEngineActor* actor = col->GetActor();
		//		Monsters* Monster = nullptr;

		//		if (nullptr != actor && actor != this)
		//		{
		//			Monster = dynamic_cast<Monsters*>(actor);

		//			if (nullptr != Monster)
		//			{
		//				Monster->Damage(150.0f, this);
		//				Monster->WallSlam(0.2f, direction_ * 3000.f, 1.0f);

		//				b_Ehit_ = true;
		//			}
		//		}
		//	}
		//}

		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		timer_Dash_E = 0.0f;
		collision_E->Off();
		b_Ehit_ = false;

		destination_ = GetTransform()->GetWorldPosition();
		destinations_.clear();

		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
	}

	/*  처음 적과 부딪힐 때 피해: (+공격력의 70%)(+스킬 증폭의 30%)(+적 현재 체력의 5/8/11/14/17%)
		벽에 부딪힐 시 추가 피해:60/95/130/165/200(+공격력의 55%)(+스킬 증폭의 60%)
		방어력 감소: 4/6/8/10/12%
		사정거리: 5m
		스테미너 소모: 70/75/80/85/90
		기절 지속 시간: 1.5초
		쿨다운: 18/17/16/15/14초  */

	// 아이템 조합
	// 아이템 스탯 계산

	// 공격력 방어력 이속 공격속도
	if (true == rearEffectRenderer_->IsCurAnimationEnd())
	{
		rearEffectRenderer_->Off();
	}
}

void Hyunwoo::onStartRSkill()
{
	mainState_.ChangeState("CustomState", true);
	customState_.ChangeState("CustomRSkill", true);

	uiController_->GetSkillGauge()->PlayAwake(5.0f, "시전 중");

	setRotationToMouse();
}

void Hyunwoo::onUpdateRSkill(float _deltaTime)
{
}

void Hyunwoo::onStartDSkill()
{
	// 여기서 마우스와 상대의 타겟팅 여부를 찾을 것.

	if (mouse_ == nullptr)
	{
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
		return;
	}
}

void Hyunwoo::onUpdateDSkill(float _deltaTime)
{
	bool result = false;
	if (false == b_Dhit_)
	{
		GameEngineCollision* rayCol = mouse_->GetRayCollision();


		float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);

		IUnit* otherCharacter = getMousePickedCharacter();
		target_ = otherCharacter;

		if (nullptr != otherCharacter && otherCharacter != this && false == b_Dhit_)
		{
			// 공격 처리
			float4 targetPosition = target_->GetTransform()->GetWorldPosition();
			float4 playerPosition = transform_.GetWorldPosition();
			float distance = float4::Calc_Len3D(playerPosition, targetPosition);

			if (distance > stat_.AttackRange + 90.f)
			{	// 사거리 너무 멀 때
				mainState_.ChangeState("NormalState", true);
				normalState_.ChangeState("Chase", true);
				return;
			}

			curAnimationName_ = "SkillD";
			renderer_->ChangeFBXAnimation("SkillD", true);
			otherCharacter->Damage(stat_.AttackPower + (stat_.AttackPower * (0.3f * stat_.Level_d)) + (20.0f * stat_.Level_d), this);
		

			b_Dhit_ = true;
			normalState_ << "Chase";
			return;
		}
		else
		{
			target_ = nullptr;
			result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);

			changeAnimationWait();
			mainState_.ChangeState("NormalState", true);
			normalState_.ChangeState("Watch", true);

			return;
		}

	}

	if ("SkillD" == curAnimationName_ && true == renderer_->IsCurrentAnimationEnd())
	{
		b_Dhit_ = false;

		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
	}


}

void Hyunwoo::onStartDeath()
{
	changeDeathAnimation();
}

void Hyunwoo::onUpdateDeath(float _deltaTime)
{

}

void Hyunwoo::onUpdateCustomState(float _deltaTime)
{
	customState_.Update(_deltaTime);
}



void Hyunwoo::onPlayEffect(const std::string& _effectName, IUnit* _victim, float4 _pos)
{
	// 상대방에게서 보내 진 이펙트 패킷이 execute 되면
	// 최종적으로 그 이펙트명이 해당 타겟 인덱스 캐릭터의 인자로 들어와 이 함수로 들어 옵니다.
	
	if ("BasicAttack" == _effectName)
	{
		if (_victim != nullptr)
		{
			float4 wp = _victim->GetTransform()->GetWorldPosition();
			wp.y += 50.0f;
			basicAttackEffect_->GetTransform()->SetWorldPosition(wp);
		}

		basicAttackEffect_->PlayAwake("FX_BI_Hit_08");
		return;
	}

	if ("SkillQ" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		qEffect_->GetTransform()->SetLocalPosition(wp);
		qEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		qEffect_->PlayAwake();
		return;
	}

	if ("SkillW" == _effectName)
	{
		//float4 wp = GetTransform()->GetWorldPosition();
		//wEffect_->GetTransform()->SetWorldPosition(wp);
		//wEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		wEffect_->PlayAwake();
		return;
	}

	if ("SkillE" == _effectName)
	{
		rearEffectRenderer_->On();
		rearEffectRenderer_->SetChangeAnimation("FX_BI_WindDust_01SE", true);
		rearEffectRenderer_->AnimationPlay();
		return;
	}

	if ("SkillR_awaken" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		rEffect_->GetTransform()->SetLocalPosition(wp);
		rEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		rEffect_->PlayAwake();
		return;
	}

	if ("SkillR_explode" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		rEffect_->GetTransform()->SetLocalPosition(wp);
		rEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		rEffect_->PlayExplode();

		FT::PlaySoundAndSendPacket("hyunwoo_Skill04_Hit.wav", transform_.GetWorldPosition());

		rearEffectRenderer_->On();
		rearEffectRenderer_->SetChangeAnimation("FX_BI_WindDust_01SE", true);
		rearEffectRenderer_->AnimationPlay();
		return;
	}
}

void Hyunwoo::onEffectTransformCheck(float _deltaTime)
{
	float4 wp = transform_.GetWorldPosition();
	wEffect_->GetTransform()->SetWorldPosition(wp);
}

void Hyunwoo::onLevelUp()
{
	switch (stat_.Level_q)
	{
	case 1:
		DefaultCool_Q_ = 8.5f;
		DefaultCool_W_ = 28.0f;
		DefaultCool_E_ = 18.0f;
		DefaultCool_R_ = 65.0f;
		DefaultCool_D_ = 10.0f;

		stat_.SP_Q = 50.0f;
		stat_.SP_W = 50.0f;
		stat_.SP_E = 70.0f;
		stat_.SP_R = 150.0f;
		break;
	case 2:
		DefaultCool_Q_ = 7.5f;
		DefaultCool_W_ = 25.0f;
		DefaultCool_E_ = 17.0f;
		DefaultCool_R_ = 55.0f;
		DefaultCool_D_ = 6.0f;

		stat_.SP_Q = 60.0f;
		stat_.SP_E = 75.0f;
		break;
	case 3:
		DefaultCool_Q_ = 6.5f;
		DefaultCool_W_ = 22.0f;
		DefaultCool_E_ = 16.0f;
		DefaultCool_R_ = 50.0f;
		DefaultCool_D_ = 6.0f;

		stat_.SP_Q = 70.0f;
		stat_.SP_E = 80.0f;
		break;
	case 4:
		DefaultCool_Q_ = 5.5f;
		DefaultCool_W_ = 19.0f;
		DefaultCool_E_ = 15.0f;
		DefaultCool_R_ = 50.0f;
		DefaultCool_D_ = 6.0f;

		stat_.SP_Q = 80.0f;
		stat_.SP_E = 85.0f;
		break;
	case 5:
		DefaultCool_Q_ = 4.5f;
		DefaultCool_W_ = 16.0f;
		DefaultCool_E_ = 14.0f;
		DefaultCool_R_ = 50.0f;
		DefaultCool_D_ = 6.0f;

		stat_.SP_Q = 90.0f;
		stat_.SP_E = 90.0f;
		break;
	default:
		break;
	}
}

void Hyunwoo::startCustomRSkill()
{
	RandomSoundPlay("Hyunwoo_PlaySkill1007500Seq0_1_ko.wav", "Hyunwoo_PlaySkill1007500Seq0_2_ko.wav", "Hyunwoo_PlaySkill1007500Seq0_3_ko.wav");

	curAnimationName_ = "SkillR_start";
	renderer_->ChangeFBXAnimation("SkillR_start", true);
	collision_R->On();


	//FT::PlaySoundAndSendPacket("hyunwoo_Skill04_Charging.wav", transform_.GetWorldPosition());

	if (GameServer::GetInstance()->IsOpened())
	{
		float4 wp = transform_.GetWorldPosition();
		rEffect_->GetTransform()->SetLocalPosition(wp);
		rEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
		rEffect_->PlayAwake();
	}


	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillR_awaken");
	FT::SendPacket(pack);
}

void Hyunwoo::updateCustomRSkill(float _deltaTime)
{
	if (true == collision_R->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_R->GetTransform(), CollisionType::OBBBox3D, float4::RED);
	}

	if (curAnimationName_ == "SkillR_start" && renderer_->IsCurrentAnimationEnd())
	{
		curAnimationName_ = "SkillR_loop";
		renderer_->ChangeFBXAnimation("SkillR_loop", true);
	}

	// 여기에 차징 대미지 증감 & 충돌체 크기 증감 코드 들어갈 예정
	if (false == b_Rhit_)
	{
		collisionRRate_ += _deltaTime;
		
		rTimer_ += _deltaTime;
		
		collision_R->GetTransform()->SetLocalPosition({ 0.f,0.f,150.f + collisionRRate_ * 50.0f });
		collision_R->GetTransform()->SetLocalScaling({ 350.0f, 10.0f, 100.0f + collisionRRate_ * 100.0f });


	}

	if (true == GameEngineInput::GetInst().Down("R") || 5.0f <= rTimer_)
	{
		if (false == rShot_)
		{
			rShot_ = true;

			uiController_->GetSkillGauge()->PlayFadeForce();

			if (GameServer::GetInstance()->IsOpened())
			{
				rEffect_->PlayExplode();
			}

			CharEffectPacket pack;
			pack.SetTargetIndex(myIndex_);
			pack.SetAnimationName("SkillR_explode");
			FT::SendPacket(pack);

			//GameEngineSoundManager::GetInstance()->StopSound();
		

			rearEffectRenderer_->On();
			rearEffectRenderer_->SetChangeAnimation("FX_BI_WindDust_01SE", true);
			rearEffectRenderer_->AnimationPlay();

			if (false == b_Rhit_)
			{
				// 여기서 피격 충돌 판정이 나옴
				auto collisionList = collision_R->GetCollisionList(eCollisionGroup::Player);

				for (GameEngineCollision* col : collisionList)
				{
					GameEngineActor* actor = col->GetActor();
					Character* character = nullptr;
					if (nullptr != actor && actor != this)
					{
						character = dynamic_cast<Character*>(actor);

						if (nullptr != character)
						{


							character->Damage((stat_.Level_r * (80.0f + (collisionRRate_ * 100.0f))) + (stat_.AttackPower * 0.75f), this);

						}
					}
				}
				b_Rhit_ = true;
			}

			if (false == b_Rhit_)
			{
				// 여기서 피격 충돌 판정이 나옴
				auto collisionList = collision_R->GetCollisionList(eCollisionGroup::Monster);

				for (GameEngineCollision* col : collisionList)
				{
					GameEngineActor* actor = col->GetActor();
					IUnit* character = nullptr;
					if (nullptr != actor)
					{
						character = dynamic_cast<IUnit*>(actor);

						if (nullptr != character)
						{


							character->Damage((stat_.Level_r * (80.0f + (collisionRRate_ * 100.0f))) + (stat_.AttackPower * 0.75f), this);

						}
					}
				}
				b_Rhit_ = true;
			}

			if (curAnimationName_ != "SkillR_end")
			{

				curAnimationName_ = "SkillR_end";
				renderer_->ChangeFBXAnimation("SkillR_end", true);

			}
		}
		else
		{

		}

		
	}

	if (curAnimationName_ == "SkillR_end" && true == renderer_->IsCurrentAnimationEnd())
	{
		b_Rhit_ = false;
		rShot_ = false;
		rTimer_ = 0.0f;
		changeAnimationWait();
		mainState_.ChangeState("NormalState", true);
		normalState_.ChangeState("Watch", true);
	}
	/*피해량: 80/120/160(+공격력의 75%)(+스킬 증폭의 85%) ~ 480/720/960(+공격력의 225%)(+스킬 증폭의 255%)
사정거리: 3m ~ 4.2m
스테미너 소모: 150
시전 시간: 1.2 ~ 3초
쿨다운: 65/55/50초*/
}

void Hyunwoo::endCustomRSkill()
{
	b_Rhit_ = false;
	collisionRRate_ = 0.0f;
	collision_R->Off();
	collision_R->GetTransform()->SetLocalScaling({ 350.0f, 10.0f, 100.0f + collisionRRate_ });
}

void Hyunwoo::onStartBasicAttacking(IUnit* _target)
{
	target_->Damage(stat_.AttackPower, this);



	FT::PlaySoundAndSendPacket("attackGlove_Normal_Hit_P.wav", transform_.GetWorldPosition());


	if (GameServer::GetInstance()->IsOpened())
	{
		float4 wp = target_->GetTransform()->GetWorldPosition();
		wp.y += 50.0f;

		basicAttackEffect_->GetTransform()->SetWorldPosition(wp);
		basicAttackEffect_->PlayAwake("FX_BI_Hit_08");
	}

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("BasicAttack");
	pack.SetVictimIndex(*_target);
	FT::SendPacket(pack);
	// 여기 이펙트 패킷 하나
}

void Hyunwoo::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{

}
