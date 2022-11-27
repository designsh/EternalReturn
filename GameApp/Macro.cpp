#include "Precompile.h"
#include "PacketSoundPlay.h"
#include <GameEngineBase/GameEngineSoundPlayer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include "CharEffectPacket.h"
#include "CharStatPacket.h"

std::unique_ptr<GameEngineSoundPlayer> FT::BGMPlayer = std::make_unique<GameEngineSoundPlayer>();

void FT::SendPacket(GameEnginePacketBase& packet)
{
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->Send(&packet);
	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->Send(&packet);
	}
}

void FT::SendStatPacket(CharacterStat& _characterStat, int _characterIndex)
{
	CharStatPacket packet;
	packet.SetStat(_characterStat);
	packet.SetTargetIndex(_characterIndex);
	FT::SendPacket(packet);
}

void FT::PlaySoundAndSendPacket(const std::string& _name, const float4& _position)
{
	if (GameServer::GetInstance()->IsOpened())
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName(_name);
		PacketSoundPlay packet;
		packet.SetSound(_name, _position);
		FT::SendPacket(packet);
	}
	else if (GameClient::GetInstance()->IsConnected())
	{
		PacketSoundPlay packet;
		packet.SetSound(_name, _position);
		FT::SendPacket(packet);
	}
}

void FT::PlaySoundAllAndSendPacket(const std::string& _name)
{
	if (GameServer::GetInstance()->IsOpened())
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName(_name);
		PacketSoundPlay packet;
		packet.SetSound(_name, float4::ZERO, true);
		FT::SendPacket(packet);
	}
	else if (GameClient::GetInstance()->IsConnected())
	{
		PacketSoundPlay packet;
		packet.SetSound(_name, float4::ZERO, true);
		FT::SendPacket(packet);
	}
}




void FT::AddText(const std::string& _text)
{
	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText(_text);
	}
}