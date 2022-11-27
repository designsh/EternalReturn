#include "PreCompile.h"
#include "ServerTestLevel.h"
#include "ePacketID.h"


#include "ChattingInput.h"
#include "ChattingHistory.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineSocketServer.h>
#include <GameEngine/GameEngineSocketClient.h>
#include "KeyboardClass.h"
#include "ChattingPacket.h"

ServerTestLevel::ServerTestLevel() // default constructer ����Ʈ ������
	: bIsServer_(false), chattingInput_(nullptr), chattingHistory_(nullptr)
{

}

ServerTestLevel::~ServerTestLevel() // default destructer ����Ʈ �Ҹ���
{

}

void ServerTestLevel::LevelStart()
{
}

void ServerTestLevel::LevelUpdate(float _DeltaTime)
{
	if (bIsServer_ == true && GameEngineInput::GetInst().Down("CloseServer"))
	{
		// (���� ����ڰ� ȣ��Ʈ�� ��) ������ �ݴ� �Լ��Դϴ�.
		server_.CloseServer();
		return;
	}

	if (!server_.IsOpened() && !client_.IsConnected() && GameEngineInput::GetInst().Down("CreateServer"))
	{
		// ���� ����ڰ� ȣ��Ʈ�μ� ������ ����� �������Դϴ�.
		bIsServer_ = true;
		server_.Initialize();
		server_.OpenServer();

		// ��Ŷ �ڵ鷯�� ���ϴ� �Լ��Դϴ�.
		// Ư�� ������ ��Ŷ(������Ʈ, ä�� ��) �� �� ���Ͽ��� �ν��ϰ� �޾Ƶ��̷��� �� �Լ��� �����ؾ� �մϴ�.
		server_.AddPacketHandler(ePacketID::Chat, new ChattingPacket);
		return;
	}

	if (!bIsServer_ && GameEngineInput::GetInst().Down("JoinServer"))
	{
		// ����ڰ� Ŭ���̾�Ʈ�μ� ������ �����ϱ� ���� ������ ����� �����Դϴ�.
		client_.Initialize();
		client_.Connect("127.0.0.1"); // ȣ��Ʈ ������� IP�� �Է����־�� �մϴ�.
		client_.AddPacketHandler(ePacketID::Chat, new ChattingPacket);
		return;
	}

	if (!bIsServer_ && client_.IsConnected() && GameEngineInput::GetInst().Down("SendChat"))
	{
		
		ChattingPacket packet;
		packet.SetText("�׽�Ʈ ��Ŷ ����");
		client_.Send(&packet);
	}

	if (server_.IsOpened())
	{
		// ProcessPacket()
		// �ڱ⿡�� ������ ��Ŷ�� �ص��մϴ�.
		server_.ProcessPacket();
	}
	else if (client_.IsConnected())
	{
		client_.ProcessPacket();
	}

}

void ServerTestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{


}

void ServerTestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	if (false == GameEngineInput::GetInst().IsKey("CreateServer"))
	{
		GameEngineInput::GetInst().CreateKey("CreateServer", '1');
		GameEngineInput::GetInst().CreateKey("JoinServer", '2');
		GameEngineInput::GetInst().CreateKey("SendChat", '3');
		GameEngineInput::GetInst().CreateKey("CloseServer", '4');
	}

	GameEngineFontManager::GetInst().Load("����");
	chattingInput_ = CreateActor<ChattingInput>();
	chattingHistory_ = CreateActor<ChattingHistory>();
}