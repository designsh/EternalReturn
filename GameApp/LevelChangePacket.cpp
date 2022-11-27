#include "Precompile.h"
#include "LevelChangePacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "UserGame.h"

void LevelChangePacket::SetChangeLevelName(const std::string _LevelName)
{
	LevelName_ = _LevelName;
}

void LevelChangePacket::userSerialize()
{
	serializer_ << LevelName_;
}

void LevelChangePacket::userDeserialize()
{
	serializer_ >> LevelName_;
}

void LevelChangePacket::initPacketID()
{
	SetPacketID(ePacketID::LvChangePacket);
}

GameEnginePacketBase* LevelChangePacket::getUserObject()
{
	return new LevelChangePacket;
}

void LevelChangePacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	// ����(ȣ��Ʈ)������ ���� ���ӿ� ������ ��� �÷��̾��� �ε����¸� üũ�ϹǷ�
	// Ŭ���̾�Ʈ(�Խ�Ʈ)������ �ش� ��Ŷ����ó��
	if (false == _bServer)
	{
		std::string CurLevelName = UserGame::CurrentLevel()->GetName();
		if (CurLevelName != LevelName_)
		{
			UserGame::LevelChange(LevelName_);
		}
	}
}

LevelChangePacket::LevelChangePacket()
	: LevelName_{}
{
}

LevelChangePacket::~LevelChangePacket()
{
}
