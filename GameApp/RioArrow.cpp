#include "PreCompile.h"
#include "RioArrow.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "CharEffectPacket.h"
#include "BasicAttackEffect.h"
#include "Character.h"
#include "PacketSoundPlay.h"
#include "RioWSkillWind.h"
#include "Monsters.h"

RioArrow::RioArrow()
	: collision_(nullptr)
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
{

}

RioArrow::~RioArrow()
{

}

void RioArrow::Start()
{

	//renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	//renderer_->SetFBXMesh("Rio_000_Arrow.fbx", "TextureDeferredLight", false);
	//renderer_->GetTransform()->SetLocalScaling(float4{ 9.f,9.f,18.f } * 0.2f);
	//renderer_->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f, 0.0f });
	//renderer_->GetTransform()->SetLocalPosition(float4{ 0.0f, 10.f, 2000.0f } * 0.001f);

	effect_ = CreateTransformComponent<GameEngineEffectRenderer>();
	effect_->SetImage("FX_BI_HitGlow_04.png", "LinearSmp");
	effect_->GetTransform()->SetLocalRotationDegree({ 0.f,90.f,0.f });
	effect_->GetTransform()->SetLocalPosition(float4{ 0.0f, 10.0f ,0.0f } * 0.001f);
	effect_->GetTransform()->SetLocalScaling({ 5.f, 1.5f });
	effect_->SetColor({ 1.0f,1.0f,0.7f });
	effect_->Off();

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::Projectile);
	collision_->SetCollisionType(CollisionType::OBBBox3D);
	collision_->Off();

	GetTransform()->SetLocalScaling(scale_);

	state_.CreateState(MakeState(RioArrow, Chase));
	state_.CreateState(MakeState(RioArrow, Fly));
	state_.CreateState(MakeState(RioArrow, Wait));

	state_ << "Wait";
}

void RioArrow::Update(float _deltaTime)
{
	if (collision_->IsUpdate())
	{
		level_->PushDebugRender(GetTransform(), CollisionType::OBBBox3D);
	}

	state_.Update(_deltaTime);
}

void RioArrow::MakeTargetArrow(IUnit& _owner, float _damage, const float4& _position, float _speed, IUnit& _target)
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

void RioArrow::MakeNonTargetArrow(IUnit& _owner, float _damage, const float4& _position, float _rotationY, float _speed)
{
	owner_ = &_owner;
	target_ = nullptr;
	damage_ = _damage;
	transform_.SetWorldPosition(_position);
	speed_ = _speed;
	rotationY_ = _rotationY;
}

void RioArrow::startWait()
{
	effect_->Off();
}

void RioArrow::updateWait(float _deltaTime)
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

void RioArrow::startChase()
{
	FT::PlaySoundAndSendPacket("Rio_ShortBow_NormalAttack_01.wav", transform_.GetWorldPosition());
	collision_->On();
	effect_->On();
}

void RioArrow::updateChase(float _deltaTime)
{
	//float4 destination = target_->GetTransform()->GetWorldPosition();
	//destination.y += 50.f;
	//setRotationTo(destination, transform_.GetWorldPosition());

	//transform_.AddWorldPosition(transform_.GetWorldForwardVector() * speed_ * _deltaTime);

	//if (float4::Calc_Len3D(destination, transform_.GetWorldPosition()) < FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	//{
	//	target_->Damage(damage_);
	//	Release();
	//}

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
			FT::PlaySoundAndSendPacket("Rio_ShortBow_Hit_01.wav", transform_.GetWorldPosition());

			target_->Damage(damage_, owner_);
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

void RioArrow::startFly()
{
	transform_.SetLocalRotationDegree({ 0.0f, rotationY_, 0.0f });
	collision_->On();
	effect_->On();
	if (bKnockback_)
	{
		FT::PlaySoundAndSendPacket("Rio_LongBow_Skill04_Projectile.wav", transform_.GetWorldPosition());
	}
}

void RioArrow::updateFly(float _deltaTime)
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

				if (bKnockback_)
				{
					opponent->WallSlam(0.25f, transform_.GetWorldForwardVector() * 1000.f, 0.0f);
					FT::PlaySoundAndSendPacket("Rio_LongBow_Skill04_Hit.wav", transform_.GetWorldPosition());
				}
				else
				{
					FT::PlaySoundAndSendPacket("Rio_ShortBow_Skill02_Hit.wav", transform_.GetWorldPosition());
				}

				if (type_ == eProjectileType::RioWSkillShort)
				{
					RioWSkillWind* wind = GetLevel()->CreateActor<RioWSkillWind>();
					wind->SetParentIndex(owner_->GetIndex());

					wind->GetTransform()->SetWorldPosition(opponent->GetTransform()->GetWorldPosition());

					wind->PlayAwake(3.0f, 0.3f);

					CharEffectPacket pack;
					pack.SetTargetIndex(owner_->GetIndex());
					pack.SetAnimationName("wWind");
					pack.SetVictimIndex(*opponent);
					FT::SendPacket(pack);
				}

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

				if (bKnockback_)
				{
					opponent->WallSlam(0.25f, transform_.GetWorldForwardVector() * 1000.f, 0.0f);
					FT::PlaySoundAndSendPacket("Rio_LongBow_Skill04_Hit.wav", transform_.GetWorldPosition());
				}
				else
				{
					FT::PlaySoundAndSendPacket("Rio_ShortBow_Skill02_Hit.wav", transform_.GetWorldPosition());
				}

				Release();
				return;
			}
		}
	}
}

void RioArrow::setRotationTo(const float4& _destination, const float4 _startPosition)
{
	float4 direction = _destination - _startPosition;
	direction.Normalize3D();

	float4 cross = float4::Cross3D(direction, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction, { 0.0f, 0.0f, 1.0f });

	transform_.SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });
}
