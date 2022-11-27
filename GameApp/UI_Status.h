#pragma once
#include <GameEngine/GameEngineActor.h>
#include "CharacterStat.h"

using namespace std;
// ���� :
class GameEngineUIRenderer;
class Player;
class UI_Status : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Status();
	~UI_Status();

	// delete Function
	UI_Status(const UI_Status& _Other) = delete;
	UI_Status(UI_Status&& _Other) noexcept = delete;
	UI_Status& operator=(const UI_Status& _Other) = delete;
	UI_Status& operator=(UI_Status&& _Other) noexcept = delete;

	bool UIOn;

public:
	void SetStatus(CharacterStat* _Mystat);
	
protected:
	GameEngineUIRenderer* MainStatusRenderer;
	GameEngineUIRenderer* SubStatusRenderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//�ۼ�Ʈ ��ȣ�� ������ �ڿ� �Ҽ����� ���ִ� �Լ�
	void AttachPercent(float _Value, string& _Result);

private:
	CharacterStat* UIStat;

#pragma region �������ͽ� �� ��¿� ������+��ġ

	float4 Value_XPivot;
	float4 Value_YPivot;

	//���� �������ͽ�
	GameEngineUIRenderer* AttackValue_Renderer;
	GameEngineUIRenderer* DefenseValue_Renderer;
	GameEngineUIRenderer* AttackSpeedValue_Renderer;
	GameEngineUIRenderer* MoveSpeedValue_Renderer;
	GameEngineUIRenderer* CriticalValue_Renderer;
	GameEngineUIRenderer* CooldownValue_Renderer;
	GameEngineUIRenderer* SkillDamageIncreaseValue_Renderer;
	GameEngineUIRenderer* SkillDamageReductionValue_Renderer;


	float4 BasicValue_Pos;
	float4 DefenseValue_Pos;
	float4 AttackSpeedValue_Pos;
	float4 MoveSpeedValue_Pos;
	float4 CriticalValue_Pos;
	float4 CooldownValue_Pos;
	float4 SkillDamageIncreaseValue_Pos;
	float4 SkillDamageReductionValue_Pos;

	//���� �������ͽ�
	GameEngineUIRenderer* VisionRange_Renderer;
	GameEngineUIRenderer* AttackRange_Renderer;
	GameEngineUIRenderer* BasicAttackDamageReduction_Renderer;
	GameEngineUIRenderer* Tenacity_Renderer;
	GameEngineUIRenderer* DefPenetration_Renderer;
	GameEngineUIRenderer* VampValue_Renderer;
	GameEngineUIRenderer* HPRegeneration_Renderer;
	GameEngineUIRenderer* SPRegeneration_Renderer;


	float4 SubStatBasicValue_Pos;
	float4 AttackRange_Pos;
	float4 BasicAttackDamageReduction_Pos;
	float4 Tenacity_Pos;
	float4 DefPenetration_Pos;
	float4 VampValue_Pos;
	float4 HPRegeneration_Pos;
	float4 SPRegeneration_Pos;

#pragma endregion 


};

