#pragma once
#include "FTConstant.h"

class CharacterStat
{
public:
	int level;
	// 기본 스탯
	float HPMax;
	float HP;
	float SPMax;
	float SP;
	float Defence;

	// 기본 공격
	float AttackPower;
	float CriticalChance;

	// 공격 증폭
	float BasicAttackAmplification; // 기본 공격 강화
	float SkillDamageAmplification; // 스킬 대미지 강화
	float CriticalDamageAmplification; // 크리티컬 대미지 강화

	// 저항
	float BasicAttackDamageReduction; // 기본 공격 대미지 감소
	float CriticalDamageReduction; // 크리티컬 대미지 감소
	float SkillAttackDamageReduction; // 스킬 대미지 감소
	float TrapDamageReduction; // 함정 대미지 감소
	float Tenacity; // 강인함

	// 재생 및 흡혈
	float HPRegeneration; // HP 재생
	float SPRegeneration; // SP 재생
	float LifeSteel; // 기본공격 흡혈
	float OmniVamp; // 모든 피해 흡혈

	// 속도
	float AttackSpeedRatio = 1.0; // 공격속도배율
	float AttackSpeed; // 초당 공격 속도
	float AttackStartTime; // 실질적으로 공격이 나가는 시간
	float AttackEndTime; // 공격이 끝나는 시간
	float MovementSpeed; // 이동 속도
	float MovementRatio; // (이동 속도 비율...1.0f 이 정상)
	float CooldownReduction; // 쿨타임 감소

	// 거리
	float AttackRange; // 기본 공격 거리
	float VisionRange; // 시야

	// 스킬 레벨
	int Level_q;
	int Level_w;
	int Level_e;
	int Level_r;
	int Level_d;
	int Level_passive;

	// 1103 박종원 : CC기 무시 bool 값
	bool isUnstoppable_;
	
	//1109 이건호 : 유키나 아야같은 소모하는 패시브용 int
	int passive_Count;

	float SP_Q;
	float SP_W;
	float SP_E;
	float SP_R;

public:
	// 기본 세팅은 리오 기준으로 되어있음
	CharacterStat()
		: level(1)
		, HPMax(782.f)
		, HP(782.f)
		, SPMax(420.f)
		, SP(420.f)
		, Defence(32.f)
		, AttackPower(29.f)
		, CriticalChance(0.0f)
		, BasicAttackAmplification(0.0f)
		, SkillDamageAmplification(0.0f)
		, CriticalDamageAmplification(FT::Char::DEFAULT_CRITICAL_DAMAGE_AMPLIFICATION)
		, BasicAttackDamageReduction(0.0f)
		, SkillAttackDamageReduction(0.0f)
		, CriticalDamageReduction(0.0f)
		, TrapDamageReduction(0.0f)
		, Tenacity(0.0f)
		, HPRegeneration(1.3f)
		, SPRegeneration(1.6f)
		, LifeSteel(0.0f)
		, OmniVamp(0.0f)
		, AttackSpeed(0.77f)
		, AttackStartTime(0.15f)
		, AttackEndTime(0.4f)
		, MovementSpeed(346.f)
		, CooldownReduction(0.0f)
		, AttackRange(425.f)
		, VisionRange(FT::Char::DEFAULT_VISION_RANGE_DAY)
		, MovementRatio(1.0f)
		, Level_q(1)
		, Level_w(1)
		, Level_e(1)
		, Level_r(1)
		, Level_d(1)
		, Level_passive(1)
		, isUnstoppable_(false)
		, passive_Count(0)
		, SP_Q(0.0f)
		, SP_W(0.0f)
		, SP_E(0.0f)
		, SP_R(0.0f)
	{

	}

	~CharacterStat()
	{

	}

	CharacterStat& operator+=(const CharacterStat& _cs)
	{
		level += _cs.level;
		HPMax += _cs.HPMax;
		HP += _cs.HP;
		SPMax += _cs.SPMax;
		SP += _cs.SP;
		Defence += _cs.Defence;
		AttackPower += _cs.AttackPower;
		CriticalChance += _cs.CriticalChance;
		BasicAttackAmplification += _cs.BasicAttackAmplification;
		SkillDamageAmplification += _cs.SkillDamageAmplification;
		CriticalDamageAmplification += _cs.CriticalDamageAmplification;
		BasicAttackDamageReduction += _cs.BasicAttackDamageReduction;
		SkillAttackDamageReduction += _cs.SkillAttackDamageReduction;
		CriticalDamageReduction += _cs.CriticalDamageReduction;
		TrapDamageReduction += _cs.TrapDamageReduction;
		Tenacity += _cs.Tenacity;
		HPRegeneration += _cs.HPRegeneration;
		SPRegeneration += _cs.SPRegeneration;
		LifeSteel += _cs.LifeSteel;
		OmniVamp += _cs.OmniVamp;
		AttackSpeed += _cs.AttackSpeed;
		AttackStartTime += _cs.AttackStartTime;
		AttackEndTime += _cs.AttackEndTime;
		MovementSpeed += _cs.MovementSpeed;
		CooldownReduction += _cs.CooldownReduction;
		AttackRange += _cs.AttackRange;
		VisionRange += _cs.VisionRange;
		MovementRatio += _cs.MovementRatio;
		Level_q += _cs.Level_q;
		Level_w += _cs.Level_w;
		Level_e += _cs.Level_e;
		Level_r += _cs.Level_r;
		Level_d += _cs.Level_d;
		Level_passive += _cs.Level_passive;
		passive_Count += _cs.passive_Count;

		return *this;
	}

	CharacterStat& operator-=(const CharacterStat& _cs)
	{
		level -= _cs.level;

		HPMax -= _cs.HPMax;
		HP -= _cs.HP;

		if (HP > HPMax)
		{
			HP = HPMax;
		}

		SPMax -= _cs.SPMax;
		SP -= _cs.SP;

		if (SP > SPMax)
		{
			SP = SPMax;
		}

		Defence -= _cs.Defence;
		AttackPower -= _cs.AttackPower;
		CriticalChance -= _cs.CriticalChance;
		BasicAttackAmplification -= _cs.BasicAttackAmplification;
		SkillDamageAmplification -= _cs.SkillDamageAmplification;
		CriticalDamageAmplification -= _cs.CriticalDamageAmplification;
		BasicAttackDamageReduction -= _cs.BasicAttackDamageReduction;
		SkillAttackDamageReduction -= _cs.SkillAttackDamageReduction;
		CriticalDamageReduction -= _cs.CriticalDamageReduction;
		TrapDamageReduction -= _cs.TrapDamageReduction;
		Tenacity -= _cs.Tenacity;
		HPRegeneration -= _cs.HPRegeneration;
		SPRegeneration -= _cs.SPRegeneration;
		LifeSteel -= _cs.LifeSteel;
		OmniVamp -= _cs.OmniVamp;
		AttackSpeed -= _cs.AttackSpeed;
		AttackStartTime -= _cs.AttackStartTime;
		AttackEndTime -= _cs.AttackEndTime;
		MovementSpeed -= _cs.MovementSpeed;
		CooldownReduction -= _cs.CooldownReduction;
		AttackRange -= _cs.AttackRange;
		VisionRange -= _cs.VisionRange;
		MovementRatio -= _cs.MovementRatio;
		Level_q -= _cs.Level_q;
		Level_w -= _cs.Level_w;
		Level_e -= _cs.Level_e;
		Level_r -= _cs.Level_r;
		Level_d -= _cs.Level_d;
		Level_passive -= _cs.Level_passive;
		passive_Count -= _cs.passive_Count;

		return *this;
	}
	void Clear()
	{
		level = 0;
		HPMax = 0;
		HP = 0;
		SPMax = 0;
		SP = 0;
		Defence = 0;
		AttackPower = 0;
		CriticalChance = 0;
		BasicAttackAmplification = 0;
		SkillDamageAmplification = 0;
		CriticalDamageAmplification = 0;
		BasicAttackDamageReduction = 0;
		SkillAttackDamageReduction = 0;
		CriticalDamageReduction = 0;
		TrapDamageReduction = 0;
		Tenacity = 0;
		HPRegeneration = 0;
		SPRegeneration = 0;
		LifeSteel = 0;
		OmniVamp = 0;
		AttackSpeed = 0;
		AttackStartTime = 0;
		AttackEndTime = 0;
		MovementSpeed = 0;
		CooldownReduction = 0;
		AttackRange = 0;
		VisionRange = 0;
		MovementRatio = 0;
		Level_q = 0;
		Level_w = 0;
		Level_e = 0;
		Level_r = 0;
		Level_d = 0;
		Level_passive = 0;
		isUnstoppable_ = false;
		passive_Count = 0;
	}
};