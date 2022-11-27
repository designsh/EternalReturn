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
	UNSEEN,						// �� ������ �ϴ� �� (�÷��� �ø��� �����ϴ� �����������)
	BACKDROP,					// ���
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

// UI ��ư ����
enum class ButtonState
{
	Normal,						// �⺻����
	MouseOver,					// ���콺��������
	Click,						// ���콺Ŭ������
	Disabled,					// ��Ȱ��ȭ����
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
	WOLF,						// ����
	BEAR,						// ��
	BAT,						// ����
	DOG,						// �鰳
	CHICKEN,					// ��
	BOAR,						// �����
	//WEEKLINE,					// Dr.��Ŭ����
	MAX
};

// ���ͻ��� �⺻Ÿ��
enum class MonsterStateBasicType
{
	NONE = -1,
	NORMAL,
	CROWDCONTROL,
	ATTACK,
	MAX
};

// ���ͻ��� ��Ÿ��
enum class MonsterStateType
{
	NONE = -1,
	APPEAR,						// ù�������
	REGEN,						// ��������(���� ��� �� ����Ÿ�ӿ� ���� ������ ����)
	IDLE,						// ������
	HOMINGINSTINCT,				// ��ȯ����
	CHASE,						// ��������
	HIT,						// �ǰݻ���
	DEATH,						// ����߻���
	DEAD,						// ���(�������)����
	STUN,						// ���ϻ���
	KNOCKBACK,					// �˹����
	WALLSLAM,					// �����ϻ���
	ATK01,						// �Ϲݰ���01����
	ATK02,						// �Ϲݰ���02����
	SKILLATTACK,				// ��ų���ݻ���
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

enum class JobType // ����Ÿ��
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
	DOCK,			// �ױ�
	POND,			// ����
	BEACH,			// �𷡻���
	UPTOWN,			// ��� ���ð�
	ALLEY,			// ����
	HOTEL,			// ȣ��
	AVENUE,			// ��ȭ��
	HOSPITAL,		// ����
	TEMPLE,			// ��
	ARCHERY_RANGE,	// �����
	CEMETERY,		// ����
	FOREST,			// ��
	FACTORY,		// ����
	CHAPEL,			// ����
	SCHOOL,			// �б�
	RESERCH_CENTRE,	// ������
	ESCAPE_DOCK,	// ������ ��â
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
	COMMON,		// ȸ��
	UNCOMMON,	// �ʷ�
	RARE,		// �Ķ�
	EPIC,		// ����
	LEGENDARY,	// ���
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

enum class ItemName							// 221020 SJH ADD : ���͵��������Ȯ�����Ƿ� �ӽ�(���� �����ʿ�)
{
	NONE = -1,
	MEAT,			// ���(����)
	LEATHER,		// ����(���)

	MAX
};
