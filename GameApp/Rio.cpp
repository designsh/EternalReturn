#include "PreCompile.h"
#include "Rio.h"
#include "MousePointer.h"
#include "LumiaMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameApp/LumiaLevel.h>
#include "RioArrow.h"
#include "PacketCreateProjectile.h"
#include "PacketSoundPlay.h"
#include "eProjectileType.h"
#include "RioDSkill.h"
#include "BasicAttackEffect.h"
#include "RioDSkillArrow.h"
#include "RioWSkillWind.h"
#include "RioDSkillFirstShot.h"
#include "CharEffectPacket.h"
#include "RioDSkillEffect.h"

#include "PlayerUIController.h"
#include "UI_SkillGauge.h"

Rio::Rio()
	: Character()
	, bLongBow_(false)
	, skillECollision_(nullptr)
	, bSkillEPassable_(false)
	, bShortRSkillExtra_(false)
	, shortBow_(nullptr)
	, longBow_(nullptr)
	, skillETime_(0.0f)
	, skillRTime_(0.0f)
	, dSkillShotEffect_(nullptr)
	, dSkillEffect_(nullptr)
{

}

Rio::~Rio()
{

}

void Rio::LoadResource()
{
	{
		GameEngineDirectory dir;

		dir.MoveParent("EternalReturn");
		dir / "Resources" / "FBX" / "Character" / "Rio";

		GameEngineFBXMesh* arrow = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_000_Arrow.fbx"));
		arrow->CreateRenderingBuffer();

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Short_Run.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Bow_Short_Idle.fbx"));
		mesh->CreateRenderingBuffer();

		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Bow_Short_Idle.fbx"));

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Bow_Long_Idle.fbx"));
		mesh->CreateRenderingBuffer();

		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Bow_Long_Idle.fbx"));

		std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;
		dir.MoveParent("EternalReturn");
		dir / "Resources" / "Sound" / "Char" / "Rio";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;

		dir.MoveParent("EternalReturn");
		dir / "Resources" / "Texture" / "Rio";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("png");
		for (GameEngineFile& file : allFile)
		{
			GameEngineTextureManager::GetInst().Load(file.FileName(), file.GetFullPath());
		}
	}
}

void Rio::ReleaseResource()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("EternalReturn");
		dir / "Resources" / "Sound" / "Char" / "Rio";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		}
	}

	GameEngineDirectory dir;
	dir.MoveParent("EternalReturn");
	dir / "Resources" / "FBX" / "Character" / "Rio";

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
	}

	GameEngineFBXMeshManager::GetInst().Delete("Rio_Bow_Short_Idle.fbx");
	GameEngineFBXMeshManager::GetInst().Delete("Rio_Short_Run.fbx");
}

void Rio::Show()
{
	Character::Show();
	if (bLongBow_)
	{
		longBow_->On();
	}
	else
	{
		shortBow_->On();
	}
}

void Rio::Hide()
{
	Character::Hide();
	longBow_->Off();
	shortBow_->Off();
}

void Rio::Start()
{
	Character::Start();

	skillECollision_ = CreateTransformComponent<GameEngineCollision>();
	skillECollision_->SetCollisionType(CollisionType::OBBBox3D);
	skillECollision_->GetTransform()->SetLocalScaling(500.f);

	customState_.CreateState(MakeState(Rio, SkillEBegin));
	customState_.CreateState(MakeState(Rio, SkillEShot));
	customState_.CreateState(MakeState(Rio, SkillEEnd));

	stat_.AttackRange = 450.0f;

	DefaultCool_Q_ = 1.0f;
	DefaultCool_W_ = 12.0f;
	DefaultCool_E_ = 8.0f;
	DefaultCool_R_ = 80.0f;
	DefaultCool_D_ = 25.0f;

	stat_.SP_Q = 0.0f;
	stat_.SP_W = 50.0f;
	stat_.SP_E = 50.0f;
	stat_.SP_R = 100.0f;

	basicHitEffect_ = GetLevel()->CreateActor<BasicAttackEffect>();
	basicHitEffect_->GetAttackRenderer()->SetImage("rioHit2.png", "PointSmp");
	basicHitEffect_->GetAttackRenderer()->GetTransform()->SetLocalPosition({ 0.0f, 150.0f, -10.0f });
	basicHitEffect_->GetAttackRenderer()->GetTransform()->SetLocalRotationDegree({ 0.0f, 0.0f,0.0f });
	basicHitEffect_->GetAttackRenderer()->GetTransform()->SetLocalScaling(basicHitEffect_->GetAttackRenderer()->GetCurrentTexture()->GetTextureSize() * 0.25f);

	basicHit0Effect_ = GetLevel()->CreateActor<BasicAttackEffect>();
	basicHit0Effect_->GetAttackRenderer()->SetImage("rioHit.png", "PointSmp");
	basicHit0Effect_->GetAttackRenderer()->GetTransform()->SetLocalPosition({ 0.0f, 150.0f, -120.0f });
	basicHit0Effect_->GetAttackRenderer()->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f,0.0f });
	basicHit0Effect_->GetAttackRenderer()->GetTransform()->SetLocalScaling(basicHit0Effect_->GetAttackRenderer()->GetCurrentTexture()->GetTextureSize());

	dSkillShotEffect_ = GetLevel()->CreateActor<RioDSkillFirstShot>();
	dSkillShotEffect_->SetParent(this);

	dSkillEffect_ = GetLevel()->CreateActor<RioDSkillEffect>();
	dSkillEffect_->SetParent(this);

	SetEquipBuildItem("CrystalTiara", EquipmentType::HEAD);
	SetEquipBuildItem("OpticalCamouflageSuit", EquipmentType::CHEST);
	SetEquipBuildItem("Radar", EquipmentType::ARM);
	SetEquipBuildItem("Bucephalus", EquipmentType::LEG);
	SetEquipBuildItem("LacedQuiver", EquipmentType::ACCESSORY);
	SetEquipBuildItem("JebesAlteredBow", EquipmentType::WEAPON);

	checkBuildItems();
}

void Rio::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

	static bool getWeapon = false;

	if (nullptr != uiController_ && false == getWeapon)
	{
		getItem("Bow");
		getWeapon = true;
	}
}

void Rio::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::CHARACTER));
	renderer_->SetFBXMesh("Rio_Short_Run.fbx", "TextureDeferredLightAni", true);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	std::string ext = "UserAnimation";

	renderer_->CreateFBXAnimation("Run_Short", "Rio_Short_Run." + ext, 0);
	renderer_->CreateFBXAnimation("Run_Long", "Rio_Long_Run." + ext, 0);

	renderer_->CreateFBXAnimation("Wait_Short", "Rio_Short_Wait." + ext, 0);
	renderer_->CreateFBXAnimation("Wait_Long", "Rio_Long_Wait." + ext, 0);

	renderer_->CreateFBXAnimation("BasicAttack_Short", "Rio_Short_Attack." + ext, 0, false);
	renderer_->CreateFBXAnimation("BasicAttack_Long", "Rio_Long_Attack." + ext, 0, false);

	renderer_->CreateFBXAnimation("SkillQ_Short", "Rio_Short_Skill_Q." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillQ_Long", "Rio_Long_Skill_Q." + ext, 0, false);

	renderer_->CreateFBXAnimation("SkillW_Short", "Rio_Short_Skill_W." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillW_Long", "Rio_Long_Skill_W." + ext, 0, false);

	renderer_->CreateFBXAnimation("SkillE_Short", "Rio_Short_Skill_E." + ext, 0, false);

	renderer_->CreateFBXAnimation("SkillR_Short_Start", "Rio_Short_Skill_R_Start." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillR_Short_End", "Rio_Short_Skill_R_End." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillR_Long", "Rio_Long_Skill_R." + ext, 0, false);

	renderer_->CreateFBXAnimation("SkillD_Short", "Rio_Short_Skill_D." + ext, 0, false);
	renderer_->CreateFBXAnimation("SkillD_Long", "Rio_Long_Skill_D." + ext, 0, false);

	renderer_->CreateFBXAnimation("Rio_CraftFood", "Rio_CraftFood." + ext, 0, false);
	renderer_->CreateFBXAnimation("Rio_CraftMetal", "Rio_CraftMetal." + ext, 0, false);

	renderer_->CreateFBXAnimation("Operate", "Rio_Operate.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("Arrive", "Rio_Arrive.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("RestStart", "Rio_Rest_Start.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("RestLoop", "Rio_Rest_Loop.UserAnimation", 0, true);
	renderer_->CreateFBXAnimation("RestEnd", "Rio_Rest_End.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("Collect", "Rio_Collect.UserAnimation", 0, true);

	renderer_->CreateFBXAnimation("Death", "Rio_Death." + ext, 0, false);

	renderer_->ChangeFBXAnimation("Wait_Short");
	renderer_->LightShadowOn();

	renderer_->GetRenderSet(1).isRender = false;

	shortBow_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::WEAPON));
	shortBow_->SetFBXMesh("Rio_Bow_Short_Idle.fbx", "TextureDeferredLightAni");
	shortBow_->GetTransform()->SetLocalScaling(100.f);
	shortBow_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	shortBow_->SetParentBoneName(renderer_, "Bip001 L Finger2");
	shortBow_->SetCustomOffset({ 0.0f, 0.0f, -0.1f });

	shortBow_->CreateFBXAnimation("ShortBow_Idle", "Rio_Bow_Short_Idle.fbx");
	shortBow_->ChangeFBXAnimation("ShortBow_Idle");
	shortBow_->LightShadowOn();

	longBow_ = CreateTransformComponent<GameEngineFBXRenderer>(static_cast<int>(ObjectRenderOrder::WEAPON));
	longBow_->SetFBXMesh("Rio_Bow_Long_Idle.fbx", "TextureDeferredLightAni");
	longBow_->GetTransform()->SetLocalScaling(100.f);
	longBow_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	longBow_->SetParentBoneName(renderer_, "Bip001 L Finger2");
	longBow_->SetCustomOffset({ 0.015f, 0.45f, -0.12f });

	longBow_->CreateFBXAnimation("LongBow_Idle", "Rio_Bow_Long_Idle.fbx");
	longBow_->ChangeFBXAnimation("LongBow_Idle");

	longBow_->GetRenderSet(0).isRender = false;
	longBow_->LightShadowOn();
	longBow_->Off();

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

		// Weapon1
		WeaponOutLineRenderer1_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponOutLineRenderer1_->SetBaseRenderer(shortBow_, "PreprocessingAni", true, false);

		OutLineData Weapon1OutLineData = WeaponOutLineRenderer1_->GetOutLineData();
		float4 Weapon1Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon1OutLineData.LineThickness;
		float4 CalcWeapon1Scale = shortBow_->GetTransform()->GetLocalScaling() + Weapon1Thickness;
		WeaponOutLineRenderer1_->GetTransform()->SetLocalScaling(CalcWeapon1Scale);
		WeaponOutLineRenderer1_->GetTransform()->SetLocalRotationDegree(shortBow_->GetTransform()->GetLocalRotation());
		WeaponOutLineRenderer1_->GetTransform()->SetLocalPosition(shortBow_->GetTransform()->GetLocalPosition());
		WeaponOutLineRenderer1_->Off();

		// Weapon2
		WeaponOutLineRenderer2_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponOutLineRenderer2_->SetBaseRenderer(longBow_, "PreprocessingAni", true, false);

		OutLineData Weapon2OutLineData = WeaponOutLineRenderer1_->GetOutLineData();
		float4 Weapon2Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon2OutLineData.LineThickness;
		float4 CalcWeapon2Scale = longBow_->GetTransform()->GetLocalScaling() + Weapon2Thickness;
		WeaponOutLineRenderer2_->GetTransform()->SetLocalScaling(CalcWeapon2Scale);
		WeaponOutLineRenderer2_->GetTransform()->SetLocalRotationDegree(longBow_->GetTransform()->GetLocalRotation());
		WeaponOutLineRenderer2_->GetTransform()->SetLocalPosition(longBow_->GetTransform()->GetLocalPosition());
		WeaponOutLineRenderer2_->GetRenderSet(0).isRender = false;
		WeaponOutLineRenderer2_->Off();
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

		// Weapon1
		WeaponSilhouetteRenderer1_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponSilhouetteRenderer1_->SetBaseRenderer(shortBow_, "PreprocessingAni", true);

		OutLineData Weapon1OutLineData = WeaponSilhouetteRenderer1_->GetOutLineData();
		float4 Weapon1Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon1OutLineData.LineThickness;
		float4 CalcWeapon1Scale = shortBow_->GetTransform()->GetLocalScaling() + Weapon1Thickness;
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalScaling(CalcWeapon1Scale);
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalRotationDegree(shortBow_->GetTransform()->GetLocalRotation());
		WeaponSilhouetteRenderer1_->GetTransform()->SetLocalPosition(shortBow_->GetTransform()->GetLocalPosition());

		// Weapon2
		WeaponSilhouetteRenderer2_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
		WeaponSilhouetteRenderer2_->SetBaseRenderer(longBow_, "PreprocessingAni", true);

		OutLineData Weapon2OutLineData = WeaponSilhouetteRenderer2_->GetOutLineData();
		float4 Weapon2Thickness = float4(1.0f, 1.0f, 1.0f, 0.0f) * Weapon2OutLineData.LineThickness;
		float4 CalcWeapon2Scale = longBow_->GetTransform()->GetLocalScaling() + Weapon2Thickness;
		WeaponSilhouetteRenderer2_->GetTransform()->SetLocalScaling(CalcWeapon2Scale);
		WeaponSilhouetteRenderer2_->GetTransform()->SetLocalRotationDegree(longBow_->GetTransform()->GetLocalRotation());
		WeaponSilhouetteRenderer2_->GetTransform()->SetLocalPosition(longBow_->GetTransform()->GetLocalPosition());
		WeaponSilhouetteRenderer2_->GetRenderSet(0).isRender = false;
	}
}

void Rio::changeAnimationWait()
{
	if (bLongBow_)
	{
		ChangeAnimation("Wait_Long");
	}
	else
	{
		ChangeAnimation("Wait_Short");
	}
}

void Rio::changeAnimationRun()
{
	if (bLongBow_)
	{
		ChangeAnimation("Run_Long");
	}
	else
	{
		ChangeAnimation("Run_Short");
	}
}

void Rio::changeAnimationBasicAttack()
{
	if (bLongBow_)
	{
		ChangeAnimation("BasicAttack_Long", true);
	}
	else
	{
		ChangeAnimation("BasicAttack_Short", true);
	}
}

void Rio::changeAnimationCraft()
{
	ChangeAnimation("Rio_CraftMetal");
}

void Rio::changeAnimationCook()
{
	ChangeAnimation("Rio_CraftFood");
}

void Rio::changeDeathAnimation()
{
	shortBow_->Off();
	longBow_->Off();
	ChangeAnimation("Death", true);
}

void Rio::onStartBasicAttacking(IUnit* _target)
{
	if (bLongBow_)
	{
		float4 offset = { 20.f, 140.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		RioArrow* arrow = level_->CreateActor<RioArrow>();
		arrow->MakeTargetArrow(*this, stat_.AttackPower, startPosition, 1000.f, *target_);
		arrow->SetType(eProjectileType::RioTargetArrow);
	}
	else
	{
		float doubleStrikeDelay = (stat_.AttackEndTime - stat_.AttackStartTime) / stat_.AttackSpeed / 2.0f;

		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		float arrowSpeed = 1500.f;

		{
			PacketCreateProjectile packetArrow;
			packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
			packetArrow.SetType(eProjectileType::RioTargetArrow);
			FT::SendPacket(packetArrow);
		}
		{
			PacketCreateProjectile packetArrow;
			packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
			packetArrow.SetWaitTime(doubleStrikeDelay);
			packetArrow.SetType(eProjectileType::RioTargetArrow);
			FT::SendPacket(packetArrow);
		}

		if (GameServer::GetInstance()->IsOpened())
		{
			RioArrow* arrow = level_->CreateActor<RioArrow>();
			arrow->SetType(eProjectileType::RioTargetArrow);
			arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);

			arrow = level_->CreateActor<RioArrow>();
			arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *target_);
			arrow->SetWaitTime(doubleStrikeDelay);
		}
	}

}

void Rio::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{
}

void Rio::onStartQSkill()
{
	uiController_->GetSkillGauge()->PlayAwake(0.4f, "시전 중");

	overrideAnimationBoneName_ = "Bip001 Spine2";
	if (bLongBow_)
	{
		overrideAnimationName_ = "SkillQ_Long";
		stat_.AttackRange = 450.0f;
	}
	else
	{
		overrideAnimationName_ = "SkillQ_Short";
		stat_.AttackRange = 550.0f;
	}

	renderer_->OverrideFBXAnimation(overrideAnimationName_, overrideAnimationBoneName_);


	//renderer_->OverrideFBXAnimation("SkillQ", "Bip001 L UpperArm");

	FT::PlaySoundAndSendPacket("Rio_Bow_Skill01_BowChange.wav", transform_.GetWorldPosition());

	coolTimer_Q_ = 1.0f;
	bShortRSkillExtra_ = false;
}

void Rio::onUpdateQSkill(float _deltaTime)
{
	inputProcess(_deltaTime);
	moveProcess(_deltaTime);
	if (renderer_->IsOverrideAnimationEnd())
	{
		bLongBow_ = !bLongBow_;

		if (bLongBow_)
		{
			if (!bHidden_)
			{
				longBow_->On();
				shortBow_->Off();
			}
		}
		else
		{
			if (!bHidden_)
			{
				longBow_->Off();
				shortBow_->On();
			}
		}

		overrideAnimationName_ = "";
		overrideAnimationBoneName_ = "";
		renderer_->ClearOverrideAnimation();
		mainState_ << "NormalState";
	}
}

void Rio::onStartWSkill()
{
	if (mouse_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}

	destination_ = transform_.GetWorldPosition();
	destinations_.clear();
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	setRotationTo(mousePosition, transform_.GetWorldPosition());
	if (bLongBow_)
	{
		ChangeAnimation("SkillW_Long", true);
	}
	else
	{
		ChangeAnimation("SkillW_Short", true);
	}

	if (bLongBow_)
	{
		RandomSoundPlay("Rio_PlaySkill1031310Seq0_1_ko.wav", "Rio_PlaySkill1031310Seq0_2_ko.wav", "Rio_PlaySkill1031310Seq0_3_ko.wav");

		FT::PlaySoundAndSendPacket("Rio_LongBow_Skill02_Shot.wav", transform_.GetWorldPosition());

		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		PacketCreateProjectile packetArrow;
		packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower * 2.0f, startPosition, transform_.GetWorldRotation().y, 1500.f);
		packetArrow.SetLifeTime(0.7f);
		packetArrow.SetWaitTime(0.25f);
		packetArrow.SetScale({ 70.f, 150.0f, 100.f });
		packetArrow.SetType(eProjectileType::RioWSkillLong);
		FT::SendPacket(packetArrow);

		if (GameServer::GetInstance()->IsOpened())
		{
			RioArrow* arrow = level_->CreateActor<RioArrow>();
			arrow->SetType(eProjectileType::RioWSkillLong);
			arrow->MakeNonTargetArrow(*this, stat_.AttackPower * 2.0f, startPosition, transform_.GetWorldRotation().y, 1500.f);
			arrow->SetLifeTime(0.7f);
			arrow->SetWaitTime(0.25f);
			arrow->SetScale({ 70.f, 150.0f, 100.f });
		}
	}
	else
	{
		RandomSoundPlay("Rio_PlaySkill1031300Seq0_1_ko.wav", "Rio_PlaySkill1031300Seq0_2_ko.wav", "Rio_PlaySkill1031300Seq0_3_ko.wav");

		FT::PlaySoundAndSendPacket("Rio_ShortBow_Skill02_Shot.wav", transform_.GetWorldPosition());

		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		for (size_t i = 0; i < 5; i++)
		{

			PacketCreateProjectile packetArrow;
			packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, transform_.GetWorldRotation().y - 10.f + 5 * i, 1000.f);
			packetArrow.SetWaitTime(0.05f * i);
			packetArrow.SetLifeTime(0.7f);
			packetArrow.SetType(eProjectileType::RioWSkillShort);
			FT::SendPacket(packetArrow);

		}

		if (GameServer::GetInstance()->IsOpened())
		{
			for (size_t i = 0; i < 5; i++)
			{
				RioArrow* arrow = level_->CreateActor<RioArrow>();
				arrow->SetType(eProjectileType::RioWSkillShort);
				arrow->MakeNonTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, transform_.GetWorldRotation().y - 10.f + 5 * i, 1000.f);
				arrow->SetWaitTime(0.05f * i);
				arrow->SetLifeTime(0.7f);
			}
		}
	}

	//coolTimer_W_ = 12.f;
}

void Rio::onUpdateWSkill(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		mainState_ << "NormalState";
		changeAnimationWait();
	}
}

void Rio::onStartESkill()
{
}

void Rio::onUpdateESkill(float _deltaTime)
{
	if (mouse_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	setRotationTo(mousePosition, transform_.GetWorldPosition());
	mainState_ << "CustomState";
	customState_ << "SkillEBegin";
}

void Rio::onStartRSkill()
{
	if (mouse_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	setRotationTo(mousePosition, transform_.GetWorldPosition());

	RandomSoundPlay("Rio_PlaySkill1031510Seq0_1_ko.wav", "Rio_PlaySkill1031510Seq0_2_ko.wav", "Rio_PlaySkill1031510Seq0_3_ko.wav");

	if (bLongBow_)
	{

		uiController_->GetSkillGauge()->PlayAwake(1.4f, "시전 중");

		ChangeAnimation("SkillR_Long");

		FT::PlaySoundAndSendPacket("Rio_LongBow_SKill04_01.wav", transform_.GetWorldPosition());

		float4 offset = { 20.f, 120.f, 30.f, 0.f };
		offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition += offset;

		PacketCreateProjectile packetArrow;
		packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower * 6.0f, startPosition, transform_.GetWorldRotation().y, 1000.f);
		packetArrow.SetLifeTime(1.0f);
		packetArrow.SetWaitTime(1.4f);
		packetArrow.SetScale({ 100.f, 150.0f, 150.f });
		packetArrow.SetType(eProjectileType::RioRSkillLong);
		FT::SendPacket(packetArrow);

		if (GameServer::GetInstance()->IsOpened())
		{
			RioArrow* arrow = level_->CreateActor<RioArrow>();
			arrow->SetType(eProjectileType::RioRSkillLong);
			arrow->MakeNonTargetArrow(*this, stat_.AttackPower * 6.0f, startPosition, transform_.GetWorldRotation().y, 1000.f);
			arrow->SetLifeTime(1.0f);
			arrow->SetWaitTime(1.4f);
			arrow->SetScale({ 100.f, 150.0f, 150.f });
			arrow->SetKnockback(true);
		}
	}
	else
	{
		if (bShortRSkillExtra_)
		{
			uiController_->GetSkillGauge()->PlayAwake(0.3f, "시전 중");

			ChangeAnimation("SkillR_Short_End");

			float4 offset = { 20.f, 120.f, 30.f, 0.f };
			offset = offset * transform_.GetTransformData().WorldWorld_;
			float4 startPosition = transform_.GetWorldPosition();
			startPosition += offset;

			PacketCreateProjectile packetArrow;
			packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower * 3.0f, startPosition, transform_.GetWorldRotation().y, 1000.f);
			packetArrow.SetLifeTime(0.5f);
			packetArrow.SetWaitTime(0.3f);
			packetArrow.SetScale({ 80.f, 150.0f, 100.f });
			packetArrow.SetType(eProjectileType::RioRSkillShortImpact);
			FT::SendPacket(packetArrow);

			if (GameServer::GetInstance()->IsOpened())
			{
				RioArrow* arrow = level_->CreateActor<RioArrow>();
				arrow->SetType(eProjectileType::RioRSkillShortImpact);
				arrow->MakeNonTargetArrow(*this, stat_.AttackPower * 3.0f, startPosition, transform_.GetWorldRotation().y, 1000.f);
				arrow->SetLifeTime(0.5f);
				arrow->SetWaitTime(0.3f);
				arrow->SetScale({ 80.f, 150.0f, 100.f });
				arrow->SetKnockback(true);
			}
		}
		else
		{

			ChangeAnimation("SkillR_Short_Start");

			float4 offset = { 20.f, 120.f, 30.f, 0.f };
			offset = offset * transform_.GetTransformData().WorldWorld_;
			float4 startPosition = transform_.GetWorldPosition();
			startPosition += offset;

			for (int i = 0; i < 3; i++)
			{
				PacketCreateProjectile packetArrow;
				packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower, startPosition, transform_.GetWorldRotation().y, 1200.f);
				packetArrow.SetLifeTime(0.5f);
				packetArrow.SetWaitTime(0.4f + 0.1f * i);
				packetArrow.SetScale({ 70.f, 150.0f, 100.f });
				packetArrow.SetType(eProjectileType::RioRSkillShort);
				FT::SendPacket(packetArrow);

				if (GameServer::GetInstance()->IsOpened())
				{
					RioArrow* arrow = level_->CreateActor<RioArrow>();
					arrow->SetType(eProjectileType::RioRSkillShort);
					arrow->MakeNonTargetArrow(*this, stat_.AttackPower, startPosition, transform_.GetWorldRotation().y, 1200.f);
					arrow->SetLifeTime(0.5f);
					arrow->SetWaitTime(0.4f + 0.1f * i);
					arrow->SetScale({ 70.f, 150.0f, 100.f });
				}
			}
		}
	}

	skillRTime_ = -0.4f;
}

void Rio::onUpdateRSkill(float _deltaTime)
{
	skillRTime_ += _deltaTime;
	if (renderer_->IsCurrentAnimationEnd())
	{
		changeAnimationWait();
		mainState_ << "NormalState";
	}

	if (attackState_.GetCurrentState()->Time_ < 0.0f)
	{
		return;
	}

	if (bLongBow_)
	{
		if (attackState_.GetCurrentState()->Time_ > 0.5f)
		{
			FT::PlaySoundAndSendPacket("Rio_LongBow_Skill04_02.wav", transform_.GetWorldPosition());

			attackState_.GetCurrentState()->Time_ = -5.f;
		}

	}
	else
	{
		if (bShortRSkillExtra_)
		{
			if (attackState_.GetCurrentState()->Time_ > 0.3f)
			{
				FT::PlaySoundAndSendPacket("Rio_ShortBow_Skill04_02.wav", transform_.GetWorldPosition());
				attackState_.GetCurrentState()->Time_ = -5.f;
				bShortRSkillExtra_ = false;
			}
		}
		else
		{
			if (skillRTime_ > 0.12f)
			{
				FT::PlaySoundAndSendPacket("Rio_ShortBow_Skill04_02.wav", transform_.GetWorldPosition());
				skillRTime_ = 0.0f;
			}

			if (attackState_.GetCurrentState()->Time_ > 0.8f)
			{
				attackState_.GetCurrentState()->Time_ = -5.f;
				bShortRSkillExtra_ = true;
				coolTimer_R_ = 0.0f;
			}
		}
	}


}

void Rio::onStartDSkill()
{
	if (mouse_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}

	destination_ = transform_.GetWorldPosition();
	destinations_.clear();
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	setRotationTo(mousePosition, transform_.GetWorldPosition());

	if (bLongBow_)
	{
		ChangeAnimation("SkillD_Long", true);
	}
	else
	{
		ChangeAnimation("SkillD_Short", true);
	}

	FT::PlaySoundAndSendPacket("skillReadyBow_in.wav", transform_.GetWorldPosition());
	

	PacketCreateProjectile packetArrow;
	packetArrow.MakeNonTargetProjectile(*this, stat_.AttackPower * 5, mousePosition, transform_.GetWorldRotation().y, 1200.f);
	packetArrow.SetType(eProjectileType::RioDSkill);
	FT::SendPacket(packetArrow);

	if (GameServer::GetInstance()->IsOpened())
	{
		RioDSkill* skill = level_->CreateActor<RioDSkill>();
		skill->SetOwner(this);
		skill->GetTransform()->SetWorldPosition(mousePosition);
		skill->SetDamage(stat_.AttackPower * 5);
		skill->SetWaitTime(0.0f);


		dSkillShotEffect_->GetTransform()->SetWorldPosition(transform_.GetWorldPosition());
		dSkillShotEffect_->GetTransform()->SetLocalRotationDegree(transform_.GetLocalRotation());
		dSkillShotEffect_->PlayAwake();
	}
	
		CharEffectPacket pack;
		pack.SetTargetIndex(myIndex_);
		pack.SetAnimationName("SkillDShot");
		FT::SendPacket(pack);

		CharEffectPacket pack1;
		pack1.SetTargetIndex(myIndex_);
		pack1.SetAnimationName("SkillD");
		pack1.SetWorldPos(mousePosition);
		FT::SendPacket(pack1);

}

void Rio::onUpdateDSkill(float _deltaTime)
{


	if (attackState_.GetCurrentState()->Time_ > 0.2f)
	{
		FT::PlaySoundAndSendPacket("attackBow_in_r1.wav", transform_.GetWorldPosition());

		attackState_.GetCurrentState()->Time_ = -10.f;
	}

	if (renderer_->IsCurrentAnimationEnd())
	{
		changeAnimationWait();
		mainState_ << "NormalState";
	}
}

void Rio::onStartDeath()
{
	changeDeathAnimation();
}

void Rio::onUpdateDeath(float _deltaTime)
{
}

void Rio::onUpdateCustomState(float _deltaTime)
{
	customState_.Update(_deltaTime);
}

void Rio::onPlayEffect(const std::string& _effectName, IUnit* _victim, float4 _pos)
{
	float4 startPosition = transform_.GetWorldPosition();
	float arrowSpeed = 1500.f;

	//if ("BasicAttack_Shot" == _effectName)
	//{
	//	basicAttackEffect_->GetAttackRenderer()->SetColor({ 1.0f, 1.0f, 0.7f });
	//	basicAttackEffect_->PlayAwake("FX_BI_Shoot_01");
	//
	//	//bullet0_ = level_->CreateActor<AyaBullet>();
	//	//bullet0_->SetDummy();
	//	//bullet0_->MakeTarget(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *_victim);
	//	return;
	//}
	if ("BasicAttack_Hit0" == _effectName)
	{
		if (_victim != nullptr)
		{
			float4 wp = _victim->GetTransform()->GetWorldPosition();
			wp.y -= 50.0f;

			float4 wr = _victim->GetTransform()->GetLocalRotation();
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


	if ("BasicAttack_Hit" == _effectName)
	{
		if (_victim != nullptr)
		{
			float4 wp = _victim->GetTransform()->GetWorldPosition();
			wp.y -= 50.0f;

			float4 wr = _victim->GetTransform()->GetLocalRotation();
			//	bullet0_ = level_->CreateActor<AyaBullet>();
			//	bullet0_->SetDummy();
			//	bullet0_->GetAttackEffect()->GetTransform()->SetWorldPosition(wp);
			basicHitEffect_->GetAttackRenderer()->SetColor({ 1.0f,1.0f,0.7f });
			basicHitEffect_->GetTransform()->SetWorldPosition(wp);
			basicHitEffect_->GetTransform()->SetLocalRotationDegree(GetTransform()->GetLocalRotation());
			basicHitEffect_->PlayFade(0.3f);
		}
		//bullet0_->GetAttackEffect()->PlayAwake("Fx_ShootGlowSE_04");
		return;
	}

	if ("wWind" == _effectName)
	{
		float4 wp = _victim->GetTransform()->GetWorldPosition();
		RioWSkillWind* wind = GetLevel()->CreateActor<RioWSkillWind>();
		wind->SetParentIndex(GetIndex());
		wind->GetTransform()->SetWorldPosition(wp);
		wind->PlayAwake(3.0f);

	}

	if ("SkillDShot" == _effectName)
	{
		float4 wr = transform_.GetLocalRotation();

		dSkillShotEffect_->GetTransform()->SetWorldPosition(startPosition);
		dSkillShotEffect_->GetTransform()->SetLocalRotationDegree(wr);
		dSkillShotEffect_->PlayAwake();

	}

	if ("SkillD" == _effectName)
	{
		RioDSkill* skill = level_->CreateActor<RioDSkill>();
		skill->SetOwner(this);
		skill->GetTransform()->SetWorldPosition(_pos);
		skill->SetDamage(stat_.AttackPower * 5);
		skill->SetWaitTime(0.0f);

		//dSkillEffect_->PlayAwake();
		//dSkillEffect_->GetTransform()->SetWorldPosition(transform_.GetWorldPosition());
	}
}

void Rio::onEffectTransformCheck(float _deltaTime)
{

}

void Rio::onLevelUp()
{
	switch (stat_.Level_q)
	{
	case 1:
		DefaultCool_Q_ = 1.0f;
		DefaultCool_W_ = 12.0f;
		DefaultCool_E_ = 8.0f;
		DefaultCool_R_ = 80.0f;
		DefaultCool_D_ = 25.0f;
		stat_.SP_Q = 0.0f;
		stat_.SP_W = 50.0f;
		stat_.SP_E = 50.0f;
		stat_.SP_R = 100.0f;
		break;
	case 2:
		DefaultCool_W_ = 11.0f;
		DefaultCool_E_ = 7.0f;
		DefaultCool_R_ = 70.0f;
		DefaultCool_D_ = 15.0f;

		stat_.SP_W = 60.0f;
		stat_.SP_E = 60.0f;
		stat_.SP_R = 120.0f;
		break;
	case 3:
		DefaultCool_W_ = 10.0f;
		DefaultCool_E_ = 6.0f;
		DefaultCool_R_ = 60.0f;

		stat_.SP_W = 70.0f;
		stat_.SP_E = 70.0f;
		stat_.SP_R = 140.0f;
		break;
	case 4:
		DefaultCool_W_ = 9.0f;
		DefaultCool_E_ = 5.0f;
		stat_.SP_W = 80.0f;
		stat_.SP_E = 80.0f;
		break;
	case 5:
		DefaultCool_W_ = 8.0f;
		DefaultCool_E_ = 4.0f;
		stat_.SP_W = 90.0f;
		stat_.SP_E = 90.0f;
		break;
	default:
		break;
	}
}

void Rio::startSkillEBegin()
{
	float height = 0.0f;
	startPosition_ = transform_.GetWorldPosition();
	landingPosition_ = transform_.GetWorldPosition();
	landingPosition_ += transform_.GetWorldForwardVector() * 500.f;

	float4 landingPosition = landingPosition_;
	landingPosition.y += FT::Map::MAX_HEIGHT;

	RandomSoundPlay("Rio_PlaySkill1031400Seq0_1_ko.wav", "Rio_PlaySkill1031400Seq0_2_ko.wav", "Rio_PlaySkill1031400Seq0_3_ko.wav");

	bSkillEPassable_ = currentMap_->GetNavMesh()->CheckIntersects(landingPosition, float4::DOWN, height);

	ChangeAnimation("SkillE_Short");
	skillETime_ = 0.0f;
}

void Rio::updateSkillEBegin(float _deltaTime)
{
	skillETime_ += _deltaTime;
	if (customState_.GetCurrentState()->Time_ > 0.2f)
	{
		customState_ << "SkillEShot";
	}

	if (bSkillEPassable_)
	{
		transform_.SetWorldPosition(float4::Lerp(startPosition_, landingPosition_, skillETime_ / 0.5f));
	}
	else
	{
		float4 moveSpeed = (direction_ * stat_.MovementSpeed * 2.0f) * _deltaTime;
		float4 nextMovePosition = transform_.GetWorldPosition() + moveSpeed;

		float temp;
		if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
		{
			GetTransform()->SetWorldPosition(nextMovePosition);
		}
	}
}

void Rio::startSkillEShot()
{
	FT::PlaySoundAndSendPacket("Rio_Bow_Skill03_Start.wav", transform_.GetWorldPosition());

	{
		auto list = skillECollision_->GetCollisionList(eCollisionGroup::Player);

		float doubleStrikeDelay = (stat_.AttackEndTime - stat_.AttackStartTime) / stat_.AttackSpeed / 2.0f;

		//float4 offset = { 20.f, 120.f, 30.f, 0.f };
		//offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition.y += 300.f;
		//startPosition += offset;

		float arrowSpeed = 1500.f;

		for (GameEngineCollision* col : list)
		{
			Character* c = dynamic_cast<Character*>(col->GetActor());
			if (nullptr != c)
			{
				if (c == this)
				{
					continue;
				}
				{
					PacketCreateProjectile packetArrow;
					packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);
					packetArrow.SetType(eProjectileType::RioTargetArrow);
					FT::SendPacket(packetArrow);
				}
				{
					PacketCreateProjectile packetArrow;
					packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);
					packetArrow.SetWaitTime(doubleStrikeDelay);
					packetArrow.SetType(eProjectileType::RioTargetArrow);
					FT::SendPacket(packetArrow);
				}

				if (GameServer::GetInstance()->IsOpened())
				{
					RioArrow* arrow = level_->CreateActor<RioArrow>();
					arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);

					arrow = level_->CreateActor<RioArrow>();
					arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);
					arrow->SetWaitTime(doubleStrikeDelay);
				}
			}
		}
	}

	{
		auto list = skillECollision_->GetCollisionList(eCollisionGroup::Monster);

		float doubleStrikeDelay = (stat_.AttackEndTime - stat_.AttackStartTime) / stat_.AttackSpeed / 2.0f;

		//float4 offset = { 20.f, 120.f, 30.f, 0.f };
		//offset = offset * transform_.GetTransformData().WorldWorld_;
		float4 startPosition = transform_.GetWorldPosition();
		startPosition.y += 300.f;
		//startPosition += offset;

		float arrowSpeed = 1500.f;

		for (GameEngineCollision* col : list)
		{
			IUnit* c = dynamic_cast<IUnit*>(col->GetActor());
			if (nullptr != c)
			{
				if (c == this)
				{
					continue;
				}
				{
					PacketCreateProjectile packetArrow;
					packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);
					packetArrow.SetType(eProjectileType::RioTargetArrow);
					FT::SendPacket(packetArrow);
				}
				{
					PacketCreateProjectile packetArrow;
					packetArrow.MakeTargetProjectile(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);
					packetArrow.SetWaitTime(doubleStrikeDelay);
					packetArrow.SetType(eProjectileType::RioTargetArrow);
					FT::SendPacket(packetArrow);
				}

				if (GameServer::GetInstance()->IsOpened())
				{
					RioArrow* arrow = level_->CreateActor<RioArrow>();
					arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);

					arrow = level_->CreateActor<RioArrow>();
					arrow->MakeTargetArrow(*this, stat_.AttackPower / 2.0f, startPosition, arrowSpeed, *c);
					arrow->SetWaitTime(doubleStrikeDelay);
				}
			}
		}
	}
}

void Rio::updateSkillEShot(float _deltaTime)
{
	if (customState_.GetCurrentState()->Time_ > 0.2f)
	{
		customState_ << "SkillEEnd";
	}

	skillETime_ += _deltaTime;
	if (bSkillEPassable_)
	{

		if (bSkillEPassable_)
		{
			transform_.SetWorldPosition(float4::Lerp(startPosition_, landingPosition_, skillETime_ / 0.5f));
		}
	}
}

void Rio::startSkillEEnd()
{
	FT::PlaySoundAndSendPacket("Rio_Bow_Skill03_End.wav", transform_.GetWorldPosition());
}

void Rio::updateSkillEEnd(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd() || skillETime_ > 0.6f)
	{
		destination_ = transform_.GetWorldPosition();
		destinations_.clear();
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}


	skillETime_ += _deltaTime;
	if (bSkillEPassable_)
	{
		if (bSkillEPassable_)
		{
			transform_.SetWorldPosition(float4::Lerp(startPosition_, landingPosition_, skillETime_ / 0.5f));
		}
	}
}

void Rio::changeAnimationOperate()
{
	ChangeAnimation("Operate");
}

void Rio::changeAnimationArrive()
{
	ChangeAnimation("Arrive");
}

void Rio::changeAnimationRestStart()
{
	ChangeAnimation("RestStart");
}

void Rio::changeAnimationRestLoop()
{
	ChangeAnimation("RestLoop");
}

void Rio::changeAnimationRestEnd()
{
	ChangeAnimation("RestEnd");
}

void Rio::changeAnimationGather()
{
	ChangeAnimation("Collect");
}

