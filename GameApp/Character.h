#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngineBase/GameEngineRandom.h>
#include "NavMesh.h"
#include "CharacterStat.h"
#include "IUnit.h"
#include "LevelUPData.h"
#include "UseableItem.h"
#include <GameEngineBase/GameEngineSoundPlayer.h>

struct CombineItem;
struct QueueItem;
struct RecoveryItem
{
public:
	UseableItemType Type;
	float RecoveryValue;
};

// 이터널 리턴에 나오는 실험체의 베이스가 되는 클래스
class StunEffect;
class LevelUpEffect;
class SlowEffect;
class DeathEffect;
class ItemBoxManager;
class MousePointer;
class LumiaMap;
class ItemBase;
class EquipmentItem;
class PlayerUIController;
class UI_DamagePopUp;
class GameEngineFBXRenderer;
class GameEngineShadowRenderer;
class GameEnginePreprocessingRenderer;
class Character : public IUnit
{
	friend class ItemBoxWindow;
	friend class ItemListWindow;
public:
	Character();
	~Character();
	Character(const Character& _other) = delete;
	Character(Character&& _other) noexcept = delete;
	Character& operator=(const Character& _other) = delete;
	Character& operator=(const Character&& _other) = delete;

public:
	static void LoadResource();

private:
	void LevelChangeStartEvent(GameEngineLevel* _NextLevel) override;
public:
	void Start() override;
	void Update(float _DeltaTime) override;

	int GetIndex() override;
	//void Damage(float _amount) override;
	void Damage(float _Amount, IUnit* _Target) override;

public: // 221025 SJH ADD : 레벨업
	void LevelUP(LevelUPData _Data);

public: // 221025 SJH ADD : Functions related to outline activation and deactivation
	void PickingOutLineOn();
	void PickingOutLineOff();

public:
	void InitSpawnPoint(const float4& _position);
	void InitEyeSight();

	void Move(const float4& _position);
	void MoveWithPathFind(const float4& _position);

	void Hyperloop(const float4& _position, Location _location);
	bool IsOperating() { return normalState_.GetCurrentStateName() == "HyperloopBegin"; }

	void Focus() { bFocused_ = true; }
	void UnFocus() { bFocused_ = false; }
	bool IsFocused() { return bFocused_; }

	virtual void Show();
	virtual void Hide();
	bool IsHidden() { return bHidden_; }

	void ChangeAnimation(const std::string& _animationName, bool _bForce = false);
	void ChangeOverrideAnimation(const std::string& _animationName, const std::string& _boneNameToAffect, bool _bForce = false);

	void Stun(float _stunTime) override;
	void Knockback(float _knockbackTime, float4 _knockbackSpeed) override;
	void WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime) override;
	void Slow(float _slowTime, float _slowRatio) override;

	bool IsDead() { return isPlayerDead_; }

#pragma region GetterSetter
public:
	CharacterStat* GetStat() { return &stat_; }
	float4 GetDirection() const { return direction_; }
	std::string GetCurAnimation() const { return curAnimationName_; }
	std::string GetOverrideAnimationName() const { return overrideAnimationName_; }
	std::string GetOverrideAnimationBoneName() const { return overrideAnimationBoneName_; }
	NavFace* GetCurrentNavFace() const { return currentNavFace_; }
	std::vector<ItemBase*> GetInventory() const { return inventory_; }
	PlayerUIController* GetUIController() const { return uiController_; }
	GameEngineRenderTarget* GetFOWRenderTarget() const { return fowRenderTarget_; }
	const std::vector<GameEngineVertex>& GetEyesightVertices() { return eyesightVertices_; }
	const std::vector<UINT>& GetEysightIndices() { return eyesightIndices_; }
	Location GetCurLocation() { return curLocation_; }
	Location GetPrevLocation() { return prevLocation_; }
	std::list<ItemBase*>& GetAllMyBuildItems() { return allMyBuildItems_; };
	UI_DamagePopUp* GetDamagepopup() { return damagePopUp_; }
	GameEngineCollision* GetCollision() { return collision_; };

	void SetUIController();
	void SetDirection(float4 _dir) { direction_ = _dir; }
	void SetCurrentNavFace(NavFace* _Navi) { currentNavFace_ = _Navi; }
	void SetCurrentNavMesh(NavMesh* _NaviMesh);
	void SetStat(CharacterStat _status) { stat_ = _status; }
	void SetIndex(int _index) { myIndex_ = _index; }
	void SetMouse(MousePointer* _mouse) { mouse_ = _mouse; }
	void SetMap(LumiaMap* _map) { currentMap_ = _map; }
	void SetInventoryItem(ItemBase* _item, int _index) { inventory_[_index] = _item; }

	void SetCharacterDeath();
	void SetFraggerIndex(int _index);

	void SetTriggerProhibit(bool _tf) { isInProhibited_ = _tf; }
	void SetCurLocation(Location _location) { curLocation_ = _location; }
	void SetPrevLocation(Location _location) { prevLocation_ = _location; }

	// 스탯 내 기준점이 되는 쿨타임 변수
	//float GetCooltimeQSkill() { return stat_.Cooltime_q; }
	//float GetCooltimeWSkill() { return stat_.Cooltime_w; }
	//float GetCooltimeESkill() { return stat_.Cooltime_e; }
	//float GetCooltimeRSkill() { return stat_.Cooltime_r; }
	//float GetCooltimeDSkill() { return stat_.Cooltime_d; }

	// "현재" 돌고 있는 스킬의 쿨타임 타이머
	float GetCurCooltimeTimerQ() { return coolTimer_Q_; }
	float GetCurCooltimeTimerW() { return coolTimer_W_; }
	float GetCurCooltimeTimerE() { return coolTimer_E_; }
	float GetCurCooltimeTimerR() { return coolTimer_R_; }
	float GetCurCooltimeTimerD() { return coolTimer_D_; }

	//void SetCooltimeQSkill(float _coolTime) { stat_.Cooltime_q = _coolTime; }
	//void SetCooltimeWSkill(float _coolTime) { stat_.Cooltime_w = _coolTime; }
	//void SetCooltimeESkill(float _coolTime) { stat_.Cooltime_e = _coolTime; }
	//void SetCooltimeRSkill(float _coolTime) { stat_.Cooltime_r = _coolTime; }
	//void SetCooltimeDSkill(float _coolTime) { stat_.Cooltime_d = _coolTime; }

#pragma endregion

#pragma region VirtualFunc
public:
	virtual JobType GetJobType() = 0;

protected:
	virtual void initRendererAndAnimation() = 0;

	virtual void changeAnimationRun() = 0;
	virtual void changeAnimationWait() = 0;
	virtual void changeAnimationBasicAttack() = 0;
	virtual void changeAnimationCook() {}
	virtual void changeAnimationCraft() = 0;
	virtual void changeAnimationOperate() = 0;
	virtual void changeAnimationArrive() = 0;
	virtual void changeAnimationRestStart() = 0;
	virtual void changeAnimationRestLoop() = 0;
	virtual void changeAnimationRestEnd() = 0;
	virtual void changeDeathAnimation() = 0;
	virtual void changeAnimationGather() = 0;

	// Main(AttackState)
	virtual void onStartBasicAttacking(IUnit* _target) = 0;
	virtual void onUpdateBasicAttacking(IUnit* _target, float _deltaTime) = 0;

	virtual void onStartQSkill() = 0;
	virtual void onUpdateQSkill(float _deltaTime) = 0;

	virtual void onStartWSkill() = 0;
	virtual void onUpdateWSkill(float _deltaTime) = 0;

	virtual void onStartESkill() = 0;
	virtual void onUpdateESkill(float _deltaTime) = 0;

	virtual void onStartRSkill() = 0;
	virtual void onUpdateRSkill(float _d1eltaTime) = 0;

	virtual void onStartDSkill() = 0;
	virtual void onUpdateDSkill(float _deltaTime) = 0;

	// 캐릭터 마다 고유한 FSM 스테이트를 update 하고 싶을 때 사용
	virtual void onUpdateCustomState(float _deltaTime) = 0;

	// Main(DeathState)
	virtual void onStartDeath() = 0;
	virtual void onUpdateDeath(float _deltaTime) = 0;

	virtual void onPlayEffect(const std::string& _effectName, IUnit* _victim = nullptr, float4 _pos = float4::ZERO) = 0;


	// LVUP
	virtual void onLevelUp() = 0;
#pragma endregion

protected:
	void inputProcess(float _deltaTime);
	void moveProcess(float _deltaTime);
	void moveTickLockDirection(float _deltaTime, const float4& _startPosition);

	void setRotationTo(const float4& _destination, const float4 _startPosition);
	void setRotationToMouse();

	IUnit* getMousePickedCharacter();

	void initBasicEffect();

	void getItem(const std::string& _itemName);
	void SetEquipBuildItem(const std::string& _itemName, EquipmentType _type);
	void checkBuildItems();

private:
	void initInput();
	void initState();

public:
	void checkItemRecipes();	// 인벤토리 내의 아이템 조합 가능여부를 판별
private:
	void moveTick(float _deltaTime, const float4& _startPosition);

	void checkCurrentNavFace();

	void getItem(int _index);
	void getEquipItem(EquipmentItem* _item, int _index);
	void getEquipItem(EquipmentItem* _item);
	void checkItemBox();
	void mixingItem();
	void gatherItem();
	void checkBuildItemsRecursive(ItemBase* _item);

	void checkInventoryInteraction();
	void checkInventoryInteractionLBtn();
	void checkInventoryInteractionRBtn();
	void checkInventoryInteractionKey();

	void useRecoveryItem(UseableItem* _item);	// 아이템을 사용
	void updateRecoveryItem(float _deltaTime);	// 실제로 회복시키는 부분 1초에 한번씩 나눠서 회복
	void mouseGrabItem();
	void mousePutItem();

	void updateFOW(float _deltaTime);
	void getFOWData(std::vector<float4>& _data, bool& _bCalc);

	void soundChangeByMap();
	void playAreaBGMsound(Location _value);

#pragma region InfightCheck
	void InfightCheck(float _DeltaTime);
	void ResetInfight();
#pragma endregion

#pragma region DebuffCheck
	void DebuffCheck(float _DeltaTime);
	void SlowCheck(float _DeltaTIme);
#pragma endregion

#pragma region CoolTimeCheck
	void CoolTimeCheck(float _DeltaTime);
#pragma endregion

#pragma region EffectTransformCheck
	void EffectTransformCheck(float _DeltaTime);
	virtual void onEffectTransformCheck(float _DeltaTime) = 0;
#pragma endregion

#pragma region ProhibitedAreaCheck
	void ProhibitedAreaCheck(float _DeltaTime);
#pragma endregion
	//------------------------------------------------------------------------------------------------------------------
	// State
	//------------------------------------------------------------------------------------------------------------------

	// Main FSM 이 다른 FSM의 Update 를 책임진다.
	//
	//   계층도
	//                    [Main]
	//     ----------------  |  ---------------- ----------------------------
	//     |                 |                 |				|           |
	//  [Normal]            [CC]            [Attack]		 [Death]     [Custom]
	// 
#pragma region MainState
	void startNormalState();
	void updateNormalState(float _deltaTime);
	void endNormalState();

	void startCrowdControlState();
	void updateCrowdControlState(float _deltaTime);

	void startAttackState();
	void updateAttackState(float _deltaTime);

	// 캐릭터의 고유한 FSM 스테이트를 update 해 준다.
	void startCustomState();
	void updateCustomState(float _deltaTime);

	void startDeathState();
	void updateDeathState(float _deltaTime);
#pragma endregion



#pragma region NormalState
	// Watch 는 Stop 과 다르다. 공격 범위에 실험체가 있다면 자동으로 공격한다.
	void startWatch();
	void updateWatch(float _deltaTime);

	// Stop 은 공격당하거나, 공격 범위에 실험체가 있더라도 자동으로 공격하지 않는다.
	void startStop();
	void updateStop(float _deltaTime);

	void startRun();
	void updateRun(float _deltaTime);

	void startChase();
	void updateChase(float _deltaTime);

	void startMixItem();
	void updateMixItem(float _deltaTime);

	void startCraft();
	void updateCraft(float _deltaTime);

	void startCook();
	void updateCook(float _deltaTime);

	void startHyperloopBegin();
	void updateHyperloopBegin(float _deltaTime);

	void startHyperloopEnd();
	void updateHyperloopEnd(float _deltaTime);

	void startRestBegin();
	void updateRestBegin(float _deltaTime);

	void startRestLoop();
	void updateRestLoop(float _deltaTime);

	void startRestEnd();
	void updateRestEnd(float _deltaTime);

	void startGather();
	void updateGather(float _deltaTime);

#pragma endregion



#pragma region CrowdControlState
	void startStun();
	void updateStun(float _deltaTime);

	void startKnockback();
	void updateKnockback(float _deltaTime);

	void startWallSlam();
	void updateWallSlam(float _deltaTime);

	//void startHyunwooE();
	//void updateHyunwooE(float _deltaTime);

#pragma endregion



#pragma region AttackState
	void startBasicAttack();
	void updateBasicAttack(float _deltaTime);

	void startBasicAttacking();
	void updateBasicAttacking(float _deltaTime);

	void startQSkill();
	void updateQSkill(float _deltaTime);

	void startWSkill();
	void updateWSkill(float _deltaTime);

	void startESkill();
	void updateESkill(float _deltaTime);

	void startRSkill();
	void updateRSkill(float _deltaTime);

	void startDSkill();
	void updateDSkill(float _deltaTime);
#pragma endregion

#pragma region DeathState
	void startPlayerAlive();
	void updatePlayerAlive(float _deltaTime);

	void startPlayerDeath();
	void updatePlayerDeath(float _deltaTime);

	void startPlayerWinner();
	void updatePlayerWinner(float _deltaTime);
#pragma endregion


public:
#pragma region EffectPlay
	void PlayEffect(const std::string& _effectName, int _originIndex, IUnit* _victim = nullptr, float4 _pos = float4::ZERO, float _damage = 0);
#pragma endregion

#pragma region RandomSoundPlay
	void RandomSoundPlay(const std::string& _sound0 = "", const std::string& _sound1 = "", const std::string& _sound2 = "");
#pragma endregion

	//------------------------------------------------------------------------------------------------------------------

protected:
	// 컴포넌트
	GameEngineFBXRenderer* renderer_;
	GameEngineCollision* collision_;
	GameEngineRenderer* eyesightRenderer_;

	// 시야 판별용
	GameEngineVertexBuffer* eyesightVertexBuffer_;
	GameEngineIndexBuffer* eyesightIndexBuffer_;
	std::vector<GameEngineVertex> eyesightVertices_;
	std::vector<UINT> eyesightIndices_;
	GameEngineRenderTarget* fowRenderTarget_;
	GameEngineTexture* fowTexture_;
	std::vector<float4> fowData_;
	std::thread* fowDataThread_;
	bool bHidden_;

	// 맵, 이동
	LumiaMap* currentMap_;
	NavFace* currentNavFace_;
	NavMesh* currentNavMesh_;
	float4 destination_;
	float4 direction_;
	float4 hyperloopDest_;
	Location hyperLocation_;
	std::vector<float4> destinations_;
	float4 fowColor_ = float4::WHITE;

	// 아이템
	ItemBoxManager* itemBoxmanager_;
	std::vector<ItemBase*> inventory_;	// 10칸
	std::list<QueueItem> queueItemMixing_;
	int prevInventoryItemPos_;

	std::vector<EquipmentItem*> equipedItem_;
	std::vector<EquipmentItem*>	equipBuildItem_;	// 부위별 최종 장비 아이템
	std::list<ItemBase*>		allMyBuildItems_;	// 아이템을 완성하기 위한 모든 재료

	std::list<RecoveryItem> queueRecoveryItem_;		// HP, SP 회복관련
	float recoveryTimeCheck_;

	// 캐릭터 상태, 능력치
	CharacterStat stat_;		// 장비아이템까지 합산된 최종능력치
	float regenTimer_;

	std::string curAnimationName_;
	std::string overrideAnimationName_;
	std::string overrideAnimationBoneName_;
	
	// 초기화용 쿨타임 척도
	float DefaultCool_Q_;
	float DefaultCool_W_;
	float DefaultCool_E_;
	float DefaultCool_R_;
	float DefaultCool_D_;

	// 쿨타임 계산 버퍼와 플래그
	float coolTimer_Q_;
	float coolTimer_W_;
	float coolTimer_E_;
	float coolTimer_R_;
	float coolTimer_D_;

	bool bCoolQ_;
	bool bCoolW_;
	bool bCoolE_;
	bool bCoolR_;
	bool bCoolD_;

	// 쿨타임 보조 효과용 변수
	bool bCoolWQ_;
	bool bCoolWW_;
	bool bCoolWE_;
	bool bCoolWR_;
	bool bCoolWD_;

	float coolTimerW_Q_;
	float coolTimerW_W_;
	float coolTimerW_E_;
	float coolTimerW_R_;
	float coolTimerW_D_;

	// Omni State
	GameEngineFSM mainState_;

	// In Main State
	GameEngineFSM normalState_;
	GameEngineFSM crowdControlState_;
	GameEngineFSM attackState_;

	// 공격
	IUnit* target_;

	GameEngineFSM deathState_;

	// 비 군중 제어 but 스탯 관여
	bool bSlowFlag_;
	float slowTimer_;
	float slowRatio_;
	float originalMovementSpeed_;

	// 군중 제어
	float timerStun_;
	float timerKnockback_;
	float4 knockbackSpeed_;

	//float timerHyunwooE_;
	//float4 dirHyunwooE_;

	// 그 외
	MousePointer* mouse_;
	bool bFocused_;
	float attackCooldown_;
	float attackTime_;

	// characterList && pm 의 player 인덱스값과 동일한 변수값
	int myIndex_;
	//
	// 플레이어 승리 판정
	bool isPlayerWon_;

	// 플레이어 죽음 판정
	bool isPlayerDead_;
	int fraggerIndex_; // 나를 죽인 사람의 인덱스

	// 1004 이건호 : UI컨트롤러 멤버변수화
	PlayerUIController* uiController_;
	UI_DamagePopUp* damagePopUp_;
	bool isMouseOntheUI_;

	// 전투 중 유무의 표시용 변수
	bool isInfight_;
	float infightTimer_;

	// 금지구역관련
	bool isInProhibited_;
	float prohibitedCounter_;
	float prohibitTimer_;
	Location curLocation_;
	Location prevLocation_;


private:
	enum class eCurrentAnimation
	{
		None,
		Wait,
		Run,
		BasicAttack
	};
	eCurrentAnimation currentAnimation_;

// 기본 이팩트
protected:
	SlowEffect* slowEffect_;
	StunEffect* stunEffect_;
	LevelUpEffect* levelUpEffect_;
	DeathEffect* deathEffect_;

// 실루엣 처리
protected:
	GameEnginePreprocessingRenderer* MainSilhouetteRenderer_;
	GameEnginePreprocessingRenderer* WeaponSilhouetteRenderer1_;
	GameEnginePreprocessingRenderer* WeaponSilhouetteRenderer2_;

// 외곽선처리
protected:
	GameEnginePreprocessingRenderer* MainOutLineRenderer_;
	GameEnginePreprocessingRenderer* WeaponOutLineRenderer1_;
	GameEnginePreprocessingRenderer* WeaponOutLineRenderer2_;

protected:
	GameEngineRandom randomGenerator;

private:
	bool isDebugInvincible_;

	GameEngineSoundPlayer areaSoundPlayer_;
};