#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// �з� : ��Ŷ
// �뵵 : ����
// ���� : ����ü���� ��Ŷ
class LevelChangePacket : public GameEnginePacketBase
{
public:
	void SetChangeLevelName(const std::string _LevelName);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
public:
	LevelChangePacket();
	~LevelChangePacket();

protected:
	LevelChangePacket(const LevelChangePacket& _other) = delete;
	LevelChangePacket(LevelChangePacket&& _other) noexcept = delete;

private:
	LevelChangePacket& operator=(const LevelChangePacket& _other) = delete;
	LevelChangePacket& operator=(const LevelChangePacket&& _other) = delete;

public:
protected:
private:
	std::string LevelName_;
};

