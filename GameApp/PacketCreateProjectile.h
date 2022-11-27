#pragma once

#include <GameEngine/GameEnginePacketBase.h>
#include "IUnit.h"
#include "eProjectileType.h"

class LumiaLevel;
class PacketCreateProjectile : public GameEnginePacketBase
{
public:
	PacketCreateProjectile(); 
	~PacketCreateProjectile();

	PacketCreateProjectile(const PacketCreateProjectile& _other) = delete; 
	PacketCreateProjectile(PacketCreateProjectile&& _other) = delete; 

	PacketCreateProjectile& operator=(const PacketCreateProjectile& _other) = delete;
	PacketCreateProjectile& operator=(const PacketCreateProjectile&& _other) = delete;

public:
	void MakeTargetProjectile(IUnit& _owner, float _damage, const float4& _position, float _speed, IUnit& _target);
	void MakeNonTargetProjectile(IUnit& _owner, float _damage, const float4& _position, float _rotationY, float _speed);
	
	void SetOwner(IUnit& _owner) { ownerIndex_ = _owner.GetIndex(); }
	void SetTarget(IUnit& _target) { targetIndex_ = _target.GetIndex(); }
	void SetDamage(float _damage) { damage_ = _damage; }
	void SetWaitTime(float _waitTime) { waitTime_ = _waitTime; }
	void SetLifeTime(float _lifeTime) { lifeTime_ = _lifeTime; }
	void SetScale(const float4& _scale) { scale_ = _scale; }
	void SetType(eProjectileType _type) { projectileType_ = static_cast<int>(_type); }
	void SetTargetType(UnitType _type) { targetType_ = static_cast<int>(_type); }

protected:
	virtual void initPacketID() override;
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	void rioTargetArrow(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer);
	void rioWSkillShort(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer);
	void rioWSkillLong(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer);
	void rioRSkillShort(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer);
	void rioRSkillShortImpact(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer);
	void rioRSkillLong(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer);
	void rioDSkill(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer);

	void ayaTargetBullet(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer);
	void ayaWSkill(LumiaLevel* _level, GameEngineSocketInterface* _network, bool _bServer);

private:
	float4 position_;
	float4 scale_;
	float damage_;
	float speed_;
	float rotationY_;
	float waitTime_;
	float lifeTime_;
	int ownerIndex_;
	int targetIndex_;
	int projectileType_;
	int targetType_;
};

