#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// Ό³Έν :
class GameEngineUIRenderer;
class Player;
class UI_Time : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Time();
	~UI_Time();

	// delete Function
	UI_Time(const UI_Time& _Other) = delete;
	UI_Time(UI_Time&& _Other) noexcept = delete;
	UI_Time& operator=(const UI_Time& _Other) = delete;
	UI_Time& operator=(UI_Time&& _Other) noexcept = delete;

	bool UIOn;

	GameEngineUIRenderer* GetProhibitedRenderer() { return ProhibitedTimeRenderer_; }

protected:
	GameEngineUIRenderer* BackGroundRenderer;
	GameEngineUIRenderer* DayNightRenderer;
	GameEngineUIRenderer* TimeRenderer;

	GameEngineUIRenderer* NightChangeClockRenderer;
	GameEngineUIRenderer* NightChangeTimeRenderer;

	GameEngineUIRenderer* DayCountRenderer;
	float Time;


	GameEngineUIRenderer* ProhibitedTimeRenderer_;

private:
	void Start() override;
	void Update(float _Time) override;

	void TimeSetting();

	float4 UI_Pos;
	float4 Day_Pos;
	float4 Time_Pos;

	float4 Clock_Pos;
	float4 ClockTime_Pos;

	float4 DayCount_Pos;
	float4 Prohibit_Pos;
};

