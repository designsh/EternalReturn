#pragma once
#include <GameEngine/GameEngineActor.h>
#include "CharacterStat.h"

using namespace std;
// 설명 :
class GameEngineUIRenderer;
class Player;
class UI_Skill : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Skill();
	~UI_Skill();

	// delete Function
	UI_Skill(const UI_Skill& _Other) = delete;
	UI_Skill(UI_Skill&& _Other) noexcept = delete;
	UI_Skill& operator=(const UI_Skill& _Other) = delete;
	UI_Skill& operator=(UI_Skill&& _Other) noexcept = delete;

	bool UIOn;

public:
	void SetJobType(JobType _myjob);

	void SetStatus(CharacterStat* _Mystat);

	GameEngineUIRenderer* GetCoolTimeMaskQ() { return Icon_Q_CoolCounter; }
	GameEngineUIRenderer* GetCoolTimeMaskW() { return Icon_W_CoolCounter; }
	GameEngineUIRenderer* GetCoolTimeMaskE() { return Icon_E_CoolCounter; }
	GameEngineUIRenderer* GetCoolTimeMaskR() { return Icon_R_CoolCounter; }
	GameEngineUIRenderer* GetCoolTimeMaskD() { return Icon_Weapon_CoolCounter; }
protected:
	GameEngineUIRenderer* BackGroundRenderer;
	//리소스가 많아지면 맵을 사용할 예정
	//map<string, GameEngineUIRenderer*> UIRendererMap;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;


#pragma region 아이콘 렌더러

	GameEngineUIRenderer* Icon_Q;
	GameEngineUIRenderer* Icon_W;
	GameEngineUIRenderer* Icon_E;
	GameEngineUIRenderer* Icon_R;
	GameEngineUIRenderer* Icon_Passive;
	GameEngineUIRenderer* Icon_Weapon;

	GameEngineUIRenderer* Icon_Q_CoolCounter;
	GameEngineUIRenderer* Icon_W_CoolCounter;
	GameEngineUIRenderer* Icon_E_CoolCounter;
	GameEngineUIRenderer* Icon_R_CoolCounter;
	GameEngineUIRenderer* Icon_Weapon_CoolCounter;

	GameEngineUIRenderer* Icon_Q_LevelCounter;
	GameEngineUIRenderer* Icon_W_LevelCounter;
	GameEngineUIRenderer* Icon_E_LevelCounter;
	GameEngineUIRenderer* Icon_R_LevelCounter;
	GameEngineUIRenderer* Icon_Passive_LevelCounter;
	GameEngineUIRenderer* Icon_Weapon_LevelCounter;

#pragma endregion

#pragma region float4 모음

	float4 Skill_BackGroundPos;
	float4 Icon_Q_Pos;
	float4 Icon_W_Pos;
	float4 Icon_E_Pos;
	float4 Icon_R_Pos;
	float4 Icon_Passive_Pos;
	float4 Icon_Weapon_Pos;

	float4 Icon_Q_LevelCounterPos;
	float4 Icon_W_LevelCounterPos;
	float4 Icon_E_LevelCounterPos;
	float4 Icon_R_LevelCounterPos;
	float4 Icon_Passive_LevelCounterPos;
	float4 Icon_Weapon_LevelCounterPos;

#pragma endregion
};

