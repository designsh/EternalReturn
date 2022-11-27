#pragma once
#include <GameEngine/GameEnginePacketBase.h>

#include "IUnit.h"

class CharEffectPacket : public GameEnginePacketBase
{
public:
	CharEffectPacket(); // default constructer ����Ʈ ������
	~CharEffectPacket(); // default destructer ����Ʈ �Ҹ���
	CharEffectPacket(const CharEffectPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharEffectPacket(CharEffectPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	CharEffectPacket& operator=(const CharEffectPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharEffectPacket& operator=(const CharEffectPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

