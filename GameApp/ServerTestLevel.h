#pragma once
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>

#include <GameEngine/GameEngineSocketServer.h>
#include <GameEngine/GameEngineSocketClient.h>

class ChattingInput;
class ChattingHistory;
class ServerTestLevel : public GameEngineLevel
{
public:
	ServerTestLevel(); // default constructer ����Ʈ ������
	~ServerTestLevel(); // default destructer ����Ʈ �Ҹ���
	ServerTestLevel(const ServerTestLevel& _other) = delete; // default Copy constructer ����Ʈ ���������
	ServerTestLevel(ServerTestLevel&& _other) noexcept = delete; // default RValue Copy constructer ����Ʈ RValue ���������
	ServerTestLevel& operator=(const ServerTestLevel& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ServerTestLevel& operator=(const ServerTestLevel&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;
protected:


private:
	// *** ���� ���ϰ� Ŭ���̾�Ʈ ���� �� �� �ϳ��� �����ؼ� ����ؾ� �մϴ�.
	GameEngineSocketServer server_;
	GameEngineSocketClient client_;
	bool bIsServer_;

	ChattingInput* chattingInput_;
	ChattingHistory* chattingHistory_;
};

