#include "PreCompile.h"
#include "ChattingPacket.h"
#include <iostream>
#include "ePacketID.h"

ChattingPacket::ChattingPacket()
{

}

ChattingPacket::~ChattingPacket()
{

}

void ChattingPacket::userSerialize()
{
	serializer_ << text_;
}

void ChattingPacket::userDeserialize()
{
	serializer_ >> text_;
}

void ChattingPacket::initPacketID()
{
	SetPacketID(ePacketID::Chat);
}

GameEnginePacketBase* ChattingPacket::getUserObject()
{
	return new ChattingPacket;
}

void ChattingPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	std::cout << text_ << std::endl;
	GameEngineDebug::OutPutDebugString(text_);

	if (_bServer)
	{
		_network->Send(this);
	}
}

void ChattingPacket::SetText(const std::string& _text)
{
	if (_text.size() > MAX_TEXT_SIZE)
	{
		return;
	}

	text_ = _text;
}