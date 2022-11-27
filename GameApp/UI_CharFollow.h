#pragma once
#include <GameEngine/GameEngineActor.h>
#include "CharacterStat.h"
#include "MonsterStateInfo.h"

using namespace std;

//캐릭터를 쫓아다니는 UI들을 모아둔 클래스입니다
class GameEngineUIRenderer;
class GameEngineProgressBarRenderer;
class UI_CharFollow : public GameEngineActor
{
public:
	UI_CharFollow();
	~UI_CharFollow();

	void SetFollowInfo(float4 _Pos, CharacterStat* _Stat);
	void SetFollowInfoMonster(float4 _Pos, MonsterStateInfo _Stat);
	
	void SetChar(JobType _Char)
	{
		MyChar = _Char;
	}

	void SetPassiveUI(int _num);

public:
	GameEngineProgressBarRenderer* HPBar_Renderer;
	GameEngineProgressBarRenderer* SPBar_Renderer;
	GameEngineProgressBarRenderer* EmptyBar_Renderer;

	float4 HpBarPos;
	float4 HpBarSize;

	float4 SpBarPos;
	float4 SpBarSize;

	float4 EmptyBarPos;
	float4 EmptyBarSize;

	float4 PassiveCountPos;
	float4 PassiveCountSize;

	bool UIOn;

	JobType MyChar;

	GameEngineUIRenderer* PassiveRenderer;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_CharFollow(const UI_CharFollow& _Other) = delete;
	UI_CharFollow(UI_CharFollow&& _Other) noexcept = delete;
	UI_CharFollow& operator=(const UI_CharFollow& _Other) = delete;
	UI_CharFollow& operator=(UI_CharFollow&& _Other) noexcept = delete;
};

