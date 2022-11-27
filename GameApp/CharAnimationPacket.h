#pragma once
#include <GameEngine/GameEnginePacketBase.h>

class CharAnimationPacket : public GameEnginePacketBase
{
public:
	CharAnimationPacket(); // default constructer ����Ʈ ������
	~CharAnimationPacket(); // default destructer ����Ʈ �Ҹ���
	CharAnimationPacket(const CharAnimationPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharAnimationPacket(CharAnimationPacket&& _other) noexcept = delete; // default RValue Copy constructer ����Ʈ RValue ���������
	CharAnimationPacket& operator=(const CharAnimationPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharAnimationPacket& operator=(const CharAnimationPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetTargetIndex(int _index);
	void SetAnimationName(const std::string& _animation);
	void SetOverrideAnimation(const std::string& _overrideAnimationName, const std::string& _boneNameToAffect);

	std::string GetAnimationName()
	{
		return curAnimation_;
	}


protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int targetIndex_;
	std::string curAnimation_;
	std::string overrideAnimationName_;
	std::string overrideAnimationBoneName_;

};

