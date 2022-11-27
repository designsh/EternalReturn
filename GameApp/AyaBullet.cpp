#include "PreCompile.h"
#include "AyaBullet.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Character.h"
#include "PacketSoundPlay.h"
#include "Monsters.h"
#include "CharEffectPacket.h"
AyaBullet::AyaBullet()
	: renderer_(nullptr)
	, collision_(nullptr)
	, target_(nullptr)
	, damage_(0.0f)
	, rotationY_(0.0f)
	, speed_(0.0f)
	, waitTime_(0.0f)
	, owner_(nullptr)
	, lifeTime_(0.5f)
	, scale_(float4(5.f, 150.f, 50.f))
	, bKnockback_(false)
	, effect_(nullptr)
	, atkEffect_(nullptr)
{
}

AyaBullet::~AyaBullet()
{
}

void AyaBullet::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::Projectile);
	collision_->SetCollisionType(CollisionType::OBBBox3D);
	collision_->Off();

	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("Fx_ShootGlowSE_04.png");
	hitBase->Cut(2, 2);

	atkEffect_ = GetLevel()->CreateActor<BasicAttackEffect>();
	atkEffect_->GetAttackRenderer()->SetImage("Fx_ShootGlowSE_04.png", "LinerSmp");
	atkEffect_->GetAttackRenderer()->GetTransform()->SetLocalPosition({ 0.0f,50.0f,0.0f }); 
	atkEffect_->GetAttackRenderer()->GetTransform()->SetLocalRotationDegree({ 90.0f,0.0f,0.0f });
	atkEffect_->GetAttackRenderer()->GetTransform()->SetLocalScaling(atkEffect_->GetAttackRenderer()->GetCurrentTexture()->GetTextureSize() * 2.5f);
	atkEffect_->GetAttackRenderer()->CreateAnimation("Fx_ShootGlowSE_04.png", "Fx_ShootGlowSE_04", 0, 3, 0.08f, false);

	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("bullet.fbx", "Color", false);
	renderer_->GetRenderSet(0).ShaderHelper->SettingConstantBufferSet("ResultColor", float4{1.0f, 0.8f, 0.0f});

	renderer_->GetTransform()->SetLocalScaling(float4{4.5f,4.5f,9.0f} * 0.0005f);
	renderer_->GetTransform()->SetLocalPosition(float4{0.0f,500.0f,0.0f } *0.0005f);
	renderer_->Off();

	GetTransform()->SetLocalScaling(scale_);

	state_.CreateState(MakeState(AyaBullet, Chase));
	state_.CreateState(MakeState(AyaBullet, Fly));
	state_.CreateState(MakeState(AyaBullet, Wait));

	state_ << "Wait";
}

void AyaBullet::Update(float _deltaTime)
{
	if (collision_->IsUpdate())
	{
		level_->PushDebugRender(GetTransform(), CollisionType::OBBBox3D);
	}

	state_.Update(_deltaTime);
}

void AyaBullet::MakeTarget(IUnit& _owner, float _damage, const float4& _position, float _speed, IUnit& _target)
{
	owner_ = &_owner;
	target_ = &_target;
	damage_ = _damage;
	transform_.SetWorldPosition(_position);
	speed_ = _speed;

	float4 destination = target_->GetTransform()->GetWorldPosition();
	destination.y += 120.f;
	float4 direction = destination - transform_.GetWorldPosition();
	direction.Normalize3D();

	setRotationTo(destination, transform_.GetWorldPosition());
}

void AyaBullet::MakeNonTarget(IUnit& _owner, float _damage, const float4& _position, float _rotationY, float _speed)
{
	owner_ = &_owner;
	target_ = nullptr;
	damage_ = _damage;
	transform_.SetWorldPosition(_position);
	speed_ = _speed;
	rotationY_ = _rotationY;
}

void AyaBullet::startWait()
{
}

void AyaBullet::updateWait(float _deltaTime)
{
	waitTime_ -= _deltaTime;

	if (waitTime_ > 0.f)
	{
		return;
	}

	if (target_ == nullptr)
	{
		state_ << "Fly";
	}
	else
	{
		state_ << "Chase";
	}
}

void AyaBullet::startChase()
{
	collision_->On();
	renderer_->On();
}

void AyaBullet::updateChase(float _deltaTime)
{
	float4 destination = target_->GetTransform()->GetWorldPosition();
	destination.y += 120.f;
	float4 direction = destination - transform_.GetWorldPosition();
	direction.Normalize3D();

	setRotationTo(destination, transform_.GetWorldPosition());

	transform_.AddWorldPosition(direction * speed_ * _deltaTime);

	if (float4::Calc_Len3D(destination, transform_.GetWorldPosition()) <= FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	{
		if (damage_ > 0.0f)
		{
			target_->Damage(damage_, owner_);

			//atkEffect_->GetAttackRenderer()->SetColor({ 1.0f,1.0f,0.7f });
			//atkEffect_->GetTransform()->SetWorldPosition(wp);
			//atkEffect_->PlayAwake("Fx_ShootGlowSE_04");
		}

			CharEffectPacket pack0;
			pack0.SetTargetIndex(owner_->GetIndex());
			pack0.SetAnimationName("BasicAttack_Hit0");
			pack0.SetVictimIndex(*target_);
			FT::SendPacket(pack0);


			CharEffectPacket pack;
			pack.SetTargetIndex(owner_->GetIndex());
			pack.SetAnimationName("BasicAttack_Hit");
			pack.SetVictimIndex(*target_);
			FT::SendPacket(pack);

		Release();
	}
}

void AyaBullet::startFly()
{
	transform_.SetLocalRotationDegree({ 0.0f, rotationY_, 0.0f });
	collision_->On();
	renderer_->On();
}

void AyaBullet::updateFly(float _deltaTime)
{
	lifeTime_ -= _deltaTime;
	if (lifeTime_ < 0)
	{
		Release();
		return;
	}

	transform_.AddWorldPosition(transform_.GetWorldForwardVector() * speed_ * _deltaTime);

	auto collisionList = collision_->GetCollisionList(eCollisionGroup::Player);
	for (GameEngineCollision* col : collisionList)
	{
		if (col->GetActor() != owner_)
		{
			Character* opponent = dynamic_cast<Character*>(col->GetActor());

			if (opponent != nullptr)
			{

				opponent->Damage(damage_, owner_);

				CharEffectPacket pack0;
				pack0.SetTargetIndex(owner_->GetIndex());
				pack0.SetAnimationName("BasicAttack_Hit0");
				pack0.SetVictimIndex(*opponent);
				FT::SendPacket(pack0);


				CharEffectPacket pack;
				pack.SetTargetIndex(owner_->GetIndex());
				pack.SetAnimationName("BasicAttack_Hit");
				pack.SetVictimIndex(*opponent);
				FT::SendPacket(pack);

				atkEffect_->GetAttackRenderer()->SetColor({ 0.8f,0.8f,1.0f });
				//atkEffect_->GetTransform()->SetWorldPosition(wp);
				atkEffect_->PlayAwake("Fx_ShootGlowSE_04");

				//CharEffectPacket pack;
				//pack.SetTargetIndex(owner_->GetIndex());
				//pack.SetAnimationName("SkillW_Hit");
				//pack.SetVictimIndex(*opponent);
				//FT::SendPacket(pack);
		

				// 여기 이펙트 패킷 하나
				// sound

				Release();
				return;
			}
		}
	}

	collisionList.clear();
	collisionList = collision_->GetCollisionList(eCollisionGroup::Monster);
	for (GameEngineCollision* col : collisionList)
	{
		if (col->GetActor() != owner_)
		{
			Monsters* opponent = dynamic_cast<Monsters*>(col->GetActor());

			if (opponent != nullptr)
			{

				opponent->Damage(damage_, owner_);

				CharEffectPacket pack0;
				pack0.SetTargetIndex(owner_->GetIndex());
				pack0.SetAnimationName("BasicAttack_Hit0");
				pack0.SetVictimIndex(*opponent);
				FT::SendPacket(pack0);


				CharEffectPacket pack;
				pack.SetTargetIndex(owner_->GetIndex());
				pack.SetAnimationName("BasicAttack_Hit");
				pack.SetVictimIndex(*opponent);
				FT::SendPacket(pack);

				atkEffect_->GetAttackRenderer()->SetColor({ 0.8f,0.8f,1.0f });
				//atkEffect_->GetTransform()->SetWorldPosition(wp);
				atkEffect_->PlayAwake("Fx_ShootGlowSE_04");

				//CharEffectPacket pack;
				//pack.SetTargetIndex(owner_->GetIndex());
				//pack.SetAnimationName("SkillW_Hit");
				//pack.SetVictimIndex(*opponent);
				//FT::SendPacket(pack);


				// sound

				Release();
				return;
			}
		}
	}
}

void AyaBullet::setRotationTo(const float4& _destination, const float4 _startPosition)
{
	float4 direction = _destination - _startPosition;
	direction.Normalize3D();

	float4 cross = float4::Cross3D(direction, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction, { 0.0f, 0.0f, 1.0f });

	transform_.SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });
}
