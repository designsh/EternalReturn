#pragma once
#include <GameEngine/GameEnginePacketBase.h>

class CharAnimationPacket : public GameEnginePacketBase
{
public:
	CharAnimationPacket(); // default constructer 디폴트 생성자
	~CharAnimationPacket(); // default destructer 디폴트 소멸자
	CharAnimationPacket(const CharAnimationPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	CharAnimationPacket(CharAnimationPacket&& _other) noexcept = delete; // default RValue Copy constructer 디폴트 RValue 복사생성자
	CharAnimationPacket& operator=(const CharAnimationPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	CharAnimationPacket& operator=(const CharAnimationPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

