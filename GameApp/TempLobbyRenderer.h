#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineUIRenderer;
class TempLobbyRenderer : public GameEngineActor
{
public:
	TempLobbyRenderer(); // default constructer ����Ʈ ������
	~TempLobbyRenderer(); // default destructer ����Ʈ �Ҹ���
	TempLobbyRenderer(const TempLobbyRenderer& _other); // default Copy constructer ����Ʈ ���������
	TempLobbyRenderer(TempLobbyRenderer&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	TempLobbyRenderer& operator=(const TempLobbyRenderer& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TempLobbyRenderer& operator=(const TempLobbyRenderer&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetPos(const float4& _pos);
	void SetScale(const float4& _pos);
	void SetImage(std::string _imageName);
	void SetRender(bool _bool);
	void SetText(const std::string& _text);

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineUIRenderer* portraitRenderer_;

};

