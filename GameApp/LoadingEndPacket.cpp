#include "Precompile.h"
#include "LoadingEndPacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include "UserGame.h"

#include "LevelChangePacket.h"

#include "LoadingLevel_LoadPercent.h"

void LoadingEndPacket::userSerialize()
{
	serializer_ << UniqueID_;
	serializer_ << LoadingEndFlag_;
}

void LoadingEndPacket::userDeserialize()
{
	serializer_ >> UniqueID_;
	serializer_ >> LoadingEndFlag_;
}

void LoadingEndPacket::initPacketID()
{
	SetPacketID(ePacketID::LoadEndPacket);
}

GameEnginePacketBase* LoadingEndPacket::getUserObject()
{
	return new LoadingEndPacket;
}

void LoadingEndPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	PlayerInfoManager* InfoManager = PlayerInfoManager::GetInstance();

	// ����(ȣ��Ʈ) ����ó��
	if (true == _bServer)
	{
		// ���������� �ش� ��Ŷ�� ���Ž� �÷��̾����� ������ �����ϰ�,
		InfoManager->GetPlayerList()[UniqueID_].IsLoading_ = LoadingEndFlag_;

		// �ε��ۼ�Ʈ ����ȭ�� ���� �ٸ� Ŭ���̾�Ʈ���� ��Ŷ ���� ��
		LoadingEndPacket EndPacket;
		EndPacket.SetUniqueID(UniqueID_);
		EndPacket.SetLoadingFlag(LoadingEndFlag_);
		_network->Send(&EndPacket);

		// ���� �ε��ۼ�Ʈ ����
		LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();

		// �������� �÷��̾�������Ͽ� �����ϴ� ��� �÷��̾ �ε��Ϸ��� ����ü���� ��Ŷ �۽�
		// ��, ������ �ε��Ϸ���� üũ ����
		// -> ������ �ε��Ϸ�ǰ� ���� Ŭ�� ������ �ε��Ǿ����� �Ʒ� ��Ŷ���� ����ü����
		//    �ش� ������ ���������ʴ´ٸ� ������ �ε��Ϸ������� ����ü������Ŷ�۽�
		if (true == InfoManager->AllPlayerLoadingStateCheck())
		{
			// ��ȯ���� true �̶�� ��� �÷��̾�(ȣ��Ʈ ����) �ε��Ϸ� �Ǿ��ٴ� �ǹ��̹Ƿ� ���� ü���� ��Ŷ �۽�
			LevelChangePacket Packet;
			Packet.SetChangeLevelName("LumiaLevel");
			_network->Send(&Packet);

			// ��Ŷ�۽��� �������Ƿ� ���� ������ ü����
			UserGame::LevelChange("LumiaLevel");
		}
	}
	// Ŭ��(�Խ�Ʈ) ����ó��
	else if (false == _bServer)
	{
		// ���������� �ش� ��Ŷ�� ���Ž� �÷��̾����� ������ �����ϰ�,
		InfoManager->GetPlayerList()[UniqueID_].IsLoading_ = LoadingEndFlag_;

		// ���� �ε��ۼ�Ʈ ����
		LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();
	}
}

LoadingEndPacket::LoadingEndPacket()
	: UniqueID_(-1)
	, LoadingEndFlag_(0)
{
}

LoadingEndPacket::~LoadingEndPacket()
{
}
