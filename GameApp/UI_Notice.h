#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// Ό³Έν :
class GameEngineUIRenderer;
class Player;
class UI_Notice : public GameEngineActor
{
public:

	// constrcuter destructer
	UI_Notice();
	~UI_Notice();

	// delete Function
	UI_Notice(const UI_Notice& _Other) = delete;
	UI_Notice(UI_Notice&& _Other) noexcept = delete;
	UI_Notice& operator=(const UI_Notice& _Other) = delete;
	UI_Notice& operator=(UI_Notice&& _Other) noexcept = delete;

public:
	void UISwitch();
	void SetText(string _Text, float _Time);

private:
	void Start() override;
	void Update(float _Time) override;

	void TimeCount(float _Time);

private:
	float4 UI_Pos;
	float4 Font_Pos;

	GameEngineUIRenderer* BackGroundRenderer;
	GameEngineUIRenderer* FontRenderer;

	float FadeWaitTime;
	float FadeTime;
	float BasicAlpha;

	bool UIOn;

	bool Changing_Flag;

};

