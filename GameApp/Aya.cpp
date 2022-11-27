#include "PreCompile.h"


#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "Aya.h"
#include "AyaBullet.h"
#include "LumiaMap.h"
#include "PacketCreateProjectile.h"
#include "RioArrow.h"
#include "MousePointer.h"
#include "Monsters.h"
#include "CharCrowdControlPacket.h"
#include "CharEffectPacket.h"
#include "AyaREffect.h"
#include "PlayerUIController.h"
#include "UI_SkillGauge.h"



Aya::Aya()
	: ammo_(6)
	, skillWFireCount_(0)
	, skillWFireDelay_(0.3f)
	, skillRCol_(nullptr)
	, pistolRenderer_(nullptr)
	, basicAttackEffect_(nullptr)
	, bullet0_(nullptr)
	, bullet1_(nullptr)
	, reloadTime_(0.0f)
	, basicHit0Effect_(nullptr)
	, basicHitEffect_(nullptr)
	, eSpectrum_(nullptr)
	, rEffect_(nullptr)
{

}

Aya::~Aya()
{

}

void Aya::LoadResource()
{
	{
		GameEngineDirectory dir;

		dir.MoveParent("EternalReturn");
		dir / "Resources" / "FBX" / "Character" / "Aya";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Aya_Idle.fbx"));
		mesh->CreateRenderingBuffer();

		GameEngineFBXMesh* spectrum0 = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("skillE_01.fbx"));
		spectrum0->CreateRenderingBuffer();
		GameEngineFBXMesh* spectrum1 = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("skillE_02.fbx"));
		spectrum1->CreateRenderingBuffer();
		GameEngineFBXMesh* spectrum2 = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("skillE_03.fbx"));
		spectrum2->CreateRenderingBuffer();
		GameEngineFBXMesh* spectrum3 = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("skillE_04.fbx"));
		spectrum3->CreateRenderingBuffer();
		GameEngineFBXMesh* spectrum4 = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("skillE_05.fbx"));
		spectrum4->CreateRenderingBuffer();

		GameEngineFBXMesh* bullet = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("bullet.fbx"));
		bullet->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Weapon_Pistol_01.fbx"));
		mesh->CreateRenderingBuffer();
		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Weapon_Pistol_01.fbx"));

		std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;
		dir.MoveParent("EternalReturn");
		dir / "Resources" / "Sound" / "Char" / "Aya";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;

		dir.MoveParent("EternalReturn");
		dir / "Resources" / "Texture" / "Aya";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("png");
		for (GameEngineFile& file : allFile)
		{
			GameEngineTextureManager::GetInst().Load(file.FileName(), file.GetFullPath());
		}
	}
}

void Aya::ReleaseResource()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("EternalReturn");
		dir / "Resources" / "Sound" / "Char" / "Aya";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		}
	}

	GameEngineDirectory dir;
	dir.MoveParent("EternalReturn");
	dir / "Resources" / "FBX" / "Character" / "Aya";

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
	}

	GameEngineFBXMeshManager::GetInst().Delete("Aya_Idle.fbx");
}

void Aya::Show()
{
	Character::Show();
	pistolRenderer_->On();
}

void Aya::Hide()
{
	Character::Hide();
	pistolRenderer_->Off();
}

void Aya::Start()
{
	Character::Start();

	skillRCol_ = CreateTransformComponent<GameEngineCollision>();
	skillRCol_->SetCollisionType(CollisionType::Sphere3D);
	skillRCol_->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	skillRCol_->GetTransform()->SetLocalScaling(700.f);

	stat_.AttackSpeed = 0.8f;
	stat_.AttackRange = 450.f;

	DefaultCool_Q_ = 6.5f;
	DefaultCool_W_ = 17.0f;
	DefaultCool_E_ = 19.0f;
	DefaultCool_R_ = 80.0f;
	DefaultCool_D_ = 40.0f;

	stat_.SP_Q = 80.0f;
	stat_.SP_W = 80.0f;
	stat_.SP_E = 85.0f;
	stat_.SP_R = 150.0f;
	// 평타 상대 피격 이펙트
	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_Shoot_01.png");
	hitBase->Cut(2, 2);

	basicAttackEffect_ = GetLevel()->CreateActor<BasicAttackEffect>();
	basicAttackEffect_->GetAttackRenderer()->SetImage("FX_BI_Shoot_01.png", "LinerSmp");
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalPosition({ 0.0f, 150.0f, 100.0f });
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalRotationDegree({ 0.0f, 90.0f,0.0f });
	basicAttackEffect_->GetAttackRenderer()->GetTransform()->SetLocalScaling(basicAttackEffect_->GetAttackRenderer()->GetCurrentTexture()->GetTextureSize());
	basicAttackEffect_->GetAttackRenderer()->CreateAnimation("FX_BI_Shoot_01.png", "FX_BI_Shoot_01", 0, 3, 0.02f, false);

	basicHitEffect_ = GetLevel()->CreateActor<BasicAttackEffect>();
	basicHitEffect_->GetAttackRenderer()->SetImage("ayahit2.png", "LinerSmp");
	basicHitEffect_->GetAttackRenderer()->GetTransform()->SetLocalPosition({ 0.0f, 150.0f, -10.0f });
	basicHitEffect_->GetAttackRenderer()->GetTransform()->SetLocalRotationDegree({ 0.0f, 0.0f,0.0f });
	basicHitEffect_->GetAttackRenderer()->GetTransform()->SetLocalScaling(basicHitEffect_->GetAttackRenderer()->GetCurrentTexture()->GetTextureSize() * 0.25f);

	basicHit0Effect_ = GetLevel()->CreateActor<BasicAttackEffect>();
	basicHit0Effect_->GetAttackRenderer()->SetImage("ayahit.png", "LinerSmp");
	basicHit0Effect_->GetAttackRenderer()->GetTransform()->SetLocalPosition({ 0.0f, 150.0f, -120.0f });
	basicHit0Effect_->GetAttackRenderer()->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f,0.0f });
	basicHit0Effect_->GetAttackRenderer()->GetTransform()->SetLocalScaling(basicHit0Effect_->GetAttackRenderer()->GetCurrentTexture()->GetTextureSize());

	eSpectrum_ = GetLevel()->CreateActor<AyaESpectrum>();

	rEffect_ = GetLevel()->CreateActor<AyaREffect>();

	SetEquipBuildItem("ImperialBurgonet", EquipmentType::HEAD);
	SetEquipBuildItem("CommandersArmor", EquipmentType::CHEST);
	SetEquipBuildItem("CorruptingTouch", EquipmentType::ARM);
	SetEquipBuildItem("StraitjacketSneakers", EquipmentType::LEG);
	SetEquipBuildItem("Grimoire", EquipmentType::ACCESSORY);
	SetEquipBuildItem("Elegance", EquipmentType::WEAPON);

	checkBuildItems();
}

void Aya::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

	static bool getWeapon = false;

	if (nullptr != uiController_ && false == getWeapon)
	{
		getItem("WaltherPPK");
		getWeapon = true;
	}

	level_->PushDebugRender(skillRCol_->GetTransform(), CollisionType::CirCle);

	stat_.passive_Count = ammo_;

	if (!bFocused_)
	{
		return;
	}

	if (ammo_ <= 0)
	{
		if (reloadTime_ == 0.0f)
		{
			FT::PlaySoundAndSendPacket("Gun_Reloading.wav", transform_.GetWorldPosition());
		}

		ChangeOverrideAnimation("Reload", "Bip001 Spine2");
		reloadTime_ += _deltaTime;

		if (renderer_->IsOverrideAnimationEnd() || reloadTime_ > 2.5f)
		{
			ammo_ = 6;
		
			CharEffectPacket packtoken;
			packtoken.SetTargetIndex(myIndex_);
			packtoken.SetAnimationName("TokenRecover");
			FT::SendPacket(packtoken);
			
			reloadTime_ = 0.0f;
			renderer_->ClearOverrideAnimation();
			overrideAnimationBoneName_ = "";
			overrideAnimationName_ = "";
			ChangeAnimation("Idle", true);
		}
	}

	FT::AddText(std::to_string(ammo_));
}

JobType Aya::GetJobType()
{
	return JobType::AYA;
}

void Aya::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::CHARACTER));
	renderer_->SetFBXMesh("Aya_Idle.fbx", "TextureDeferredLightAni", true);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	std::string ext = "UserAnimation";

	renderer_->CreateFBXAnimation("Idle", "Aya_Idle." + ext, 0, true);
	renderer_->CreateFBXAnimation("Run", "Aya_Run." + ext, 0, true);
	renderer_->CreateFBXAnimation("Attack", "Aya_Attack." + ext, 0, false);
	renderer_->CreateFBXAnimation("Reload", "Aya_Reload." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillD", "Aya_SkillD." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillE", "Aya_SkillE." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "Aya_SkillQ." + ext, 0, false, 0.03f);
	renderer_->CreateFBXAnimation("SkillR_End", "Aya_SkillR_End." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillR_Start", "Aya_SkillR_Start." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Back", "Aya_SkillW_Back." + ext, 0);
	renderer_->CreateFBXAnimation("SkillW_Forward", "Aya_SkillW_Forward." + ext, 0);
	renderer_->CreateFBXAnimation("SkillW_Left", "Aya_SkillW_Left." + ext, 0);
	renderer_->CreateFBXAnimation("SkillW_Right", "Aya_SkillW_Right." + ext, 0);
	renderer_->CreateFBXAnimation("SkillW_Shot", "Aya_SkillW_Shot." + ext, 0, false, 0.027f);
	renderer_->CreateFBXAnimation("SkillW_Wait", "Aya_SkillW_Wait." + ext, 0);
	renderer_->CreateFBXAnimation("Death", "Aya_Death." + ext, 0, false);

	renderer_->CreateFBXAnimation("Craft", "Aya_Craft.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Operate", "Aya_Operate.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Arrive", "Aya_Arrive.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("RestStart", "Aya_Rest_Start.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("RestLoop", "Aya_Rest_Loop.UserAnimation", 0, true);
	renderer_->CreateFBXAnimation("RestEnd", "Aya_Rest_End.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("Collect", "Aya_Collect.UserAnimation", 0, true);

	renderer_->ChangeFBXAnimation("Idle");
	renderer_->LightShadowOn();

	pistolRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::WEAPON));
	pistolRenderer_->SetFBXMesh("Weapon_Pistol_01.fbx", "TextureDeferredLightAni");
	pistolRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	pistolRenderer_->GetTransform()->SetLocalScaling(100.f);
	pistolRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });
	pistolRenderer_->SetParentBoneName(renderer_, "Bip001 R Finger1");
	pistolRenderer_->SetCustomOffset({ 2.7f, 0.f, -2.5f });
	pistolRenderer_->CreateFBXAnimation("Idle", "Weapon_Pistol_01.fbx");
	pistolRenderer_->ChangeFBXAnimation("Idle");
	pistolRenderer_->LightShadowOn();

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
		MainOutLineRenderer_->Off();

		// Weapon1
		WeaponOutLineRenderer1_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponOutLineRenderer1_->SetBaseRenderer(pistolRenderer_, "PreprocessingAni", true, false);

		OutLineData Weapon1OutLineData = WeaponOutLineRenderer1_->GetOutLineData();
		float4 Weapon1Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon1OutLineData.LineThickness;
		float4 CalcWeapon1Scale = pistolRenderer_->GetTransform()->GetLocalScaling() + Weapon1Thickness;
		WeaponOutLineRenderer1_->GetTransform()->SetLocalScaling(CalcWeapon1Scale);
		WeaponOutLineRenderer1_->GetTransform()->SetLocalRotationDegree(pistolRenderer_->GetTransform()->GetLocalRotation());
		WeaponOutLineRenderer1_->GetTransform()->SetLocalPosition(pistolRenderer_->GetTransform()->GetLocalPosition());
		WeaponOutLineRenderer1_->Off();
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

		// Weapon1
		WeaponSilhouetteRenderer1_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponSilhouetteRenderer1_->SetBaseRenderer(pistolRenderer_, "PreprocessingAni", true);

		OutLineData Weapon1OutLineData = WeaponSilhouetteRenderer1_->GetOutLineData();
		float4 Weapon1Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon1OutLineData.LineThickness;
		float4 CalcWeapon1Scale = pistolRenderer_->GetTransform()->GetLocalScaling() + Weapon1Thickness;
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalScaling(CalcWeapon1Scale);
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalRotationDegree(pistolRenderer_->GetTransform()->GetLocalRotation());
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalPosition(pistolRenderer_->GetTransform()->GetLocalPosition());
	}
}

void Aya::changeAnimationRun()
{
	ChangeAnimation("Run");
}

void Aya::changeAnimationWait()
{
	ChangeAnimation("Idle");
}

void Aya::changeAnimationBasicAttack()
{
	if (bFocused_)
	{
		if (ammo_ <= 0)
		{
			ChangeAnimation("Idle");
			return;
		}
	}

	ChangeAnimation("Attack", true);
}

void Aya::changeDeathAnimation()
{
	ChangeAnimation("Death");
}

void Aya::changeAnimationOperate()
{
	ChangeAnimation("Operate");
}

void Aya::changeAnimationArrive()
{
	ChangeAnimation("Arrive");
}

void Aya::changeAnimationRestStart()
{
	ChangeAnimation("RestStart");
}

void Aya::changeAnimationRestLoop()
{
	ChangeAnimation("RestLoop");
}

void Aya::changeAnimationRestEnd()
{
	ChangeAnimation("RestEnd");
}

void Aya::changeAnimationCraft()
{
	ChangeAnimation("Craft");
}

void Aya::changeAnimationGather()
{
	ChangeAnimation("Collect");
}

void Aya::onStartBasicAttacking(IUnit* _target)
{
	if (ammo_ <= 0)
	{
		ChangeAnimation("Idle");
		mainState_ << "NormalState";
		return;
	}
	else
	{
		ChangeAnimation("Attack");
	}

	if (GameServer::GetInstance()->IsOpened())
	{
		--ammo_;
	}

	CharEffectPacket packtoken;
	packtoken.SetTargetIndex(myIndex_);
	packtoken.SetAnimationName("TokenLost");
	FT::SendPacket(packtoken);

	float4 offset = { 20.f, 120.f, 30.f, 0.f };
	offset = offset * transform_.GetTransformData().WorldWorld_;
	float4 startPosition = transform_.GetWorldPosition();
	startPosition += offset;

	float arrowSpeed = 2500.f;
	{
		PacketCreateProjectile packetBullet;
		packetBullet.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
		packetBullet.SetType(eProjectileType::AyaTargetBullet);
		FT::SendPacket(packetBullet);
	}

	if (GameServer::GetInstance()->IsOpened())
	{
		bullet0_ = level_->CreateActor<AyaBullet>();
		bullet0_->MakeTarget(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);

		basicAttackEffect_->GetAttackRenderer()->SetColor({ 1.0f, 1.0f, 0.7f });
		basicAttackEffect_->PlayAwake("FX_BI_Shoot_01");
		//arrow = level_->CreateActor<AyaBullet>();
		//bullet0_->MakeTarget(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
	}

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("BasicAttack_Shot");
	FT::SendPacket(pack);

	FT::PlaySoundAndSendPacket("Gun_Fire.wav", transform_.GetWorldPosition());
}

void Aya::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{
	if (ammo_ <= 0)
	{
		mainState_ << "NormalState";
		return;
	}

	if (renderer_->IsCurrentAnimationEnd())
	{
		changeAnimationWait();
	}
}

void Aya::onStartQSkill()
{
	renderer_->ClearOverrideAnimation();
	overrideAnimationBoneName_ = "";
	target_ = getMousePickedCharacter();

	if (target_ == nullptr)
	{
		coolTimer_Q_ = 0.0f;
		stat_.SP += stat_.SP_Q;
		FT::SendStatPacket(stat_, GetIndex());
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}

	coolTimer_Q_ = 8.0f;

	float4 targetPosition = target_->GetTransform()->GetWorldPosition();
	float4 playerPosition = transform_.GetWorldPosition();

	float dist = float4::Calc_Len3D(targetPosition, playerPosition);
	if (dist > stat_.AttackRange + 100.f)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}

	ChangeAnimation("SkillQ");
	setRotationTo(targetPosition, playerPosition);

	float doubleStrikeDelay = 0.4f;

	float4 offset = { 20.f, 120.f, 30.f, 0.f };
	offset = offset * transform_.GetTransformData().WorldWorld_;
	float4 startPosition = transform_.GetWorldPosition();
	startPosition += offset;

	float arrowSpeed = 2500.f;

	{
		PacketCreateProjectile packetBullet;
		packetBullet.MakeTargetProjectile(*this, stat_.AttackPower / 1.5f, startPosition, arrowSpeed, *target_);
		packetBullet.SetType(eProjectileType::AyaTargetBullet);
		FT::SendPacket(packetBullet);
	}
	{
		PacketCreateProjectile packetArrow;
		packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 1.0f, startPosition, arrowSpeed, *target_);
		packetArrow.SetWaitTime(doubleStrikeDelay);
		packetArrow.SetType(eProjectileType::AyaTargetBullet);
		FT::SendPacket(packetArrow);
	}

	if (GameServer::GetInstance()->IsOpened())
	{
		AyaBullet* bullet = level_->CreateActor<AyaBullet>();
		bullet->MakeTarget(*this, stat_.AttackPower / 1.5f, startPosition, arrowSpeed, *target_);

		bullet = level_->CreateActor<AyaBullet>();
		bullet->MakeTarget(*this, stat_.AttackPower / 1.0f, startPosition, arrowSpeed, *target_);
		bullet->SetWaitTime(doubleStrikeDelay);
	}
	RandomSoundPlay("Aya_PlaySkill1002200seq0_1_ko.wav", "Aya_PlaySkill1002200seq0_2_ko.wav", "Aya_PlaySkill1002200seq0_3_ko.wav");	
	FT::PlaySoundAndSendPacket("aya_Skill01_Attack_v1.wav", transform_.GetWorldPosition());

	uiController_->GetSkillGauge()->PlayAwake(0.3f, "시전 중");
}

void Aya::onUpdateQSkill(float _deltaTime)
{
	if (attackState_.GetTime() > 0.3f)
	{
		FT::PlaySoundAndSendPacket("aya_Skill01_Attack_v1.wav", transform_.GetWorldPosition());
		attackState_.GetCurrentState()->Time_ = -10.f;
	}

	if (renderer_->IsCurrentAnimationEnd())
	{
		changeAnimationWait();
		mainState_ << "NormalState";
	}
}

void Aya::onStartWSkill()
{
	renderer_->ClearOverrideAnimation();
	overrideAnimationBoneName_ = "";
	setRotationToMouse();

	ChangeAnimation("SkillW_Wait");
	ChangeOverrideAnimation("SkillW_Shot", "Bip001 Spine2", true);


	float4 offset = { 20.f, 120.f, 30.f, 0.f };
	offset = offset * transform_.GetTransformData().WorldWorld_;
	float4 startPosition = transform_.GetWorldPosition();
	startPosition += offset;

	float bulletSpeed = 2500.f;



	PacketCreateProjectile packetBullet;
	packetBullet.MakeNonTargetProjectile(*this, stat_.AttackPower / 1.5f, startPosition, transform_.GetWorldRotation().y, bulletSpeed);
	packetBullet.SetScale({ 80.f, 150.f, 100 });
	packetBullet.SetType(eProjectileType::AyaWSkill);
	packetBullet.SetLifeTime(0.5);
	FT::SendPacket(packetBullet);


	if (GameServer::GetInstance()->IsOpened())
	{
		AyaBullet* bullet = level_->CreateActor<AyaBullet>();
		bullet->MakeNonTarget(*this, stat_.AttackPower / 1.5f, startPosition, transform_.GetWorldRotation().y, bulletSpeed);
		bullet->SetScale({ 80.f, 150.f, 100 });
	}
	RandomSoundPlay("Aya_PlaySkill1002300seq0_1_ko.wav", "Aya_PlaySkill1002300seq0_2_ko.wav", "Aya_PlaySkill1002300seq0_3_ko.wav");
	FT::PlaySoundAndSendPacket("aya_Skill02_Shot.wav", transform_.GetWorldPosition());
	skillWFireCount_ = 1;


	uiController_->GetSkillGauge()->PlayReverseAwake(3.0f, "시전 중");
}

void Aya::onUpdateWSkill(float _deltaTime)
{
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	static float cos = 0.0f;
	static float4 cross;
	if (GameEngineInput::Press("RButton") || GameEngineInput::Down("RButton"))
	{
		Move(mousePosition);
		cos = float4::Dot3DToCos(mousePosition - transform_.GetWorldPosition(), transform_.GetWorldForwardVector());
		cross = float4::Cross3D(mousePosition - transform_.GetWorldPosition(), transform_.GetWorldForwardVector());
	}

	if (cos > 0.8f)
	{
		ChangeAnimation("SkillW_Forward");
	}
	else if (cos < -0.8f)
	{
		ChangeAnimation("SkillW_Back");
	}
	else
	{	// 양수 왼쪽, 음수 오른쪽
		if (cross.y > 0.f)
		{
			ChangeAnimation("SkillW_Left");
		}
		else
		{
			ChangeAnimation("SkillW_Right");
		}
	}


	float4 worldPosition = transform_.GetWorldPosition();
	worldPosition.y = destination_.y;
	if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	{
		moveTickLockDirection(_deltaTime, worldPosition);
	}
	else
	{
		if (!destinations_.empty())
		{
			destination_ = destinations_.back();
			destinations_.pop_back();

			// 여기서 한번 더 해주지 않으면 움직임이 1 프레임 손실된다.
			if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
			{
				moveTickLockDirection(_deltaTime, worldPosition);
			}
		}
		else
		{
			ChangeAnimation("SkillW_Wait");
		}
	}

	if (renderer_->IsOverrideAnimationEnd())
	{
		renderer_->ClearOverrideAnimation();
		overrideAnimationBoneName_ = "";
		overrideAnimationName_ = "";
	}

	if (attackState_.GetTime() >= 0.3f)
	{
		attackState_.GetCurrentState()->Time_ = 0.0f;
		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		float bulletSpeed = 2500.f;


		PacketCreateProjectile packetBullet;
		packetBullet.MakeNonTargetProjectile(*this, stat_.AttackPower / 1.5f, startPosition, transform_.GetWorldRotation().y, bulletSpeed);
		packetBullet.SetScale({ 80.f, 150.f, 100 });
		packetBullet.SetType(eProjectileType::AyaWSkill);
		packetBullet.SetLifeTime(0.5);
		FT::SendPacket(packetBullet);


		if (GameServer::GetInstance()->IsOpened())
		{
			AyaBullet* bullet = level_->CreateActor<AyaBullet>();
			bullet->MakeNonTarget(*this, stat_.AttackPower / 1.5f, startPosition, transform_.GetWorldRotation().y, bulletSpeed);
			bullet->SetScale({ 80.f, 150.f, 100 });
		}

		FT::PlaySoundAndSendPacket("aya_Skill02_Shot.wav", transform_.GetWorldPosition());

		ChangeOverrideAnimation("SkillW_Shot", "Bip001 Spine2", true);
		++skillWFireCount_;
	}

	if (skillWFireCount_ >= 10)
	{
		renderer_->ClearOverrideAnimation();
		overrideAnimationBoneName_ = "";
		overrideAnimationName_ = "";
		changeAnimationWait();
		mainState_ << "NormalState";

	}

	if (GameEngineInput::Down("Z"))
	{
		renderer_->ClearOverrideAnimation();
		overrideAnimationBoneName_ = "";
		mainState_ << "NormalState";
	}
}

void Aya::onStartESkill()
{

	float4 wp = transform_.GetWorldPosition();

	renderer_->ClearOverrideAnimation();
	overrideAnimationBoneName_ = "";
	setRotationToMouse();

	ChangeAnimation("SkillE", true);
	RandomSoundPlay("Aya_PlaySkill1002400seq0_1_ko.wav", "Aya_PlaySkill1002400seq0_2_ko.wav", "Aya_PlaySkill1002400seq0_3_ko.wav");
	FT::PlaySoundAndSendPacket("aya_Skill03_Activation.wav", wp);

	if (GameServer::GetInstance()->IsOpened())
	{
		eSpectrum_->GetTransform()->SetWorldPosition(wp);
		eSpectrum_->GetTransform()->SetLocalRotationDegree(transform_.GetLocalRotation());
		eSpectrum_->PlayAwake();
	}


	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillE");
	FT::SendPacket(pack);
}

void Aya::onUpdateESkill(float _deltaTime)
{
	if (attackState_.GetCurrentState()->Time_ > 0.5f)
	{
		destinations_.clear();
		destination_ = transform_.GetWorldPosition();
		changeAnimationWait();

		mainState_ << "NormalState";
	}

	float4 moveSpeed = (direction_ * stat_.MovementSpeed * 2.0f) * _deltaTime;
	float4 nextMovePosition = transform_.GetWorldPosition() + moveSpeed;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		eSpectrum_->Reset();
	}


}

void Aya::onStartRSkill()
{
	renderer_->ClearOverrideAnimation();
	overrideAnimationBoneName_ = "";
	ChangeAnimation("SkillR_Start");

	RandomSoundPlay("Aya_PlaySkill1002500seq0_1_ko.wav", "Aya_PlaySkill1002500seq0_2_ko.wav", "Aya_PlaySkill1002500seq0_3_ko.wav");
	FT::PlaySoundAndSendPacket("aya_Skill04_Ready.wav", transform_.GetWorldPosition());

	if (GameServer::GetInstance()->IsOpened())
	{
		rEffect_->PlayAwake();
	}

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("SkillR");
	FT::SendPacket(pack);

	uiController_->GetSkillGauge()->PlayAwake(1.5f, "시전 중");
}

void Aya::onUpdateRSkill(float _d1eltaTime)
{
	if (attackState_.GetTime() < 0.0f)
	{
		if (renderer_->IsCurrentAnimationEnd())
		{
			normalState_ << "Watch";
			mainState_ << "NormalState";
		}
		return;
	}

	if (attackState_.GetTime() > 1.5f)
	{
		std::list<GameEngineCollision*> list = skillRCol_->GetCollisionList(eCollisionGroup::Player);
		for (GameEngineCollision* c : list)
		{
			Character* unit = dynamic_cast<Character*>(c->GetActor());
			if (nullptr != unit)
			{
				if (unit == this)
				{
					continue;
				}
				unit->Damage(300.f + stat_.AttackPower * 2.f, this);
				FT::PlaySoundAndSendPacket("aya_Skill01_Attack_v1.wav", transform_.GetWorldPosition());
				unit->Stun(2.0f);

				CharCrowdControlPacket packet;
				packet.SetStun(2.0f);
				packet.SetTargetIndex(unit->GetIndex());
				FT::SendPacket(packet);
			}
		}

		list.clear();
		list = skillRCol_->GetCollisionList(eCollisionGroup::Monster);
		for (GameEngineCollision* c : list)
		{
			Monsters* unit = dynamic_cast<Monsters*>(c->GetActor());
			if (nullptr != unit)
			{
				unit->Damage(300.f + stat_.AttackPower * 2.f, this);
				FT::PlaySoundAndSendPacket("aya_Skill01_Attack_v1.wav", transform_.GetWorldPosition());
				unit->Stun(2.0f);

				CharCrowdControlPacket packet;
				packet.SetStun(2.0f);
				packet.SetTargetIndex(unit->GetIndex());
				FT::SendPacket(packet);
			}
		}

		attackState_.GetCurrentState()->Time_ = -1000.f;
		ChangeAnimation("SkillR_End");
		FT::PlaySoundAndSendPacket("aya_Skill04_Activation.wav", transform_.GetWorldPosition());
	}

}

void Aya::onStartDSkill()
{
	renderer_->ClearOverrideAnimation();
	
	ChangeOverrideAnimation("Reload", "Bip001 Spine2");
	FT::PlaySoundAndSendPacket("Pistol_skill_D.wav", transform_.GetWorldPosition());
}

void Aya::onUpdateDSkill(float _deltaTime)
{
	if (renderer_->IsOverrideAnimationEnd())
	{
		renderer_->ClearOverrideAnimation();
		overrideAnimationBoneName_ = "";
		overrideAnimationName_ = "";
		mainState_ << "NormalState";


		if (GameServer::GetInstance()->IsOpened())
		{
			ammo_ = 6;
		}
	
		CharEffectPacket packtoken;
		packtoken.SetTargetIndex(myIndex_);
		packtoken.SetAnimationName("TokenRecover");
		FT::SendPacket(packtoken);
		
		return;
	}

	inputProcess(_deltaTime);
	moveProcess(_deltaTime);
}

void Aya::onUpdateCustomState(float _deltaTime)
{
}

void Aya::onStartDeath()
{
}

void Aya::onUpdateDeath(float _deltaTime)
{
}

void Aya::onPlayEffect(const std::string& _effectName, IUnit* _victim, float4 _pos)
{
	float4 startPosition = transform_.GetWorldPosition();
	float arrowSpeed = 1500.f;

	if ("TokenLost" == _effectName)
	{
		if (ammo_ <= 0)
		{
			return;
		}

		ammo_--;
		stat_.passive_Count = ammo_;
	}

	if ("TokenRecover" == _effectName)
	{
		ammo_ = 6;
		stat_.passive_Count = ammo_;
	}

	if ("BasicAttack_Shot" == _effectName)
	{
		basicAttackEffect_->GetAttackRenderer()->SetColor({ 1.0f, 1.0f, 0.7f });
		basicAttackEffect_->PlayAwake("FX_BI_Shoot_01");

		//bullet0_ = level_->CreateActor<AyaBullet>();
		//bullet0_->SetDummy();
		//bullet0_->MakeTarget(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *_victim);
		return;
	}

	if ("BasicAttack_Hit" == _effectName)
	{
		if (_victim != nullptr)
		{
			float4 wp = _victim->GetTransform()->GetWorldPosition();
			wp.y -= 50.0f;
			//	bullet0_ = level_->CreateActor<AyaBullet>();
			//	bullet0_->SetDummy();
			//	bullet0_->GetAttackEffect()->GetTransform()->SetWorldPosition(wp);
			basicHitEffect_->GetAttackRenderer()->SetColor({ 1.0f,1.0f,0.7f });
			basicHitEffect_->GetTransform()->SetWorldPosition(wp);
			basicHitEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
			basicHitEffect_->PlayFade(0.3f);
		}
		return;
	}

	if ("BasicAttack_Hit0" == _effectName)
	{
		if (_victim != nullptr)
		{
			float4 wp = _victim->GetTransform()->GetWorldPosition();
			wp.y -= 50.0f;

			//	bullet0_ = level_->CreateActor<AyaBullet>();
			//	bullet0_->SetDummy();
			//	bullet0_->GetAttackEffect()->GetTransform()->SetWorldPosition(wp);
			basicHit0Effect_->GetAttackRenderer()->SetColor({ 1.0f,1.0f,0.7f });
			basicHit0Effect_->GetTransform()->SetWorldPosition(wp);
			basicHit0Effect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
			basicHit0Effect_->PlayFade(0.3f);
		}
		//bullet0_->GetAttackEffect()->PlayAwake("Fx_ShootGlowSE_04");
		return;
	}

	if ("SkillE" == _effectName)
	{
		eSpectrum_->GetTransform()->SetWorldPosition(startPosition);
		eSpectrum_->GetTransform()->SetLocalRotationDegree(transform_.GetLocalRotation());
		eSpectrum_->PlayAwake();
		return;
	}

	if ("SkillR" == _effectName)
	{
		rEffect_->PlayAwake();
		return;
	}

}

void Aya::onEffectTransformCheck(float _deltaTime)
{
	float4 wp = GetTransform()->GetWorldPosition();
	basicAttackEffect_->GetTransform()->SetWorldPosition(wp);
	basicAttackEffect_->GetTransform()->SetLocalRotationDegree(transform_.GetLocalRotation());

	rEffect_->GetTransform()->SetWorldPosition(wp);


	//basicHitEffect_->GetTransform()->SetWorldPosition(wp);
	//basicHitEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
}

void Aya::onLevelUp()
{
	switch (stat_.Level_q)
	{
	case 1:
		DefaultCool_Q_ = 6.5f;
		DefaultCool_W_ = 17.0f;
		DefaultCool_E_ = 19.0f;
		DefaultCool_R_ = 80.0f;
		DefaultCool_D_ = 40.0f;
		stat_.SP_Q = 80.0f;
		stat_.SP_W = 80.0f;
		stat_.SP_E = 85.0f;
		stat_.SP_R = 150.0f;
		break;
	case 2:
		DefaultCool_Q_ = 6.0f;
		DefaultCool_W_ = 15.0f;
		DefaultCool_E_ = 17.0f;
		DefaultCool_R_ = 60.0f;
		DefaultCool_D_ = 25.0f;

		stat_.SP_W = 90.0f;
		stat_.SP_E = 90.0f;

		break;
	case 3:
		DefaultCool_Q_ = 5.5f;
		DefaultCool_W_ = 13.0f;
		DefaultCool_E_ = 15.0f;
		DefaultCool_R_ = 40.0f;

		stat_.SP_W = 100.0f;
		stat_.SP_E = 95.0f;
		break;
	case 4:
		DefaultCool_Q_ = 5.0f;
		DefaultCool_W_ = 11.0f;
		DefaultCool_E_ = 13.0f;

		stat_.SP_W = 110.0f;
		stat_.SP_E = 100.0f;
		break;
	case 5:
		DefaultCool_Q_ = 4.5f;
		DefaultCool_W_ = 9.0f;
		DefaultCool_E_ = 11.0f;

		stat_.SP_W = 120.0f;
		stat_.SP_E = 105.0f;
		break;
	default:
		break;
	}
}
