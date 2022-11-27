#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineUIRenderer;
class TempLobbyRenderer : public GameEngineActor
{
public:
	TempLobbyRenderer(); // default constructer 디폴트 생성자
	~TempLobbyRenderer(); // default destructer 디폴트 소멸자
	TempLobbyRenderer(const TempLobbyRenderer& _other); // default Copy constructer 디폴트 복사생성자
	TempLobbyRenderer(TempLobbyRenderer&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	TempLobbyRenderer& operator=(const TempLobbyRenderer& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TempLobbyRenderer& operator=(const TempLobbyRenderer&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

