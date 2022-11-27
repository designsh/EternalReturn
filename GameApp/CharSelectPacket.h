#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// 분류 : 
// 용도 : 
// 설명 : 
class CharSelectPacket : public GameEnginePacketBase
{
public:
	CharSelectPacket(); 
	~CharSelectPacket();
	CharSelectPacket(const CharSelectPacket& _other) = delete;
	CharSelectPacket(CharSelectPacket&& _other) noexcept;
	CharSelectPacket& operator=(const CharSelectPacket& _other) = delete;
	CharSelectPacket& operator=(const CharSelectPacket&& _other) = delete;

public:
	void SetTargetIndex(int _index);
	void SetCharacter(int _character);
	void SetStartPoint(int _startPoint);

	int GetTargetIndex()
	{
		return targetIndex_;
	}

	int GetCharacter()
	{
		return character_;
	}

	int GetStartPoint()
	{
		return startPoint_;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	// 무엇이 원소로 있어야 하나?
	// 1. 어떤 캐릭터를 선택했나?
	// 2. 어떤 지점을 시작 지점으로 선택했나?
	int targetIndex_;
	int character_;
	int startPoint_;
	// 정수로 해놨지만 이후 enum 이든 합의된 값으로 바꿀 것.
};

