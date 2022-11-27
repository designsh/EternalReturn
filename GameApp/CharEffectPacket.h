#pragma once
#include <GameEngine/GameEnginePacketBase.h>

#include "IUnit.h"

class CharEffectPacket : public GameEnginePacketBase
{
public:
	CharEffectPacket(); // default constructer 디폴트 생성자
	~CharEffectPacket(); // default destructer 디폴트 소멸자
	CharEffectPacket(const CharEffectPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	CharEffectPacket(CharEffectPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	CharEffectPacket& operator=(const CharEffectPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	CharEffectPacket& operator=(const CharEffectPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

	void SetTargetIndex(int _index);
	void SetVictimIndex(IUnit& _unit);
	void SetAnimationName(const std::string& _animation);
	void SetWorldPos(float4 _worldPos);
	void SetPopupDamage(float _damage);


protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int targetIndex_;
	int victimIndex_;
	std::string effectAnimationName_;
	int victimType_;
	int popupDamageCount_;

	float4 worldPos_;
};

