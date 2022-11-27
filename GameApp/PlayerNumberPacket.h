#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// �з� : 
// �뵵 : 
// ���� : 
class PlayerNumberPacket : public GameEnginePacketBase
{
public:
	PlayerNumberPacket(); // default constructer ����Ʈ ������
	~PlayerNumberPacket(); // default destructer ����Ʈ �Ҹ���
	PlayerNumberPacket(const PlayerNumberPacket& _other) = delete; // default Copy constructer ����Ʈ ���������
	PlayerNumberPacket(PlayerNumberPacket&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	PlayerNumberPacket& operator=(const PlayerNumberPacket& _other) = delete; // default Copy operator ����Ʈ ���� ������
	PlayerNumberPacket& operator=(const PlayerNumberPacket&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetPlayerNumber(int _playerNumber);
	//void SetOtherPlayers(std::vector<PlayerInfo> _others);
	int GetPlayerNumber()
	{
		return playerNumber_;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	int playerNumber_;
	int othersSize_;
	std::vector<int> otherCharacter_;
	std::vector<int> otherStartPoint_;
	std::vector<int> otherIsReady_;

};

