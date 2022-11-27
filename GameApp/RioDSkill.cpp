#include "PreCompile.h"
#include "RioDSkill.h"

#include "GameEngine/GameEngineCollision.h"
#include "IUnit.h"
#include "RioDSkillEffect.h"
RioDSkill::RioDSkill()
	: waitTime_(0.0f)
	, damage_(0.0f)
	, collision_(nullptr)
	, owner_(nullptr)
	, soundCount_(4)
	, bShotSound_(false)
{

}

RioDSkill::~RioDSkill()
{

}

void RioDSkill::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionType(CollisionType::Sphere3D);
	collision_->GetTransform()->SetLocalScaling(800.0f);

	state_.CreateState(MakeState(RioDSkill, Wait));
	state_.CreateState(MakeState(RioDSkill, Falling));
	state_.CreateState(MakeState(RioDSkill, Fall));

	state_ << "Wait";

	effect_ = GetLevel()->CreateActor<RioDSkillEffect>();

}

void RioDSkill::Update(float _deltaTime)
{
	level_->PushDebugRender(collision_->GetTransform(), CollisionType::CirCle);

	state_.Update(_deltaTime);
}

void RioDSkill::startWait()
{
	effect_->PlayAwake();
	effect_->GetTransform()->SetWorldPosition(transform_.GetWorldPosition());

	float4 skillPosition = transform_.GetWorldPosition();
	float4 ownerPosition = owner_->GetTransform()->GetWorldPosition();
	float length = float4::Calc_Len3D(skillPosition, ownerPosition);

	//if (length > 700.f)
	//{
	//	float4 direction = skillPosition - ownerPosition;
	//	direction.Normalize3D();
	//	transform_.SetWorldPosition(ownerPosition + direction * 700.0f);
	//}
}

void RioDSkill::updateWait(float _deltaTime)
{
	waitTime_ -= _deltaTime;

	if (waitTime_ < 0.f)
	{

		state_ << "Falling";
	}
}

void RioDSkill::startFalling()
{

	effect_->PlayShot();
}

void RioDSkill::updateFalling(float _deltaTime)
{
	if (state_.GetCurrentState()->Time_ > 0.32f && false == bShotSound_)
	{
		FT::PlaySoundAndSendPacket("skillBow_fire.wav", transform_.GetWorldPosition());
		FT::PlaySoundAndSendPacket("skillBow_in.wav", transform_.GetWorldPosition());

		bShotSound_ = true;
	}

	if (state_.GetCurrentState()->Time_ > 3.0f)
	{
		state_ << "Fall";
	}
}

void RioDSkill::startFall()
{
	{
		auto list = collision_->GetCollisionList(eCollisionGroup::Player);
		for (GameEngineCollision* col : list)
		{
			IUnit* unit = dynamic_cast<IUnit*>(col->GetActor());

			if (unit == owner_)
			{
				continue;
			}

			unit->Damage(damage_, owner_);
			unit->Slow(1.0f, 0.5f);
		}
	}

	{
		auto list = collision_->GetCollisionList(eCollisionGroup::Monster);
		for (GameEngineCollision* col : list)
		{
			IUnit* unit = dynamic_cast<IUnit*>(col->GetActor());

			if (unit == owner_)
			{
				continue;
			}

			unit->Damage(damage_, owner_);
			unit->Slow(1.0f, 0.5f);
		}
	}

}

void RioDSkill::updateFall(float _deltaTime)
{
//	if (soundCount_ < 0)
//	{
//		Release();
//	}
//
//	if (state_.GetCurrentState()->Time_ > 0.07f)
//	{
//		FT::PlaySoundAndSendPacket("Rio_ShortBow_Hit_01.wav", transform_.GetWorldPosition());
//		state_.GetCurrentState()->Time_ = 0.0f;
//		soundCount_--;
//	}


	if (state_.GetCurrentState()->Time_ > 2.0f)
	{
		Release();
	}

}
