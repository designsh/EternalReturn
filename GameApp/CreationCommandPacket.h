#pragma once
#include <GameEngine/GameEnginePacketBase.h>

#include "MonsterInfo.h"

// �з� : ��Ŷ
// �뵵 : 
// ���� : Ŭ���̾�Ʈ���� ���ҽ� �ε� �� ���ͻ������� ����� ������ ��Ŷ
class CreationCommandPacket : public GameEnginePacketBase
{
public: // Inline Get Function
public: // Inline Set Function
	inline void SetFirstFlag(bool _IsFirst)
	{
		IsFirst_ = _IsFirst;
	}

	inline void FirstFlagOn()
	{
		IsFirst_ = true;
	}

	inline void FirstFlagOff()
	{
		IsFirst_ = false;
	}

	inline void SetTotMonsterCount(int _TotMonsterCount)
	{
		TotMonsterCount_ = _TotMonsterCount;
	}

	inline void SetMonsterInfo(MonsterInfo _MonsterInfo)
	{
		MonsterInfo_ = _MonsterInfo;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:

public:
	CreationCommandPacket();
	~CreationCommandPacket();

protected:
	CreationCommandPacket(const CreationCommandPacket& _other) = delete;
	CreationCommandPacket(CreationCommandPacket&& _other) noexcept = delete;

private:
	CreationCommandPacket& operator=(const CreationCommandPacket& _other) = delete;
	CreationCommandPacket& operator=(const CreationCommandPacket&& _other) = delete;

public:
protected:
private:
	bool IsFirst_;																			// ���� ����������������
	int TotMonsterCount_;																	// �ѻ����ؾ��ϴ� ���ͼ�
	MonsterInfo MonsterInfo_;																// ��������
};

