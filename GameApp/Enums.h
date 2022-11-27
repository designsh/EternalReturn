#pragma once

// ObjectRenderOrder
enum class ObjectRenderOrder
{
	NONE = -1,
	WEAPON,
	CHARACTER,

	MAX
};

// Unit Type
enum class UnitType
{
	NONE = -1,
	CHARACTER,
	MONSTER,
	MAX
};

// Day And Night Type
enum class DayAndNightType
{
	NONE = -1,
	DAY,
	NIGHT,
	MAX
};

// UI RenderOrder
enum class UIRenderOrder
{
	UNSEEN,						// 안 보여야 하는 것 (컬러로 컬리젼 정의하는 렌더러라던지)
	BACKDROP,					// 배경
	UIPANEL0,
	UIPANEL1,
	UIPANEL2,
	UI,
	UIICON,
	UICOL,
	FONT,
	MOUSE,
	MAX
};

// UI 버튼 상태
enum class ButtonState
{
	Normal,						// 기본상태
	MouseOver,					// 마우스오버상태
	Click,						// 마우스클릭상태
	Disabled,					// 비활성화상태
	Max
};

// UI ProgressBar Direct
enum class ProgressBarDirect
{
	BottomToTop,				// Bottom -> Top
	TopToBottom,				// Top -> Bottom
	RightToLeft,				// Right -> Left
	LeftToRight					// Left -> Right
};

// Monster Type
enum class MonsterType
{
	NONE = -1,
	WOLF,						// 늑대
	BEAR,						// 곰
	BAT,						// 박쥐
	DOG,						// 들개
	CHICKEN,					// 닭
	BOAR,						// 멧돼지
	//WEEKLINE,					// Dr.위클라인
	MAX
};

// 몬스터상태 기본타입
enum class MonsterStateBasicType
{
	NONE = -1,
	NORMAL,
	CROWDCONTROL,
	ATTACK,
	MAX
};

// 몬스터상태 상세타입
enum class MonsterStateType
{
	NONE = -1,
	APPEAR,						// 첫등장상태
	REGEN,						// 리젠상태(몬스터 사망 후 리젠타임에 의해 리젠한 상태)
	IDLE,						// 대기상태
	HOMINGINSTINCT,				// 귀환상태
	CHASE,						// 추적상태
	HIT,						// 피격상태
	DEATH,						// 사망중상태
	DEAD,						// 사망(리젠대기)상태
	STUN,						// 스턴상태
	KNOCKBACK,					// 넉백상태
	WALLSLAM,					// 벽스턴상태
	ATK01,						// 일반공격01상태
	ATK02,						// 일반공격02상태
	SKILLATTACK,				// 스킬공격상태
	MAX
};

enum class eCollisionGroup
{
	None = 0,
	MousePointer,
	Player,
	PlayerGround,
	PlayerSight,
	PlayerAttack,
	Projectile,
	MonsterSight,
	Monster,
	MonsterAttack,
	Item,
	ItemBox,
	Trigger,
	UI,

	Unit,
	MouseRay,
	HyperLoop,

	MAX = 999
};

enum class JobType // 직업타입
{
	NONE = -1,
	YUKI,
	HYUNWOO,
	JACKIE,
	RIO,
	AYA,
	// ....
	DUMMY,
	MAX
};

enum class Location
{
	NONE = -1,
	DOCK,			// 항구
	POND,			// 연못
	BEACH,			// 모래사장
	UPTOWN,			// 고급 주택가
	ALLEY,			// 골목길
	HOTEL,			// 호텔
	AVENUE,			// 번화가
	HOSPITAL,		// 병원
	TEMPLE,			// 절
	ARCHERY_RANGE,	// 양궁장
	CEMETERY,		// 묘지
	FOREST,			// 숲
	FACTORY,		// 공장
	CHAPEL,			// 성당
	SCHOOL,			// 학교
	RESERCH_CENTRE,	// 연구소
	ESCAPE_DOCK,	// 오래된 선창
	MAX
};

enum class ItemType
{
	NONE = -1,
	EQUIPMENT,
	USEABLE,
	MISC,
	MAX
};

enum class ItemTier
{
	NONE = -1,
	COMMON,		// 회색
	UNCOMMON,	// 초록
	RARE,		// 파랑
	EPIC,		// 보라
	LEGENDARY,	// 노랑
	MAX
};

enum class EquipmentType
{
	NONE = -1,
	WEAPON,
	HEAD,
	CHEST,
	ARM,
	LEG,
	ACCESSORY,
	MAX
};

enum class ItemName							// 221020 SJH ADD : 몬스터드랍아이템확률정의로 임시(추후 협의필요)
{
	NONE = -1,
	MEAT,			// 고기(음식)
	LEATHER,		// 가죽(재료)

	MAX
};
