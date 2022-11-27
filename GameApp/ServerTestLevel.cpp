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

ServerTestLevel::ServerTestLevel() // default constructer 디폴트 생성자
	: bIsServer_(false), chattingInput_(nullptr), chattingHistory_(nullptr)
{

}

ServerTestLevel::~ServerTestLevel() // default destructer 디폴트 소멸자
{

}

void ServerTestLevel::LevelStart()
{
}

void ServerTestLevel::LevelUpdate(float _DeltaTime)
{
	if (bIsServer_ == true && GameEngineInput::GetInst().Down("CloseServer"))
	{
		// (현재 사용자가 호스트일 시) 서버를 닫는 함수입니다.
		server_.CloseServer();
		return;
	}

	if (!server_.IsOpened() && !client_.IsConnected() && GameEngineInput::GetInst().Down("CreateServer"))
	{
		// 현재 사용자가 호스트로서 서버를 만드는 과정들입니다.
		bIsServer_ = true;
		server_.Initialize();
		server_.OpenServer();

		// 패킷 핸들러를 더하는 함수입니다.
		// 특정 유형의 패킷(업데이트, 채팅 등) 을 현 소켓에서 인식하고 받아들이려면 이 함수를 실행해야 합니다.
		server_.AddPacketHandler(ePacketID::Chat, new ChattingPacket);
		return;
	}

	if (!bIsServer_ && GameEngineInput::GetInst().Down("JoinServer"))
	{
		// 사용자가 클라이언트로서 서버에 입장하기 위해 소켓을 만드는 과정입니다.
		client_.Initialize();
		client_.Connect("127.0.0.1"); // 호스트 사용자의 IP를 입력해주어야 합니다.
		client_.AddPacketHandler(ePacketID::Chat, new ChattingPacket);
		return;
	}

	if (!bIsServer_ && client_.IsConnected() && GameEngineInput::GetInst().Down("SendChat"))
	{
		
		ChattingPacket packet;
		packet.SetText("테스트 패킷 전송");
		client_.Send(&packet);
	}

	if (server_.IsOpened())
	{
		// ProcessPacket()
		// 자기에게 들어오는 패킷을 해독합니다.
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

	GameEngineFontManager::GetInst().Load("굴림");
	chattingInput_ = CreateActor<ChattingInput>();
	chattingHistory_ = CreateActor<ChattingHistory>();
}