#pragma once
#include <GameEngine/GameEnginePacketBase.h>
#include "MonsterStateInfo.h"

// �з� : ��Ŷ
// �뵵 : 
// ���� : GameTimeController�� ���Ͽ� ����(ȣ��Ʈ)�� Ŭ���̾�Ʈ(�Խ�Ʈ)���� �ϰ�ó�� �������� ������ȯ�� ����ϴ� ��Ŷ
class MonsterStateChangePacket : public GameEnginePacketBase
{
public: // Inline Get Function
public: // Inline Set Function
	inline void SetIndex(int _Index)
	{
		Index_ = _Index;
	}

	inline void SetMonsterType(MonsterType _Type)
	{
		Type_ = _Type;
	}

	inline void SetMonsterStateType(MonsterStateType _StateType)
	{
		StateType_ = _StateType;
	}

	inline void SetMonsterStatInfo(MonsterStateInfo _StateInfo)
	{
		StatInfo_ = _StateInfo;
	}

	inline void SetTargetIndex(int _TargetIndex)
	{
		TargetIndex_ = _TargetIndex;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
public:
	MonsterStateChangePacket();
	~MonsterStateChangePacket();

protected:
	MonsterStateChangePacket(const MonsterStateChangePacket& _other) = delete;
	MonsterStateChangePacket(MonsterStateChangePacket&& _other) noexcept = delete;

private:
	MonsterStateChangePacket& operator=(const MonsterStateChangePacket& _other) = delete;
	MonsterStateChangePacket& operator=(const MonsterStateChangePacket&& _other) = delete;

public:
protected:
private:
	int Index_;														// ���¸� ��ȯ�Ϸ��� ������ �����ε���(��, �ϰ�ó���� -1 ����)
	MonsterType Type_;												// ���¸� ��ȯ�Ϸ��� ����Ÿ��
	MonsterStateType StateType_;									// ��ȯ�Ϸ��� ����
	MonsterStateInfo StatInfo_;										// ���� �������� ���ŵ�����

private:
	int TargetIndex_;												// Ÿ������ ������ ĳ������ �ε���
};

