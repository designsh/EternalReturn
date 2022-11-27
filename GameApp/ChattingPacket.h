#pragma once
#include <GameEngine/GameEnginePacketBase.h>

class ChattingPacket : public GameEnginePacketBase
{
public:
	ChattingPacket();
	~ChattingPacket();
	ChattingPacket(const ChattingPacket& _other) = delete;
	ChattingPacket(ChattingPacket&& _other) = delete;
	ChattingPacket& operator=(const ChattingPacket& _other) = delete;
	ChattingPacket& operator=(const ChattingPacket&& _other) = delete;

public:
	void SetText(const std::string& _text);
	std::string GetText() { return text_; }

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	enum { MAX_TEXT_SIZE = 512 };
	std::string text_;

};

