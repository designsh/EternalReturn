#include "PreCompile.h"
#include "Dummy.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineCollision.h>
#include "CharStatPacket.h"

#include "GameServer.h"
#include "GameClient.h"

Dummy::Dummy()
	: timer_collision_Q(0.0f), timer_end_Q(0.0f), collision_Q(nullptr), b_Qhit_(false)
{

}

Dummy::~Dummy()
{

}

void Dummy::LoadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("EternalReturn");
	dir / "Resources" / "FBX" / "PJW";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_run.fbx"));
	mesh->CreateRenderingBuffer();

	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_wait.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_death.fbx"));

}

void Dummy::ReleaseResource()
{
	GameEngineFBXMeshManager::GetInst().Delete("Hyunwoo_run.fbx");

	GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_wait.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Hyunwoo_death.fbx");


}

void Dummy::Start()
{
	Character::Start();
	initDummyCollision();
}

void Dummy::Update(float _deltaTime)
{
	Character::Update(_deltaTime);

}

void Dummy::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Hyunwoo_run.fbx", "TextureDeferredLightAni", true);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Hyunwoo_run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "Hyunwoo_wait.fbx", 0);
	renderer_->CreateFBXAnimation("Death", "Hyunwoo_death.fbx", 0, false);
	renderer_->CreateFBXAnimation("Atk0", "Hyunwoo_atk0.fbx", 0, false);
	//renderer_->CreateFBXAnimation("Atk1", "Hyunwoo_atk1.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "Hyunwoo_skillQ.fbx", 0, false);
	//renderer_->CreateFBXAnimation("SkillE_start", "Dummy_skillE_start.fbx", 0, false);
	//renderer_->CreateFBXAnimation("SkillE_loop", "Dummy_skillE_loop.fbx", 0);
	//renderer_->CreateFBXAnimation("SkillE_end", "Dummy_skillE_end.fbx", 0, false);
	//renderer_->CreateFBXAnimation("SkillR_start", "Dummy_skillR_start.fbx", 0, false);
	//renderer_->CreateFBXAnimation("SkillR_loop", "Dummy_skillR_loop.fbx", 0);
	//renderer_->CreateFBXAnimation("SkillR_end", "Dummy_skillR_end.fbx", 0, false);

	renderer_->ChangeFBXAnimation("Wait");
}


void Dummy::initDummyCollision()
{
	collision_Q = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_Q->GetTransform()->SetLocalPosition({ 0.0f,0.0f,200.0f });
	collision_Q->GetTransform()->SetLocalScaling({ 300.0f, 1.0f, 250.0f });
	collision_Q->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_Q->SetCollisionType(CollisionType::AABBBox3D);
	collision_Q->Off();

}

void Dummy::changeAnimationRun()
{
	curAnimationName_ = "Run";
	renderer_->ChangeFBXAnimation("Run");
}

void Dummy::changeAnimationWait()
{
	curAnimationName_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Dummy::changeAnimationBasicAttack()
{
	curAnimationName_ = "Atk0";
	renderer_->ChangeFBXAnimation("Atk0", true);
}





void Dummy::onStartQSkill()
{

}

void Dummy::onUpdateQSkill(float _deltaTime)
{

}

void Dummy::onStartWSkill()
{
}

void Dummy::onUpdateWSkill(float _deltaTime)
{
}

void Dummy::onStartESkill()
{
}

void Dummy::onUpdateESkill(float _deltaTime)
{
}

void Dummy::onStartRSkill()
{
}

void Dummy::onUpdateRSkill(float _deltaTime)
{
}

void Dummy::onStartDSkill()
{
}

void Dummy::onUpdateDSkill(float _deltaTime)
{
}

void Dummy::onStartDeath()
{
	curAnimationName_ = "Death";
	renderer_->ChangeFBXAnimation("Death", true);
}

void Dummy::onUpdateDeath(float _deltaTime)
{

}