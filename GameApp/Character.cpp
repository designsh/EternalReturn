#include "PreCompile.h"

#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SmallPostBlur.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include "UserGame.h"


#include "Character.h"
#include "LumiaLevel.h"
#include "MousePointer.h"
#include "LumiaMap.h"
#include "ItemBox.h"
#include "ItemBase.h"
#include "EquipmentItem.h"
#include "PlayerInfoManager.h"
#include "PlayerUIController.h"
#include "CharStatPacket.h"
#include "CharEffectPacket.h"
#include "GameServer.h"
#include "GameClient.h"
#include "ItemBoxManager.h"
#include "PacketSoundPlay.h"
#include "CharDeathPacket.h"
#include "SlowEffect.h"
#include "StunEffect.h"
#include "LevelUpEffect.h"
#include "DeathEffect.h"
#include "UI_Skill.h"
#include "UI_ItemBox.h"
#include "CharCrowdControlPacket.h"
#include "FogOfWar.h"
#include "GameTimeController.h"
#include "ProhibitedArea.h"
#include "Monsters.h"
#include "NoticeBroadcastPacket.h"
#include "UI_SkillGauge.h"

Character::Character()
	: collision_(nullptr)
	, currentNavFace_(nullptr)
	, currentNavMesh_(nullptr)
	, itemBoxmanager_(nullptr)
	, currentMap_(nullptr)
	, mouse_(nullptr)
	, renderer_(nullptr)
	, bFocused_(false)
	, attackCooldown_(0.f)
	, attackTime_(0.f)
	, target_(nullptr)
	, timerStun_(0.0f)
	, timerKnockback_(0.0f)
	, currentAnimation_(eCurrentAnimation::None)
	, myIndex_(-1)
	, uiController_(nullptr)
	, overrideAnimationName_("")
	, DefaultCool_Q_(1.0f)
	, DefaultCool_W_(1.0f)
	, DefaultCool_E_(1.0f)
	, DefaultCool_R_(1.0f)
	, DefaultCool_D_(1.0f)
	, coolTimer_Q_(0.0f)
	, coolTimer_W_(0.0f)
	, coolTimer_E_(0.0f)
	, coolTimer_R_(0.0f)
	, coolTimer_D_(0.0f)
	, bCoolQ_(false)
	, bCoolW_(false)
	, bCoolE_(false)
	, bCoolR_(false)
	, bCoolD_(false)
	, slowTimer_(0.0f)
	, slowRatio_(0.0f)
	, bSlowFlag_(false)
	, slowEffect_(nullptr)
	, stunEffect_(nullptr)
	, levelUpEffect_(nullptr)
	, deathEffect_(nullptr)
	, bCoolWQ_(false)
	, bCoolWW_(false)
	, bCoolWE_(false)
	, bCoolWR_(false)
	, bCoolWD_(false)
	, coolTimerW_Q_(0.5f)
	, coolTimerW_W_(0.5f)
	, coolTimerW_E_(0.5f)
	, coolTimerW_R_(0.5f)
	, coolTimerW_D_(0.5f)
	, MainSilhouetteRenderer_(nullptr)
	, WeaponSilhouetteRenderer1_(nullptr)
	, WeaponSilhouetteRenderer2_(nullptr)
	, MainOutLineRenderer_(nullptr)
	, WeaponOutLineRenderer1_(nullptr)
	, WeaponOutLineRenderer2_(nullptr)
	, eyesightVertexBuffer_(nullptr)
	, eyesightIndexBuffer_(nullptr)
	, fowRenderTarget_(nullptr)
	, fowTexture_(nullptr)
	, fowDataThread_(nullptr)
	, isInfight_(false)
	, infightTimer_(0.0f)
	, isInProhibited_(false)
	, prohibitedCounter_(20.0f)
	, bHidden_(false)
	, prohibitTimer_(0.0f)
	, curLocation_(Location::NONE)
	, prevLocation_(Location::NONE)
	, hyperLocation_(Location::NONE)
	, isPlayerWon_(false)
	, isPlayerDead_(false)
	, fraggerIndex_(-1)
	, damagePopUp_(nullptr)
	, recoveryTimeCheck_(0.0f)
	, isDebugInvincible_(false)
	, isMouseOntheUI_(false)
	, regenTimer_(0.0f)
{
	// 생성과 동시에 유닛타입 결정
	UnitType_ = UnitType::CHARACTER;
}

Character::~Character()
{
	if (nullptr != eyesightVertexBuffer_)
	{
		delete eyesightVertexBuffer_;
		eyesightVertexBuffer_ = nullptr;
	}

	if (nullptr != eyesightIndexBuffer_)
	{
		delete eyesightIndexBuffer_;
		eyesightIndexBuffer_ = nullptr;
	}

	if (nullptr != fowRenderTarget_)
	{
		delete fowRenderTarget_;
		fowRenderTarget_ = nullptr;
	}

	if (nullptr != fowTexture_)
	{
		delete fowTexture_;
		fowTexture_ = nullptr;
	}

	if (nullptr != fowDataThread_)
	{
		fowDataThread_->join();
		delete fowDataThread_;
		fowDataThread_ = nullptr;
	}
}

void Character::SetUIController()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (nullptr == uiController_)
	{
		uiController_ = GetLevel()->CreateActor<PlayerUIController>();
		uiController_->SetJobType((JobType)(pm->GetMyPlayer().character_));
	}
}

void Character::SetCurrentNavMesh(NavMesh* _NavMesh)
{
	currentNavMesh_ = _NavMesh;
	currentNavFace_ = currentNavMesh_->CurrentCheck(&transform_, float4::DOWN);
}

void Character::SetCharacterDeath()
{
	isPlayerDead_ = true;

	LumiaLevel* level = GetLevelConvert<LumiaLevel>();
	level->SubtractSurvivorCount();

	mainState_.ChangeState("DeathState", true);
	deathState_.ChangeState("PlayerDeath", true);
}

void Character::SetFraggerIndex(int _index)
{
	fraggerIndex_ = _index;
}

void Character::LoadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("EternalReturn");
	dir / "Resources" / "Texture" / "Character";

	std::vector<GameEngineFile> allFile = dir.GetAllFile("png");
	for (GameEngineFile& file : allFile)
	{
		GameEngineTextureManager::GetInst().Load(file.FileName(), file.GetFullPath());
	}
}

void Character::LevelChangeStartEvent(GameEngineLevel* _NextLevel)
{
	getItem("Bread");
	getItem("Water");
}

void Character::Start()
{
	initRendererAndAnimation();
	initInput();
	initState();
	initBasicEffect();
	//InitEyeSight();

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->GetTransform()->SetLocalScaling(150.0f);
	collision_->SetCollisionGroup(eCollisionGroup::Player);
	collision_->SetCollisionType(CollisionType::OBBBox3D);

	LumiaLevel* level = GetLevelConvert<LumiaLevel>();
	// 현재 레벨이 루미아 레벨이 아닌 경우 처리하지 않음
	if (nullptr == level)
	{
		return;
	}

	itemBoxmanager_ = level->GetItemBoxManager();
	inventory_.resize(10);
	equipedItem_.resize(static_cast<size_t>(EquipmentType::MAX));

	currentMap_ = level->GetMap();
	mouse_ = level->GetMousePointer();

	if (nullptr == currentMap_)
	{
		GameEngineDebug::MsgBoxError("level에 Map 정보가 없습니다.");
	}

	if (nullptr == mouse_)
	{
		GameEngineDebug::MsgBoxError("level에 MousePointer 정보가 없습니다.");
	}

	equipedItem_.resize(static_cast<int>(EquipmentType::MAX));
	equipBuildItem_.resize(static_cast<int>(EquipmentType::MAX));

	damagePopUp_ = GetLevel()->CreateActor<UI_DamagePopUp>();
	damagePopUp_->SetParent(this);
}

void Character::Update(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	LumiaLevel* level = GetLevelConvert<LumiaLevel>();


	DebuffCheck(_DeltaTime);		// CC가 아닌 디버프(출혈, 슬로우 등) 체크
	EffectTransformCheck(_DeltaTime);

	if (true == isPlayerDead_)
	{
		collision_->Off();
	}

	if (nullptr != collision_ &&
		collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::OBBBox3D, float4::RED);
	}


	if (false == bFocused_)
	{
		return;
	}

	// 이 밑에서부터가 "자기 캐릭터" 에만 적용되는 업데이트입니다.

	if (uiController_ != nullptr)
	{
		if (stat_.HP <= (stat_.HPMax * 0.33f))
		{
			uiController_->GetBloodBackground()->Draw(true);

			if (true == isPlayerDead_)
			{
				uiController_->GetBloodBackground()->PlayerDead();
			}
		}
		else
		{
			uiController_->GetBloodBackground()->Draw(false);
		}

		if (1 >= level->GetSurvivorCount() && false == isPlayerDead_)
		{
			uiController_->GetBloodBackground()->Draw(false);
			mainState_.ChangeState("DeathState", true);
			deathState_.ChangeState("PlayerWinner", true);
		}
	}

	CoolTimeCheck(_DeltaTime);

	if (false == isDebugInvincible_)
	{
		ProhibitedAreaCheck(_DeltaTime);
	}

	checkCurrentNavFace();
	checkItemBox();
	checkInventoryInteraction();

	if (true == isInfight_)
	{
		InfightCheck(_DeltaTime);
	}


	attackCooldown_ += _DeltaTime;

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("MainState : " + mainState_.GetCurrentStateName());
		controlWindow->AddText("NormalState : " + normalState_.GetCurrentStateName());
		controlWindow->AddText("CrowdControlState : " + crowdControlState_.GetCurrentStateName());
		controlWindow->AddText("AttackState : " + attackState_.GetCurrentStateName());
	}

	if (GameEngineInput::Down("I"))
	{
		isDebugInvincible_ = !isDebugInvincible_;
	}

	if (GameEngineInput::Down("Z"))
	{
		mainState_ << "NormalState";
		normalState_ << "Watch";
		coolTimer_Q_ = 0.5f;
		coolTimer_W_ = 0.5f;
		coolTimer_E_ = 0.5f;
		coolTimer_R_ = 0.5f;
		coolTimer_D_ = 0.5f;
		stat_.SP = stat_.SPMax;

		renderer_->ClearOverrideAnimation();
		uiController_->GetSkillGauge()->PlayFadeForce();

		CharStatPacket packet;
		packet.SetStat(stat_);
		packet.SetTargetIndex(GetIndex());
		FT::SendPacket(packet);
	}

	if (GameEngineInput::Down("M"))
	{
		GetUIController()->GetMinimapUI()->Toggle();
	}

	if (eyesightRenderer_ == nullptr)
	{
		return;
	}

	updateFOW(_DeltaTime);
	updateRecoveryItem(_DeltaTime);

	regenTimer_ -= _DeltaTime;
	if (regenTimer_ < 0.0f && !IsDead())
	{
		regenTimer_ = 2.0f;
		stat_.HP += stat_.HPRegeneration;
		stat_.SP += stat_.SPRegeneration;

		if (stat_.HP > stat_.HPMax)
		{
			stat_.HP = stat_.HPMax;
		}

		if (stat_.SP > stat_.SPMax)
		{
			stat_.SP = stat_.SPMax;
		}

		CharStatPacket packet;
		packet.SetStat(stat_);
		packet.SetTargetIndex(GetIndex());
		FT::SendPacket(packet);
	}

	mainState_.Update(_DeltaTime);

	soundChangeByMap();
}


int Character::GetIndex() { return myIndex_; }

void Character::checkCurrentNavFace()
{
	if (nullptr != currentNavFace_)
	{
		if (true == currentNavFace_->OutCheck(GetTransform()))
		{
			currentNavFace_ = currentNavFace_->MoveCheck(GetTransform());
		}
	}
	else
	{
		LumiaLevel* level = GetLevelConvert<LumiaLevel>();
		if (nullptr != level)
		{
			SetCurrentNavFace(GetLevelConvert<LumiaLevel>()->GetMap()->GetNavMesh()->CurrentCheck(GetTransform(), float4::DOWN));
		}
	}
}

void Character::getItem(int _index)
{
	// 박스가 열려있는 상태에서
	// 열려있는 UI의 아이템을 클릭하는 경우
	// 그 아이템을 가지고온다.

	ItemBase* Item = itemBoxmanager_->GetItemFromItemBox(_index);

	if (nullptr == Item)
	{
		return;
	}

	if (ItemType::EQUIPMENT == Item->GetItemType())
	{
		getEquipItem(reinterpret_cast<EquipmentItem*>(Item), _index);
		checkItemRecipes();
		return;
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr != invenItem)
		{
			continue;
		}

		invenItem = Item;
		//이건호 : 기존 UI를 비우고 인벤토리 벡터 전체를 받아오도록 설정
		uiController_->GetInventoryUI()->EmptySlot();
		uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
		itemBoxmanager_->DeleteItemFromItemBox(_index);

		std::list<ItemBase*>::iterator iter = allMyBuildItems_.begin();

		for (; iter != allMyBuildItems_.end(); ++iter)
		{
			if ((*iter)->GetName() != Item->GetName())
			{
				continue;
			}

			break;
		}

		if (iter != allMyBuildItems_.end())
		{
			allMyBuildItems_.erase(iter);
		}

		break;
	}

	checkItemRecipes();
}

void Character::getEquipItem(EquipmentItem* _item, int _index)
{
	std::list<ItemBase*>::iterator iter = allMyBuildItems_.begin();

	for (; iter != allMyBuildItems_.end(); ++iter)
	{
		if ((*iter)->GetName() != _item->GetName())
		{
			continue;
		}

		break;
	}

	if (iter != allMyBuildItems_.end())
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("equipmentInstall.wav");
		EquipmentItem* currentEquipItem = equipedItem_[static_cast<size_t>(_item->GetEquipType())];

		if (nullptr == currentEquipItem)
		{
			equipedItem_[static_cast<size_t>(_item->GetEquipType())] = reinterpret_cast<EquipmentItem*>(_item);
			itemBoxmanager_->DeleteItemFromItemBox(_index);
			allMyBuildItems_.erase(iter);
			uiController_->GetEquipUI()->PushItem(_item, _item->GetEquipType());

			stat_ += _item->GetStat();

			CharStatPacket packet;
			packet.SetStat(stat_);
			packet.SetTargetIndex(GetIndex());

			FT::SendPacket(packet);
			return;
		}

		if (currentEquipItem->GetItemTier() < _item->GetItemTier())
		{
			// 장착되있던 아이템은 인벤토리로 들어감
			bool isFull = true;
			for (auto& invenItem : inventory_)
			{
				if (nullptr != invenItem)
				{
					continue;
				}

				isFull = false;
				invenItem = currentEquipItem;
				//이건호 : 기존 UI를 비우고 인벤토리 벡터 전체를 받아오도록 설정
				uiController_->GetInventoryUI()->EmptySlot();
				uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);

				stat_ -= currentEquipItem->GetStat();
				break;
			}

			if (true == isFull)
			{
				// 인벤토리가 가득찬 경우에는 바닥에 떨어짐
			}

			equipedItem_[static_cast<size_t>(_item->GetEquipType())] = reinterpret_cast<EquipmentItem*>(_item);
			itemBoxmanager_->DeleteItemFromItemBox(_index);
			allMyBuildItems_.erase(iter);

			stat_ += _item->GetStat();

			CharStatPacket packet;
			packet.SetStat(stat_);
			packet.SetTargetIndex(GetIndex());

			FT::SendPacket(packet);
			return;
		}
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr != invenItem)
		{
			continue;
		}

		invenItem = _item;
		//이건호 : 기존 UI를 비우고 인벤토리 벡터 전체를 받아오도록 설정
		uiController_->GetInventoryUI()->EmptySlot();
		uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
		itemBoxmanager_->DeleteItemFromItemBox(_index);

		std::list<ItemBase*>::iterator iter = allMyBuildItems_.begin();

		for (; iter != allMyBuildItems_.end(); ++iter)
		{
			if ((*iter)->GetName() != _item->GetName())
			{
				continue;
			}

			break;
		}

		if (iter != allMyBuildItems_.end())
		{
			allMyBuildItems_.erase(iter);
		}
		break;
	}
}

void Character::getEquipItem(EquipmentItem* _item)
{
	std::list<ItemBase*>::iterator iter = allMyBuildItems_.begin();

	for (; iter != allMyBuildItems_.end(); ++iter)
	{
		if ((*iter)->GetName() != _item->GetName())
		{
			continue;
		}

		break;
	}

	if (iter != allMyBuildItems_.end())
	{
		static bool skipSound = true;

		if (false == skipSound)
		{
			GameEngineSoundManager::GetInstance()->PlaySoundByName("equipmentInstall.wav");
		}
		skipSound = false;

		EquipmentItem* currentEquipItem = equipedItem_[static_cast<size_t>(_item->GetEquipType())];

		if (nullptr == currentEquipItem)
		{
			equipedItem_[static_cast<size_t>(_item->GetEquipType())] = reinterpret_cast<EquipmentItem*>(_item);
			allMyBuildItems_.erase(iter);
			if (nullptr != uiController_)
			{
				uiController_->GetEquipUI()->PushItem(_item, _item->GetEquipType());
			}

			stat_ += _item->GetStat();

			CharStatPacket packet;
			packet.SetStat(stat_);
			packet.SetTargetIndex(GetIndex());

			FT::SendPacket(packet);
			return;
		}

		if (currentEquipItem->GetItemTier() < _item->GetItemTier())
		{
			// 장착되있던 아이템은 인벤토리로 들어감
			bool isFull = true;
			for (auto& invenItem : inventory_)
			{
				if (nullptr != invenItem)
				{
					continue;
				}

				isFull = false;
				invenItem = currentEquipItem;
				//이건호 : 기존 UI를 비우고 인벤토리 벡터 전체를 받아오도록 설정
				if (nullptr != uiController_)
				{
					uiController_->GetInventoryUI()->EmptySlot();
					uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
				}

				stat_ -= currentEquipItem->GetStat();
				break;
			}

			if (true == isFull)
			{
				// 인벤토리가 가득찬 경우에는 바닥에 떨어짐
			}

			equipedItem_[static_cast<size_t>(_item->GetEquipType())] = reinterpret_cast<EquipmentItem*>(_item);
			allMyBuildItems_.erase(iter);

			stat_ += _item->GetStat();

			CharStatPacket packet;
			packet.SetStat(stat_);
			packet.SetTargetIndex(GetIndex());

			FT::SendPacket(packet);
			return;
		}
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr != invenItem)
		{
			continue;
		}

		invenItem = _item;
		//이건호 : 기존 UI를 비우고 인벤토리 벡터 전체를 받아오도록 설정
		if (nullptr != uiController_)
		{
			uiController_->GetInventoryUI()->EmptySlot();
			uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
		}

		std::list<ItemBase*>::iterator iter = allMyBuildItems_.begin();

		for (; iter != allMyBuildItems_.end(); ++iter)
		{
			if ((*iter)->GetName() != _item->GetName())
			{
				continue;
			}

			break;
		}

		if (iter != allMyBuildItems_.end())
		{
			allMyBuildItems_.erase(iter);
		}
		break;
	}
}

void Character::getItem(const std::string& _itemName)
{
	ItemBase* item = itemBoxmanager_->GetItemFromItemList(_itemName);

	if (nullptr == item)
	{
		return;
	}

	if (ItemType::EQUIPMENT == item->GetItemType())
	{
		getEquipItem(reinterpret_cast<EquipmentItem*>(item));
		checkItemRecipes();
		return;
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr != invenItem)
		{
			continue;
		}

		invenItem = item;
		if (nullptr != uiController_)
		{
			uiController_->GetInventoryUI()->EmptySlot();
			uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
		}

		std::list<ItemBase*>::iterator iter = allMyBuildItems_.begin();

		for (; iter != allMyBuildItems_.end(); ++iter)
		{
			if ((*iter)->GetName() != item->GetName())
			{
				continue;
			}

			break;
		}

		if (iter != allMyBuildItems_.end())
		{
			allMyBuildItems_.erase(iter);
		}

		break;
	}

	checkItemRecipes();
}

void Character::checkItemBox()
{
	if (itemBoxmanager_ == nullptr)
	{
		return;
	}

	if (nullptr == itemBoxmanager_->GetSelectBox())
	{
		return;
	}

	if (true == itemBoxmanager_->isOpen())
	{
		if (true == GameEngineInput::GetInst().Down("LButton"))
		{
			int SlotNum = itemBoxmanager_->GetItemBoxUI()->SlotMouseCollisionCheck();

			switch (SlotNum)
			{
			case 0:
				getItem(0);
				break;
			case 1:
				getItem(1);
				break;
			case 2:
				getItem(2);
				break;
			case 3:
				getItem(3);
				break;
			case 4:
				getItem(4);
				break;
			case 5:
				getItem(5);
				break;
			case 6:
				getItem(6);
				break;
			case 7:
				getItem(7);
				break;
			default:
				break;
			}
		}
	}

	// Player와 SelectBox가 서로 충돌상태인지를 체크

	GameEngineCollision* OtherCol = collision_->GetCollision(static_cast<int>(eCollisionGroup::ItemBox));

	if (nullptr == OtherCol)
	{
		itemBoxmanager_->CloseItemBox();
		itemBoxmanager_->ResetSelectBox();
		return;
	}

	ItemBox* Box = GetLevelConvert<LumiaLevel>()->GetItemBoxManager()->GetSelectBox();

	if (nullptr == Box)
	{
		return;
	}

	if (OtherCol != Box->GetCollision())
	{
		return;
	}

	if (false == Box->IsGatherBox())
	{
		itemBoxmanager_->OpenItemBox();
		return;
	}

	// 채집물인 경우 인벤토리로 바로 획득
	if (GameEngineInput::Down("RButton") && nullptr != mouse_->GetRayCollision()->GetCollision(eCollisionGroup::ItemBox))
	{
		gatherItem();
	}

}

bool sortItemQueue(QueueItem _left, QueueItem _right)
{
	// 우선도에 따라 정렬 (루트)
	if (false == _left.isMyBuild_ &&
		true == _right.isMyBuild_)
	{
		return false;
	}

	return true;
}

void Character::checkItemRecipes()
{
	std::vector<ItemBase*> vecCheckItem;

	for (const auto& inven : inventory_)
	{
		if (nullptr != inven)
		{
			vecCheckItem.push_back(inven);
		}
	}

	for (const auto& equip : equipedItem_)
	{
		if (nullptr != equip)
		{
			vecCheckItem.push_back(equip);
		}
	}

	if (2 > vecCheckItem.size())
	{
		return;
	}

	std::vector<std::vector<int>> cases =
		GameEngineMath::Combination(static_cast<int>(vecCheckItem.size()), 2);

	std::map<CombineItem, ItemBase*>& itemRecipes = itemBoxmanager_->GetAllItemRecipes();

	queueItemMixing_.clear();

	for (int i = 0; i < cases.size(); i++)
	{
		int left = cases[i][0];
		int right = cases[i][1];

		if (nullptr == vecCheckItem[left] ||
			nullptr == vecCheckItem[right])
		{
			continue;
		}

		// 아이템 조합가능여부를 판별
		std::map<CombineItem, ItemBase*>::iterator iter = itemRecipes.end();

		CombineItem CI = CombineItem(vecCheckItem[left], vecCheckItem[right]);

		iter = itemRecipes.find(CI);

		if (itemRecipes.end() == iter)
		{
			continue;
		}

		QueueItem QI;
		QI.CI_ = CI;

		for (const auto& myItem : allMyBuildItems_)
		{
			if (myItem == (*iter).second)
			{
				QI.isMyBuild_ = true;
			}
		}

		// 조합 대기열에 등록
		queueItemMixing_.push_back(QI);

		queueItemMixing_.sort(sortItemQueue);
	}
}

void Character::mixingItem()
{
	if (queueItemMixing_.empty())
	{
		return;
	}

	CombineItem itemNames = queueItemMixing_.front().CI_;
	queueItemMixing_.pop_front();

	std::map<CombineItem, ItemBase*>& itemRecipes = itemBoxmanager_->GetAllItemRecipes();

	std::map<CombineItem, ItemBase*>::iterator iter = itemRecipes.end();
	iter = itemRecipes.find(itemNames);

	if (itemRecipes.end() == iter)
	{
		GameEngineDebug::MsgBoxError("없는 아이템 조합식입니다.");
		return;
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr == invenItem)
		{
			continue;
		}

		if (invenItem == itemNames.left_)
		{
			invenItem = nullptr;
			break;
		}
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr == invenItem)
		{
			continue;
		}

		if (invenItem == itemNames.right_)
		{
			invenItem = nullptr;
			break;
		}
	}

	bool isEquiped = false;

	for (auto& equipedItem : equipedItem_)
	{
		if (nullptr == equipedItem)
		{
			continue;
		}

		if (equipedItem == itemNames.left_)
		{
			equipedItem = nullptr;
			isEquiped = true;
			break;
		}
	}

	for (auto& equipedItem : equipedItem_)
	{
		if (nullptr == equipedItem)
		{
			continue;
		}

		if (equipedItem == itemNames.right_)
		{
			equipedItem = nullptr;
			isEquiped = true;
			break;
		}
	}

	if (true == isEquiped)
	{
		getEquipItem(reinterpret_cast<EquipmentItem*>(iter->second));

		uiController_->GetInventoryUI()->EmptySlot();
		uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);

		uiController_->GetEquipUI()->EmptySlot();
		uiController_->GetEquipUI()->GetEquipInfo(equipedItem_);
		uiController_->GetEquipUI()->ItemRenderCheck();

		checkItemRecipes();
		return;
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr != invenItem)
		{
			continue;
		}

		invenItem = iter->second;
		uiController_->GetInventoryUI()->EmptySlot();
		uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
		break;
	}

	checkItemRecipes();
}

void Character::gatherItem()
{
	UI_ItemBox* ui = itemBoxmanager_->GetItemBoxUI();

	if (ui != nullptr)
	{
		ui->Release();
		ui = nullptr;
	}

	ui = GetLevel()->CreateActor<UI_ItemBox>();
	ui->GetItemBoxInfo(itemBoxmanager_->GetSelectBox());

	itemBoxmanager_->SetItemBoxUI(ui);
	ui->Off();


	normalState_ << "Gather";
}

void Character::SetEquipBuildItem(const std::string& _itemName, EquipmentType _type)
{
	equipBuildItem_[static_cast<int>(_type)] = reinterpret_cast<EquipmentItem*>
		(itemBoxmanager_->GetItemFromItemList(_itemName));
	allMyBuildItems_.push_back(itemBoxmanager_->GetItemFromItemList(_itemName));
}

void Character::checkBuildItems()
{
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::HEAD)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::CHEST)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::ARM)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::LEG)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::ACCESSORY)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::WEAPON)]));
}

void Character::checkBuildItemsRecursive(ItemBase* _item)
{
	std::map<CombineItem, ItemBase*> Recipes = itemBoxmanager_->GetAllItemRecipes();

	for (const auto& iter : Recipes)
	{
		if (iter.second->GetName() == _item->GetName())
		{
			allMyBuildItems_.push_back(iter.first.left_);
			allMyBuildItems_.push_back(iter.first.right_);

			checkBuildItemsRecursive(iter.first.left_);
			checkBuildItemsRecursive(iter.first.right_);
		}
	}
}

void Character::checkInventoryInteraction()
{
	checkInventoryInteractionLBtn();
	checkInventoryInteractionRBtn();
	checkInventoryInteractionKey();
}

void Character::checkInventoryInteractionLBtn()
{
	if (false == GameEngineInput::GetInst().Down("LButton"))
	{
		return;
	}

	if (false == mouse_->isGrabbed())
	{
		mouseGrabItem();
	}
	else
	{
		mousePutItem();
	}
}

void Character::checkInventoryInteractionRBtn()
{
	int SlotNum = uiController_->GetInventoryUI()->SlotMouseCollisionCheck();

	if (-1 == SlotNum)
	{
		isMouseOntheUI_ = false;
		return;
	}

	isMouseOntheUI_ = true;

	if (nullptr == inventory_[SlotNum])
	{
		return;
	}

	if (false == GameEngineInput::GetInst().Down("RButton"))
	{
		return;
	}

	EquipmentItem* equip = nullptr;
	UseableItem* use = nullptr;

	switch (inventory_[SlotNum]->GetItemType())
	{
	case ItemType::EQUIPMENT:
		equip = reinterpret_cast<EquipmentItem*>(inventory_[SlotNum]);
		break;
	case ItemType::USEABLE:
		use = reinterpret_cast<UseableItem*>(inventory_[SlotNum]);
		break;
	default:
		break;
	}

	if (nullptr != equip)
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("equipmentInstall.wav");
		EquipmentItem* tmp = equipedItem_[static_cast<size_t>(equip->GetEquipType())];

		equipedItem_[static_cast<size_t>(equip->GetEquipType())] = equip;

		stat_ += equip->GetStat();

		uiController_->GetEquipUI()->EmptySlot();
		uiController_->GetEquipUI()->GetEquipInfo(equipedItem_);

		inventory_[SlotNum] = nullptr;

		for (auto& invenItem : inventory_)
		{
			if (nullptr != invenItem)
			{
				continue;
			}

			invenItem = tmp;

			if (nullptr != tmp)
			{
				stat_ -= tmp->GetStat();
			}

			uiController_->GetInventoryUI()->EmptySlot();
			uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
			break;
		}

		CharStatPacket packet;
		packet.SetStat(stat_);
		packet.SetTargetIndex(GetIndex());

		FT::SendPacket(packet);

		return;
	}

	if (nullptr != use)
	{
		useRecoveryItem(use);

		inventory_[SlotNum] = nullptr;

		uiController_->GetInventoryUI()->EmptySlot();
		uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
	}
}

void Character::checkInventoryInteractionKey()
{
	for (int SlotNum = 0; SlotNum < 10; SlotNum++)
	{
		if (9 != SlotNum)
		{
			if (false == GameEngineInput::GetInst().Down(std::to_string(SlotNum + 1)))
			{
				return;
			}
		}
		else
		{
			if (false == GameEngineInput::GetInst().Down("0"))
			{
				return;
			}
		}

		if (nullptr == inventory_[SlotNum])
		{
			return;
		}

		EquipmentItem* equip = nullptr;
		UseableItem* use = nullptr;

		switch (inventory_[SlotNum]->GetItemType())
		{
		case ItemType::EQUIPMENT:
			equip = reinterpret_cast<EquipmentItem*>(inventory_[SlotNum]);
			break;
		case ItemType::USEABLE:
			use = reinterpret_cast<UseableItem*>(inventory_[SlotNum]);
			break;
		default:
			break;
		}

		if (nullptr != equip)
		{
			GameEngineSoundManager::GetInstance()->PlaySoundByName("equipmentInstall.wav");
			EquipmentItem* tmp = equipedItem_[static_cast<size_t>(equip->GetEquipType())];

			equipedItem_[static_cast<size_t>(equip->GetEquipType())] = equip;

			stat_ += equip->GetStat();

			uiController_->GetEquipUI()->EmptySlot();
			uiController_->GetEquipUI()->GetEquipInfo(equipedItem_);

			inventory_[SlotNum] = nullptr;

			for (auto& invenItem : inventory_)
			{
				if (nullptr != invenItem)
				{
					continue;
				}

				invenItem = tmp;

				if (nullptr != tmp)
				{
					stat_ -= tmp->GetStat();
				}

				uiController_->GetInventoryUI()->EmptySlot();
				uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
				break;
			}

			CharStatPacket packet;
			packet.SetStat(stat_);
			packet.SetTargetIndex(GetIndex());

			FT::SendPacket(packet);

			return;
		}

		if (nullptr != use)
		{
			useRecoveryItem(use);

			inventory_[SlotNum] = nullptr;

			uiController_->GetInventoryUI()->EmptySlot();
			uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
		}
	}
}

void Character::useRecoveryItem(UseableItem* _item)
{
	RecoveryItem RI = {};

	RI.Type = _item->RegenType_;
	RI.RecoveryValue = _item->RegenValue_ / _item->RegenTime_;

	for (int i = 0; i < static_cast<int>(_item->RegenTime_); i++)
	{
		queueRecoveryItem_.push_back(RI);
	}

	std::string str = "Consume_";
	int value = randomGenerator.RandomInt(1, 3);

	switch (RI.Type)
	{
	case UseableItemType::HP:
		str.append("Eat_" + to_string(value));
		break;
	case UseableItemType::SP:
		str.append("Drink_" + to_string(value));
		break;
	default:
		break;
	}

	str.append(".wav");

	GameEngineSoundPlayer player;

	player.Stop();
	player.ChangeSound(str);
	player.Play();
	player.SetVolume(0.5f);
}

void Character::updateRecoveryItem(float _deltaTime)
{
	if (queueRecoveryItem_.empty())
	{
		return;
	}

	recoveryTimeCheck_ += _deltaTime;

	if (1.0f > recoveryTimeCheck_)
	{
		return;
	}

	recoveryTimeCheck_ = 0.0f;

	RecoveryItem RI = queueRecoveryItem_.front();
	queueRecoveryItem_.pop_front();

	if (UseableItemType::HP == RI.Type)
	{
		stat_.HP += RI.RecoveryValue;
		if (stat_.HP >= stat_.HPMax)
		{
			stat_.HP = stat_.HPMax;
		}
	}
	else if (UseableItemType::SP == RI.Type)
	{
		stat_.SP += RI.RecoveryValue;
		if (stat_.SP >= stat_.SPMax)
		{
			stat_.SP = stat_.SPMax;
		}
	}

	CharStatPacket packet;
	packet.SetStat(stat_);
	packet.SetTargetIndex(GetIndex());

	FT::SendPacket(packet);
}

void Character::mouseGrabItem()
{
	UI_Inventory* inven = uiController_->GetInventoryUI();

	int SlotNum = inven->SlotMouseCollisionCheck();

	if (-1 == SlotNum)
	{
		return;
	}

	if (nullptr == inventory_[SlotNum])
	{
		return;
	}

	prevInventoryItemPos_ = SlotNum;
	mouse_->GrabItem(inventory_[SlotNum], inven->GetIconRenderers()[SlotNum]);

	inventory_[SlotNum] = nullptr;

	inven->EmptySlot();
	inven->GetInventoryInfo(inventory_);
}

void Character::mousePutItem()
{
	UI_Inventory* inven = uiController_->GetInventoryUI();

	int SlotNum = inven->SlotMouseCollisionCheck();

	if (-1 == SlotNum)
	{
		mouse_->PutItem();
		checkItemRecipes();
		return;
	}

	if (nullptr == inventory_[SlotNum])
	{
		inventory_[SlotNum] = mouse_->PutItem();
	}
	else
	{
		inventory_[prevInventoryItemPos_] = inventory_[SlotNum];
		inventory_[SlotNum] = mouse_->PutItem();
	}

	inven->EmptySlot();
	inven->GetInventoryInfo(inventory_);
}

IUnit* Character::getMousePickedCharacter()
{
	{
		std::list<GameEngineCollision*> mousePickedCollision = mouse_->GetRayCollision()->GetCollisionList(eCollisionGroup::Player);
		GameEngineActor* mousePickedActor = nullptr;
		for (GameEngineCollision* c : mousePickedCollision)
		{
			mousePickedActor = c->GetActor();

			if (mousePickedActor == this)
			{
				continue;
			}

			Character* chracter = dynamic_cast<Character*>(mousePickedActor);
			if (nullptr != chracter && !chracter->IsHidden())
			{
				return dynamic_cast<IUnit*>(mousePickedActor);
			}
		}
	}

	{
		std::list<GameEngineCollision*> mousePickedCollision = mouse_->GetRayCollision()->GetCollisionList(eCollisionGroup::Monster);
		GameEngineActor* mousePickedActor = nullptr;
		for (GameEngineCollision* c : mousePickedCollision)
		{
			mousePickedActor = c->GetActor();
			Monsters* monster = dynamic_cast<Monsters*>(mousePickedActor);

			// 피킹된 몬스터가 존재하며 시야범위내있어야하며 사망상태가 아닐때 타겟지정가능
			if (nullptr != monster && false == monster->IsHidden() && false == monster->IsDead())
			{
				return dynamic_cast<IUnit*>(mousePickedActor);
			}
		}
	}

	return nullptr;
}

void Character::InitSpawnPoint(const float4& _position)
{
	//float4 spawnPoint = { -6780.f, 0.0f, -780.f };
	//float4 spawnPoint = { -2500.f, 0.0f, 10000.f };
	//float4 spawnPoint = { 0.f, 0.0f, 0.f };
	GetTransform()->SetWorldPosition(_position);
	destination_ = _position;

	currentMap_ = GetLevelConvert<LumiaLevel>()->GetMap();

	if (nullptr == currentMap_)
	{
		GameEngineDebug::MsgBox("레벨에 맵이 배치되지 않았습니다.");
	}

	// 부모인 NaviActor가 항상 자신의 위치노드를 업데이트함

	//else
	//{
	//	currentNavMesh_ = currentMap_->GetNavMesh(GetTransform()->GetWorldPosition());
	//}

	//if (nullptr == currentNavMesh_)
	//{
	//	GameEngineDebug::MsgBox("초기 캐릭터 위치가 네비게이션 메쉬 위에 있지 않습니다.");
	//}
}

void Character::Move(const float4& _position)
{
	destination_ = _position;
	destinations_.clear();
}

void Character::MoveWithPathFind(const float4& _position)
{
	destination_ = _position;

	destinations_.clear();

	if ((destination_ - transform_.GetWorldPosition()).Len3D() > FT::Char::MOVE_PATH_FIND_DISTANCE)
	{
		destinations_ = currentMap_->FindPath(transform_.GetWorldPosition(), destination_);
		if (destinations_.size() > 0)
		{
			destinations_[0] = destination_;
		}

		destination_ = transform_.GetWorldPosition();
	}
}

void Character::Hyperloop(const float4& _position, Location _location)
{
	mainState_ << "NormalState";
	normalState_ << "HyperloopBegin";

	hyperloopDest_ = _position;
	hyperLocation_ = _location;
}

void Character::Show()
{
	renderer_->On();
	bHidden_ = false;
}

void Character::Hide()
{
	renderer_->Off();



	bHidden_ = true;

}

void Character::ChangeAnimation(const std::string& _animationName, bool _bForce)
{
	if ("" == _animationName)
	{
		return;
	}

	curAnimationName_ = _animationName;
	renderer_->ChangeFBXAnimation(_animationName, _bForce);
}

void Character::ChangeOverrideAnimation(const std::string& _animationName, const std::string& _boneNameToAffect, bool _bForce)
{
	if ("" == _animationName)
	{
		renderer_->ClearOverrideAnimation();
		return;
	}

	overrideAnimationName_ = _animationName;
	overrideAnimationBoneName_ = _boneNameToAffect;
	renderer_->OverrideFBXAnimation(_animationName, _boneNameToAffect, _bForce);
}

void Character::Damage(float _Amount, IUnit* _Target)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	ResetInfight();
	LumiaLevel* level = GetLevelConvert<LumiaLevel>();

	if (0.0f >= _Amount)
	{
		return;
	}

	int DMG = static_cast<int>(_Amount);

	if (GameServer::GetInstance()->IsOpened())
	{
		GetDamagepopup()->DamageFontAppear(GetTransform()->GetLocalPosition(), to_string(DMG));
	}

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("DamagePopup");
	pack.SetPopupDamage(static_cast<float>(DMG));
	FT::SendPacket(pack);

	stat_.HP -= _Amount;

	if (stat_.HP <= 0.f && false == isPlayerDead_)
	{
		//e	FT::PlaySoundAndSendPacket("death.wav", transform_.GetWorldPosition());
	//	FT::PlaySoundAllAndSendPacket("Announce_Player_Kill_01.wav");

		if (GameServer::GetInstance()->IsOpened())
		{
			this->SetCharacterDeath();

			//	GetUIController()->GetMinimapUI()->RemoveIcon(myIndex_);

			CharDeathPacket deathpacket;
			deathpacket.SetTargetIndex(myIndex_);

			if (_Target == nullptr)
			{
				// 금지구역 폭사
				deathEffect_->PlayAwake(true);
				deathpacket.SetFraggerType(static_cast<int>(UnitType::NONE));
				deathpacket.SetFraggerIndex(-3);

				fraggerIndex_ = -3;

				CharEffectPacket pack;
				pack.SetTargetIndex(myIndex_);
				pack.SetAnimationName("Death_Explode");
				FT::SendPacket(pack);

				FT::PlaySoundAllAndSendPacket("Announce_Player_Kill_01.wav");
				if (true == GameServer::GetInstance()->IsOpened())
				{
					level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetNoticeUI()->SetText(pm->GetMyPlayer().playerNickname_ + "가 금지 구역에서 살해당했습니다.", 3.0f);
				}

				NoticeBroadcastPacket packet;
				packet.SetString(pm->GetMyPlayer().playerNickname_ + "가 금지 구역에서 사망했습니다.");
				packet.SetTimer(3.0f);

				FT::SendPacket(packet);
			}
			else if (_Target->UnitType_ == UnitType::CHARACTER)
			{
				deathEffect_->PlayAwake(false);
				Character* charTmp = dynamic_cast<Character*>(_Target);
				int tmpIndex = charTmp->GetIndex();
				deathpacket.SetFraggerType(static_cast<int>(UnitType::CHARACTER));
				deathpacket.SetFraggerIndex(tmpIndex);

				CharEffectPacket pack;
				pack.SetTargetIndex(myIndex_);
				pack.SetAnimationName("Death");
				FT::SendPacket(pack);


				FT::PlaySoundAllAndSendPacket("Announce_Player_Kill_01.wav");
				if (true == GameServer::GetInstance()->IsOpened())
				{
					level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetNoticeUI()->SetText(pm->GetPlayerList()[myIndex_].playerNickname_ + "가 " + pm->GetPlayerList()[tmpIndex].playerNickname_ + " 에게 살해당했습니다.", 3.0f);
				}
				NoticeBroadcastPacket packet;
				packet.SetString(pm->GetPlayerList()[myIndex_].playerNickname_ + "가 " + pm->GetPlayerList()[tmpIndex].playerNickname_ + " 에게 살해당했습니다.");
				packet.SetTimer(3.0f);

				FT::SendPacket(packet);
			}
			else if (_Target->UnitType_ == UnitType::MONSTER)
			{
				// animal
				deathEffect_->PlayAwake(false);
				deathpacket.SetFraggerType(static_cast<int>(UnitType::MONSTER));
				deathpacket.SetFraggerIndex(-2);

				fraggerIndex_ = -2;

				CharEffectPacket pack;
				pack.SetTargetIndex(myIndex_);
				pack.SetAnimationName("Death");
				FT::SendPacket(pack);

				FT::PlaySoundAllAndSendPacket("Announce_Player_Kill_01.wav");
				if (true == GameServer::GetInstance()->IsOpened())
				{
					level->GetCharacterActorList()[pm->GetMyNumber()]->GetUIController()->GetNoticeUI()->SetText(pm->GetMyPlayer().playerNickname_ + "가 몬스터에게 살해당했습니다.", 3.0f);
				}
				NoticeBroadcastPacket packet;
				packet.SetString(pm->GetMyPlayer().playerNickname_ + "가 몬스터에게 살해당했습니다.");
				packet.SetTimer(3.0f);

				FT::SendPacket(packet);
			}


			FT::SendPacket(deathpacket);

		}
		else if (GameClient::GetInstance()->IsConnected())
		{
			CharDeathPacket deathpacket;
			deathpacket.SetTargetIndex(myIndex_);

			if (_Target == nullptr)
			{
				// 금지구역 폭사
				deathEffect_->PlayAwake(true);
				deathpacket.SetFraggerType(static_cast<int>(UnitType::NONE));
				deathpacket.SetFraggerIndex(-3);

				CharEffectPacket pack;
				pack.SetTargetIndex(myIndex_);
				pack.SetAnimationName("Death_Explode");
				FT::SendPacket(pack);


				FT::PlaySoundAllAndSendPacket("Announce_Player_Kill_01.wav");
				NoticeBroadcastPacket packet;
				packet.SetString(pm->GetMyPlayer().playerNickname_ + "가 금지 구역에서 사망했습니다.");
				packet.SetTimer(3.0f);

				FT::SendPacket(packet);
			}
			else if (_Target->UnitType_ == UnitType::CHARACTER)
			{
				deathEffect_->PlayAwake(false);
				Character* charTmp = dynamic_cast<Character*>(_Target);
				int tmpIndex = charTmp->GetIndex();
				deathpacket.SetFraggerType(static_cast<int>(UnitType::CHARACTER));
				deathpacket.SetFraggerIndex(tmpIndex);
				CharEffectPacket pack;
				pack.SetTargetIndex(myIndex_);
				pack.SetAnimationName("Death");
				FT::SendPacket(pack);


				FT::PlaySoundAllAndSendPacket("Announce_Player_Kill_01.wav");
				NoticeBroadcastPacket packet;
				packet.SetString(pm->GetPlayerList()[myIndex_].playerNickname_ + "가 " + pm->GetPlayerList()[tmpIndex].playerNickname_ + " 에게 살해당했습니다.");
				packet.SetTimer(3.0f);

				FT::SendPacket(packet);
			}
			else if (_Target->UnitType_ == UnitType::MONSTER)
			{
				// animal
				deathEffect_->PlayAwake(false);
				deathpacket.SetFraggerType(static_cast<int>(UnitType::MONSTER));
				deathpacket.SetFraggerIndex(-2);
				CharEffectPacket pack;
				pack.SetTargetIndex(myIndex_);
				pack.SetAnimationName("Death");
				FT::SendPacket(pack);


				FT::PlaySoundAllAndSendPacket("Announce_Player_Kill_01.wav");
				NoticeBroadcastPacket packet;
				packet.SetString(pm->GetMyPlayer().playerNickname_ + "가 몬스터에게 살해당했습니다.");
				packet.SetTimer(3.0f);

				FT::SendPacket(packet);
			}


			FT::SendPacket(deathpacket);
		}
	}


	CharStatPacket packet;
	packet.SetStat(stat_);
	packet.SetTargetIndex(GetIndex());

	FT::SendPacket(packet);
}

void Character::LevelUP(LevelUPData _Data)
{
	// 레벨이없네!!!!!
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();


	stat_.level += 1;
	stat_.AttackPower += _Data.AttackPower_;					// 공격력
	stat_.HPMax += _Data.HP_;									// 체력(최대체력)
	stat_.HPRegeneration += _Data.HPRegeneration_;				// 체력재생
	stat_.SPMax += _Data.SP_;									// 스태미나(최대스태미나)
	stat_.SPRegeneration += _Data.SPRegeneration_;				// 스태미나재생
	stat_.Defence += _Data.Defence_;							// 방어력
	stat_.AttackSpeed += _Data.AttackSpeed_;					// 공격속도
	stat_.CriticalChance += _Data.CriticalChance_;				// 치명타
	stat_.MovementSpeed += _Data.MovementSpeed_;				// 이동속도
	stat_.VisionRange += _Data.VisionRange_;					// 시야

	if (5 > stat_.Level_q)										// 스킬 레벨은 각 최대 스킬 레벨(5,3) 만큼만 일괄적으로 오릅니다.
	{
		stat_.Level_q++;
		stat_.Level_w++;
		stat_.Level_e++;
	}

	if (3 > stat_.Level_r)
	{
		stat_.Level_r++;
		stat_.Level_passive++;
	}

	if (2 > stat_.Level_d)
	{
		stat_.Level_d++;
	}
	if (myIndex_ == pm->GetMyNumber())
	{
		uiController_->GetSkillUI()->SetStatus(&stat_);
	}
	onLevelUp();

	levelUpEffect_->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
	levelUpEffect_->PlayAwake();
}

void Character::PickingOutLineOn()
{
	if (nullptr != MainOutLineRenderer_)
	{
		MainOutLineRenderer_->On();
	}

	if (nullptr != WeaponOutLineRenderer1_)
	{
		WeaponOutLineRenderer1_->On();
	}

	if (nullptr != WeaponOutLineRenderer2_)
	{
		WeaponOutLineRenderer2_->On();
	}
}

void Character::PickingOutLineOff()
{
	if (nullptr != MainOutLineRenderer_)
	{
		MainOutLineRenderer_->Off();
	}

	if (nullptr != WeaponOutLineRenderer1_)
	{
		WeaponOutLineRenderer1_->Off();
	}

	if (nullptr != WeaponOutLineRenderer2_)
	{
		WeaponOutLineRenderer2_->Off();
	}
}

void Character::Stun(float _stunTime)
{
	ResetInfight();

	if (true == stat_.isUnstoppable_)
	{
		return;
	}

	mainState_ << "CrowdControlState";
	crowdControlState_ << "Stun";
	timerStun_ = _stunTime;
	changeAnimationWait();

	stunEffect_->PlayAwake(_stunTime);
}


void Character::Knockback(float _knockbackTime, float4 _knockbackSpeed)
{
	ResetInfight();

	if (true == stat_.isUnstoppable_)
	{
		return;
	}

	mainState_.ChangeState("CrowdControlState", true);
	crowdControlState_.ChangeState("Knockback", true);
	timerKnockback_ = _knockbackTime;
	knockbackSpeed_ = _knockbackSpeed;
}

void Character::WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime)
{
	ResetInfight();

	if (true == stat_.isUnstoppable_)
	{
		return;
	}

	mainState_ << "CrowdControlState";
	crowdControlState_ << "WallSlam";
	timerKnockback_ = _knockbackTime;
	knockbackSpeed_ = _knockbackSpeed;
	timerStun_ = _stunTime;
}

void Character::Slow(float _slowTime, float _slowRatio)
{
	ResetInfight();

	if (true == stat_.isUnstoppable_)
	{
		return;
	}

	slowTimer_ = _slowTime;

	if (1.0f < _slowRatio)
	{
		slowRatio_ = 1.0f;
	}
	else
	{
		slowRatio_ = _slowRatio;
	}


	slowEffect_->PlayAwake(_slowTime);

	bSlowFlag_ = true;
}

void Character::initInput()
{
	GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
	GameEngineInput::GetInst().CreateKey("Q", 'Q');
	GameEngineInput::GetInst().CreateKey("W", 'W');
	GameEngineInput::GetInst().CreateKey("E", 'E');
	GameEngineInput::GetInst().CreateKey("R", 'R');
	GameEngineInput::GetInst().CreateKey("D", 'D');
	GameEngineInput::GetInst().CreateKey("A", 'A');
	GameEngineInput::GetInst().CreateKey("S", 'S');
	GameEngineInput::GetInst().CreateKey("M", 'M');
	GameEngineInput::GetInst().CreateKey("V", 'V');
	GameEngineInput::GetInst().CreateKey("Tab", VK_TAB);
	GameEngineInput::GetInst().CreateKey("Mixing", VK_OEM_3);	// ~ Key
}

void Character::initState()
{
	mainState_.CreateState(MakeStateWithEnd(Character, NormalState));
	mainState_.CreateState(MakeState(Character, AttackState));
	mainState_.CreateState(MakeState(Character, CrowdControlState));
	mainState_.CreateState(MakeState(Character, CustomState));
	mainState_.CreateState(MakeState(Character, DeathState));

	normalState_.CreateState(MakeState(Character, Watch));
	normalState_.CreateState(MakeState(Character, Stop));
	normalState_.CreateState(MakeState(Character, Run));
	normalState_.CreateState(MakeState(Character, Chase));
	normalState_.CreateState(MakeState(Character, MixItem));
	normalState_.CreateState(MakeState(Character, Craft));
	normalState_.CreateState(MakeState(Character, Cook));
	normalState_.CreateState(MakeState(Character, HyperloopBegin));
	normalState_.CreateState(MakeState(Character, HyperloopEnd));
	normalState_.CreateState(MakeState(Character, RestBegin));
	normalState_.CreateState(MakeState(Character, RestLoop));
	normalState_.CreateState(MakeState(Character, RestEnd));
	normalState_.CreateState(MakeState(Character, Gather));

	attackState_.CreateState(MakeState(Character, BasicAttack));
	attackState_.CreateState(MakeState(Character, BasicAttacking));
	attackState_.CreateState(MakeState(Character, QSkill));
	attackState_.CreateState(MakeState(Character, WSkill));
	attackState_.CreateState(MakeState(Character, ESkill));
	attackState_.CreateState(MakeState(Character, RSkill));
	attackState_.CreateState(MakeState(Character, DSkill));

	crowdControlState_.CreateState(MakeState(Character, Stun));
	crowdControlState_.CreateState(MakeState(Character, Knockback));
	crowdControlState_.CreateState(MakeState(Character, WallSlam));

	deathState_.CreateState(MakeState(Character, PlayerAlive));
	deathState_.CreateState(MakeState(Character, PlayerDeath));
	deathState_.CreateState(MakeState(Character, PlayerWinner));


	mainState_ << "NormalState";

	normalState_ << "Watch";

	crowdControlState_ << "Stun";

	deathState_ << "PlayerAlive";
}

void Character::InitEyeSight()
{
	if (eyesightVertexBuffer_ != nullptr)
	{
		return;
	}

	eyesightVertices_.reserve(37);
	eyesightIndices_.reserve(36 * 3);

	for (size_t i = 0; i < 37; i++)
	{
		GameEngineVertex v;
		v.POSITION = { 0.0f, 0.0f, 0.0f, 1.0f };
		v.COLOR = float4::WHITE;
		eyesightVertices_.push_back(v);
	}

	for (int i = 0; i < 36 - 1; i++)
	{
		eyesightIndices_.push_back(i + 0);
		eyesightIndices_.push_back(i + 1);
		eyesightIndices_.push_back(36);
	}

	eyesightIndices_.push_back(35);
	eyesightIndices_.push_back(0);
	eyesightIndices_.push_back(36);

	eyesightVertexBuffer_ = new GameEngineVertexBuffer();
	eyesightIndexBuffer_ = new GameEngineIndexBuffer();

	eyesightVertexBuffer_->Create(eyesightVertices_, D3D11_USAGE::D3D11_USAGE_DYNAMIC);
	eyesightIndexBuffer_->Create(eyesightIndices_, D3D11_USAGE::D3D11_USAGE_DEFAULT);

	eyesightRenderer_ = CreateTransformComponent<GameEngineRenderer>(nullptr);
	eyesightRenderer_->SetRenderingPipeLine("Color");
	eyesightRenderer_->SetMesh(eyesightVertexBuffer_, eyesightIndexBuffer_);
	eyesightRenderer_->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerWireFrame");
	eyesightRenderer_->GetGameEngineRenderingPipeLine()->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	eyesightRenderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", fowColor_);

	fowRenderTarget_ = new GameEngineRenderTarget;
	fowTexture_ = new GameEngineTexture;
	fowTexture_->Create(float4(1280.f, 720.f), DXGI_FORMAT_R8G8B8A8_UNORM);
	fowRenderTarget_->Create(fowTexture_, float4::BLACK);
	fowRenderTarget_->Clear();


	FogOfWar* fow = level_->AddPostProcessCameraMergePrev<FogOfWar>();
	fow->SetTarget(level_->GetMainCamera()->GetCameraRenderTarget());
	fow->SetFilter(fowRenderTarget_);


	GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
	if (Window != nullptr)
	{
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("FOW", fowRenderTarget_, Size * 3);
	}
}

void Character::updateFOW(float _deltaTime)
{
	if (level_->GetMainCamera()->IsDebugCheck())
	{
		eyesightRenderer_->Off();
	}
	else
	{
		eyesightRenderer_->On();
	}

	static bool bCalc = false;

	if (bCalc)
	{
		return;
	}
	else
	{
		static float sightUpdateTime = 0.0f;
		sightUpdateTime += _deltaTime;

		if (sightUpdateTime > 0.0166f)
		{
			if (nullptr != fowDataThread_)
			{
				fowDataThread_->join();
				delete fowDataThread_;
				fowDataThread_ = nullptr;
			}

			std::function<void(std::vector<float4>&, bool&)> func = std::bind(&Character::getFOWData, this, std::placeholders::_1, std::placeholders::_2);

			fowDataThread_ = new std::thread(func, std::ref(fowData_), std::ref(bCalc));

			bCalc = true;

			sightUpdateTime = 0.0f;
		}
		else
		{
			return;
		}
	}

	DayAndNightType dayType = GameTimeController::GetInstance()->GetCurrentDayType();
	if (dayType == DayAndNightType::DAY)
	{
		stat_.VisionRange = FT::Char::DEFAULT_VISION_RANGE_DAY;
	}
	else if (dayType == DayAndNightType::NIGHT)
	{
		stat_.VisionRange = FT::Char::DEFAULT_VISION_RANGE_NIGHT;
	}

	if (bFocused_)
	{
		ID3D11DeviceContext* dc = GameEngineDevice::GetContext();

		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		//std::vector<float4> result = currentMap_->GetEyeSightPolygon(transform_.GetWorldPosition());

		//for (size_t i = 0; i < result.size(); i++)
		//{
		//	eyesightVertices_[i].POSITION = result[i];
		//}

		for (size_t i = 0; i < fowData_.size(); i++)
		{
			eyesightVertices_[i].POSITION = fowData_[i];
		}

		eyesightVertices_.back().POSITION = transform_.GetWorldPosition();

		dc->Map(eyesightVertexBuffer_->Buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		memcpy(subResource.pData, &eyesightVertices_[0], subResource.RowPitch);
		dc->Unmap(eyesightVertexBuffer_->Buffer_, 0);
	}

	CameraComponent* cam = level_->GetMainCamera();
	cam->CameraTransformUpdate();

	float4x4 View = cam->GetTransform()->GetTransformData().View_;
	float4x4 Projection = cam->GetTransform()->GetTransformData().Projection_;

	fowRenderTarget_->Clear();
	fowRenderTarget_->Setting();
	eyesightRenderer_->GetTransform()->GetTransformData().Projection_ = Projection;
	eyesightRenderer_->GetTransform()->GetTransformData().View_ = View;

	eyesightRenderer_->GetTransform()->GetTransformData().WVPCalculation();
	eyesightRenderer_->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerNone");
	eyesightRenderer_->Render(_deltaTime, false);
	eyesightRenderer_->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerWireFrame");

	//	SmallPostBlur* BlurEffect = AddPostProcessCameraMergeNext<SmallPostBlur>();
//	BlurEffect->SetTarget(GameEngineDevice::GetBackBufferTarget());
//	BlurEffect->SetFilter("BlurFilter.png");

	SmallPostBlur blur;
	blur.Initialize();
	blur.SetTarget(fowRenderTarget_);
	blur.SetFilter("BlurFilter.png");
	blur.Effect(_deltaTime);
	blur.Effect(_deltaTime);
	blur.Effect(_deltaTime);
}

void Character::getFOWData(std::vector<float4>& _data, bool& _bCalc)
{
	_data.clear();
	_data = currentMap_->GetEyeSightPolygon(transform_.GetWorldPosition(), stat_.VisionRange);
	_bCalc = false;
}

void Character::soundChangeByMap()
{
	if (curLocation_ == prevLocation_)
	{
		return;
	}

	LumiaLevel* level = GetLevelConvert<LumiaLevel>();

	if ("LumiaLevel" != level->GetName())
	{
		return;
	}

	playAreaBGMsound(GetCurLocation());

	prevLocation_ = curLocation_;
}

void Character::playAreaBGMsound(Location _value)
{
	LumiaLevel* level = GetLevelConvert<LumiaLevel>();

	if ("LumiaLevel" != level->GetName())
	{
		return;
	}

	int mapIndex = static_cast<int>(_value);
	if (15 == mapIndex)
	{
		return;
	}

	std::string str = "BSER_AreaBGM_";
	str.append(level->GetMap()->GetMapName(mapIndex));
	str.append(".wav");

	areaSoundPlayer_.Stop();
	areaSoundPlayer_.ChangeSound(str);
	areaSoundPlayer_.Play();
	areaSoundPlayer_.SetVolume(0.3f);
}

void Character::initBasicEffect()
{
	slowEffect_ = GetLevel()->CreateActor<SlowEffect>();
	slowEffect_->GetTransform()->SetLocalPosition(this->GetTransform()->GetLocalPosition());
	slowEffect_->SetParent(this);

	stunEffect_ = GetLevel()->CreateActor<StunEffect>();
	stunEffect_->GetTransform()->SetLocalPosition(this->GetTransform()->GetLocalPosition());
	stunEffect_->SetParent(this);

	levelUpEffect_ = GetLevel()->CreateActor<LevelUpEffect>();
	levelUpEffect_->GetTransform()->SetLocalPosition(this->GetTransform()->GetLocalPosition());
	levelUpEffect_->SetParent(this);

	deathEffect_ = GetLevel()->CreateActor<DeathEffect>();
	deathEffect_->GetTransform()->SetLocalPosition(this->GetTransform()->GetLocalPosition());
	deathEffect_->SetParent(this);
}


void Character::inputProcess(float _deltaTime)
{
	if (mouse_ == nullptr)
	{
		return;
	}
	GameEngineCollision* rayCol = mouse_->GetRayCollision();

	bool result = false;
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	if (GameEngineInput::Press("RButton") || GameEngineInput::Down("RButton"))
	{

		IUnit* otherCharacter = getMousePickedCharacter();
		if (nullptr != otherCharacter && otherCharacter != this)
		{
			// 공격 처리
			target_ = otherCharacter;
			normalState_ << "Chase";
			return;
		}
		else
		{
			target_ = nullptr;
			result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);
			if (result)
			{
				Move(destination_);
			}
			else
			{
				destination_ = mousePosition;
			}
		}

	}
	else if (GameEngineInput::Up("RButton"))
	{
		bool result = false;
		result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);
		if (result)
		{
			result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);

			if (result)
			{
				MoveWithPathFind(mousePosition);
			}
			else
			{
				destination_ = mousePosition;
			}
		}
	}

	if (GameEngineInput::Up("X"))
	{
		normalState_ << "RestBegin";
	}
}

void Character::moveProcess(float _deltaTime)
{
	float4 worldPosition = transform_.GetWorldPosition();
	worldPosition.y = destination_.y;
	if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	{
		moveTick(_deltaTime, worldPosition);
	}
	else
	{
		if (!destinations_.empty())
		{
			destination_ = destinations_.back();
			destinations_.pop_back();

			// 여기서 한번 더 해주지 않으면 움직임이 1 프레임 손실된다.
			if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
			{
				moveTick(_deltaTime, worldPosition);
			}
		}
		else
		{
			normalState_ << "Watch";
			changeAnimationWait();
			return;
		}
	}
}

void Character::moveTick(float _deltaTime, const float4& _startPosition)
{
	setRotationTo(destination_, _startPosition);

	float4 moveSpeed = (direction_ * stat_.MovementSpeed * stat_.MovementRatio) * _deltaTime;
	float4 nextMovePosition = _startPosition + moveSpeed;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{

		changeAnimationRun();
		currentAnimation_ = eCurrentAnimation::Run;
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		destination_ = _startPosition;
	}
}

void Character::moveTickLockDirection(float _deltaTime, const float4& _startPosition)
{
	float4 direction = destination_ - _startPosition;
	direction.Normalize3D();
	float4 moveSpeed = (direction * stat_.MovementSpeed * stat_.MovementRatio) * _deltaTime;
	float4 nextMovePosition = _startPosition + moveSpeed;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		destination_ = _startPosition;
	}
}

void Character::setRotationTo(const float4& _destination, const float4 _startPosition)
{
	direction_ = _destination - _startPosition;
	direction_.Normalize3D();

	float4 cross = float4::Cross3D(direction_, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction_, { 0.0f, 0.0f, 1.0f });

	transform_.SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });
}

void Character::setRotationToMouse()
{
	if (mouse_ == nullptr)
	{
		changeAnimationWait();
		mainState_ << "NormalState";
		return;
	}
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	setRotationTo(mousePosition, transform_.GetWorldPosition());
}

void Character::startNormalState()
{
}

void Character::updateNormalState(float _deltaTime)
{
	if (bFocused_)
	{
		if (true == GameEngineInput::GetInst().Down("Q"))
		{
			if (false == bCoolQ_)
			{
				if (stat_.SP < stat_.SP_Q)
				{
					uiController_->GetNoticeUI()->SetText("SP가 부족하여 사용할 수 없습니다!", 2.f);
					return;
				}
				else
				{
					stat_.SP -= stat_.SP_Q;

					CharStatPacket packet;
					packet.SetStat(stat_);
					packet.SetTargetIndex(GetIndex());

					FT::SendPacket(packet);
				}

				if (uiController_ != nullptr)
				{
					uiController_->GetSkillUI()->GetCoolTimeMaskQ()->SetImage("cooltime_mask.png", "PointSmp");
					uiController_->GetSkillUI()->GetCoolTimeMaskQ()->SetAlpha(1.0f);
					uiController_->GetSkillUI()->GetCoolTimeMaskQ()->On();
				}

				bCoolQ_ = true;
				coolTimer_Q_ = DefaultCool_Q_;
				mainState_.ChangeState("AttackState", true);
				attackState_.ChangeState("QSkill", true);
			}
			else
			{
				uiController_->GetNoticeUI()->SetText("지금은 사용할 수 없습니다!", 2.f);
			}
			return;
		}


		if (true == GameEngineInput::GetInst().Down("W"))
		{
			if (false == bCoolW_)
			{
				if (stat_.SP < stat_.SP_W)
				{
					uiController_->GetNoticeUI()->SetText("SP가 부족하여 사용할 수 없습니다!", 2.f);
					return;
				}
				else
				{
					stat_.SP -= stat_.SP_W;

					CharStatPacket packet;
					packet.SetStat(stat_);
					packet.SetTargetIndex(GetIndex());

					FT::SendPacket(packet);
				}

				if (uiController_ != nullptr)
				{
					uiController_->GetSkillUI()->GetCoolTimeMaskW()->SetImage("cooltime_mask.png", "PointSmp");
					uiController_->GetSkillUI()->GetCoolTimeMaskW()->SetAlpha(1.0f);
					uiController_->GetSkillUI()->GetCoolTimeMaskW()->On();
				}
				bCoolW_ = true;
				coolTimer_W_ = DefaultCool_W_;
				mainState_.ChangeState("AttackState", true);
				attackState_.ChangeState("WSkill", true);
			}
			else
			{
				uiController_->GetNoticeUI()->SetText("지금은 사용할 수 없습니다!", 2.f);
			}


			return;
		}

		if (true == GameEngineInput::GetInst().Down("E"))
		{
			if (false == bCoolE_)
			{
				if (stat_.SP < stat_.SP_E)
				{
					uiController_->GetNoticeUI()->SetText("SP가 부족하여 사용할 수 없습니다!", 2.f);
					return;
				}
				else
				{
					stat_.SP -= stat_.SP_E;

					CharStatPacket packet;
					packet.SetStat(stat_);
					packet.SetTargetIndex(GetIndex());
					FT::SendPacket(packet);
				}


				if (uiController_ != nullptr)
				{
					uiController_->GetSkillUI()->GetCoolTimeMaskE()->SetImage("cooltime_mask.png", "PointSmp");
					uiController_->GetSkillUI()->GetCoolTimeMaskE()->SetAlpha(1.0f);
					uiController_->GetSkillUI()->GetCoolTimeMaskE()->On();
				}
				bCoolE_ = true;
				coolTimer_E_ = DefaultCool_E_;
				mainState_.ChangeState("AttackState", true);
				attackState_.ChangeState("ESkill", true);


			}
			else
			{
				uiController_->GetNoticeUI()->SetText("지금은 사용할 수 없습니다!", 2.f);
			}



			return;
		}

		if (true == GameEngineInput::GetInst().Down("R"))
		{

			if (false == bCoolR_)
			{
				if (stat_.SP < stat_.SP_R)
				{
					uiController_->GetNoticeUI()->SetText("SP가 부족하여 사용할 수 없습니다!", 2.f);
					return;
				}
				else
				{
					stat_.SP -= stat_.SP_R;

					CharStatPacket packet;
					packet.SetStat(stat_);
					packet.SetTargetIndex(GetIndex());

					FT::SendPacket(packet);
				}

				if (uiController_ != nullptr)
				{
					uiController_->GetSkillUI()->GetCoolTimeMaskR()->SetImage("cooltime_mask.png", "PointSmp");
					uiController_->GetSkillUI()->GetCoolTimeMaskR()->SetAlpha(1.0f);
					uiController_->GetSkillUI()->GetCoolTimeMaskR()->On();
				}
				bCoolR_ = true;
				coolTimer_R_ = DefaultCool_R_;
				mainState_.ChangeState("AttackState", true);
				attackState_.ChangeState("RSkill", true);


			}
			else
			{
				uiController_->GetNoticeUI()->SetText("지금은 사용할 수 없습니다!", 2.f);
			}



			return;
		}

		if (true == GameEngineInput::GetInst().Down("D") && false == bCoolD_)
		{
			if (false == bCoolD_)
			{
				if (uiController_ != nullptr)
				{
					uiController_->GetSkillUI()->GetCoolTimeMaskD()->SetImage("cooltime_mask.png", "PointSmp");
					uiController_->GetSkillUI()->GetCoolTimeMaskD()->SetAlpha(1.0f);
					uiController_->GetSkillUI()->GetCoolTimeMaskD()->On();
				}
				bCoolD_ = true;
				coolTimer_D_ = DefaultCool_D_;
				mainState_.ChangeState("AttackState", true);
				attackState_.ChangeState("DSkill", true);
			}
			else
			{
				uiController_->GetNoticeUI()->SetText("지금은 사용할 수 없습니다!", 2.f);

			}


			return;
		}

		if (true == GameEngineInput::Down("Mixing"))
		{
			// 큐에 들어있는 조합을 실행
			// Mixing State 따로 만들 예정
			//mixingItem();
			normalState_ << "MixItem";
			return;
		}

		if (true == uiController_->GetInventoryUI()->CraftCollisionMouseCheck())
		{
			//인벤토리에 있는 제작버튼을 좌클릭해서도 제작 가능
			if (true == GameEngineInput::GetInst().Down("LButton"))
			{
				normalState_ << "MixItem";
				return;
			}
		}
	}



	// Normal State 업데이트
	normalState_.Update(_deltaTime);


	// 업데이트 후에 높이 체크
	checkCurrentNavFace();
	if (nullptr != currentNavFace_)
	{
		float Dist = currentNavFace_->YCheck(GetTransform());
		transform_.SetWorldMove({ 0.0f, -Dist, 0.0f });
	}
}

void Character::endNormalState()
{
	//destination_ = transform_.GetWorldPosition();
	destinations_.clear();
}

void Character::startCrowdControlState()
{
}

void Character::updateCrowdControlState(float _deltaTime)
{
	crowdControlState_.Update(_deltaTime);
}

void Character::startAttackState()
{

}

void Character::updateAttackState(float _deltaTime)
{
	attackState_.Update(_deltaTime);

	checkCurrentNavFace();
	if (nullptr != currentNavFace_)
	{
		float Dist = currentNavFace_->YCheck(GetTransform());
		transform_.SetWorldMove({ 0.0f, -Dist, 0.0f });
	}
}

void Character::startCustomState()
{
}

void Character::updateCustomState(float _deltaTime)
{
	onUpdateCustomState(_deltaTime);
}

void Character::startDeathState()
{
	changeDeathAnimation();
}

void Character::updateDeathState(float _deltaTime)
{
	deathState_.Update(_deltaTime);
}

void Character::startWatch()
{
	changeAnimationWait();
	currentAnimation_ = eCurrentAnimation::Wait;
}

void Character::updateWatch(float _deltaTime)
{
	inputProcess(_deltaTime);

	float4 worldPosition = GetTransform()->GetWorldPosition();
	worldPosition.y = destination_.y;
	if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	{
		normalState_ << "Run";
		return;
	}
}

void Character::startStop()
{
}

void Character::updateStop(float _deltaTime)
{
}

void Character::startRun()
{
}

void Character::updateRun(float _deltaTime)
{

	inputProcess(_deltaTime);
	moveProcess(_deltaTime);
}

void Character::startChase()
{
}

void Character::updateChase(float _deltaTime)
{
	inputProcess(_deltaTime);
	if (target_ == nullptr)
	{
		normalState_ << "Watch";
		return;
	}


	float4 targetPosition = target_->GetTransform()->GetWorldPosition();
	float4 playerPosition = transform_.GetWorldPosition();
	float distance = float4::Calc_Len3D(playerPosition, targetPosition);

	//if (distance > stat_.VisionRange)
	//{
	//	normalState_ << "Watch";
	//	target_ = nullptr;
	//	return;
	//}

	if (distance < stat_.AttackRange)
	{
		setRotationTo(targetPosition, playerPosition);

		if (attackCooldown_ > stat_.AttackSpeedRatio / stat_.AttackSpeed)
		{
			mainState_ << "AttackState";
			attackState_ << "BasicAttack";
			return;
		}

		if (currentAnimation_ == eCurrentAnimation::BasicAttack)
		{
			if (renderer_->IsCurrentAnimationEnd())
			{
				changeAnimationWait();
				currentAnimation_ = eCurrentAnimation::Wait;
			}
		}
		else if (currentAnimation_ == eCurrentAnimation::Run)
		{
			changeAnimationWait();
			currentAnimation_ = eCurrentAnimation::Wait;
		}

	}
	else
	{
		Move(targetPosition);
		moveProcess(_deltaTime);
	}

}

void Character::startMixItem()
{

}

void Character::updateMixItem(float _deltaTime)
{
	if (queueItemMixing_.empty())
	{
		normalState_ << "Watch";
		return;
	}

	QueueItem item = queueItemMixing_.front();
	ItemType itemType = item.CI_.left_->GetItemType();

	if (itemType == ItemType::USEABLE)
	{
		normalState_ << "Cook";
	}
	else
	{
		normalState_ << "Craft";
	}
}

void Character::startCraft()
{
	changeAnimationCraft();
	uiController_->GetSkillGauge()->PlayAwake(2.0f, "제작 중");

	FT::PlaySoundAndSendPacket("chracter_Craft_Tool.wav", transform_.GetWorldPosition());
}

void Character::updateCraft(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{

		mixingItem();
		normalState_ << "Watch";
		return;
	}

	if (normalState_.GetTime() > 2.0f)
	{
		mixingItem();
		normalState_ << "Watch";
		return;
	}
}

void Character::startCook()
{
	changeAnimationCook();
	FT::PlaySoundAndSendPacket("chracter_Craft_Food.wav", transform_.GetWorldPosition());
}

void Character::updateCook(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		mixingItem();
		normalState_ << "Watch";
		return;
	}

	if (normalState_.GetTime() > 6.0f)
	{
		mixingItem();
		normalState_ << "Watch";
		return;
	}
}

void Character::startHyperloopBegin()
{
	changeAnimationOperate();
	FT::PlaySoundAndSendPacket("hyperLoopWorking.wav", transform_.GetWorldPosition());
}

void Character::updateHyperloopBegin(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		transform_.SetWorldPosition(hyperloopDest_);
		destination_ = hyperloopDest_;
		FT::PlaySoundAndSendPacket("hyperloop_arrive.wav", transform_.GetWorldPosition());
		curLocation_ = hyperLocation_;
		hyperLocation_ = Location::NONE;
		normalState_ << "HyperloopEnd";
	}
}

void Character::startHyperloopEnd()
{
	changeAnimationArrive();
}

void Character::updateHyperloopEnd(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		normalState_ << "Watch";
	}
}

void Character::startRestBegin()
{
	uiController_->GetSkillGauge()->PlayAwake(0.5f, "시전 중");
	changeAnimationRestStart();
}

void Character::updateRestBegin(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		normalState_ << "RestLoop";
	}
}

void Character::startRestLoop()
{
	changeAnimationRestLoop();
}

void Character::updateRestLoop(float _deltaTime)
{
	if (GameEngineInput::Up("X"))
	{
		normalState_ << "RestEnd";
	}

	if (normalState_.GetTime() > 3.0f)
	{
		normalState_.GetCurrentState()->Time_ = 0.0f;
		stat_.HP += stat_.HPMax * 0.1f;
		stat_.SP += stat_.SPMax * 0.1f;
		if (stat_.HP > stat_.HPMax)
		{
			stat_.HP = stat_.HPMax;
		}

		if (stat_.SP > stat_.SPMax)
		{
			stat_.SP = stat_.SPMax;
		}

		CharStatPacket packet;
		packet.SetTargetIndex(GetIndex());
		packet.SetStat(stat_);
		FT::SendPacket(packet);
	}
}

void Character::startRestEnd()
{
	changeAnimationRestEnd();
}

void Character::updateRestEnd(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		normalState_ << "Watch";
	}
}

void Character::startGather()
{
	uiController_->GetSkillGauge()->PlayAwake(2.0f, "채집 중");
	changeAnimationGather();
}

void Character::updateGather(float _deltaTime)
{
	if (normalState_.GetTime() > 2.0f)
	{
		getItem(0);
		normalState_ << "Watch";
	}
}

void Character::startStun()
{
	changeAnimationWait();

	// 이 부분에서
	// "니 컴퓨터의 내 인덱스 더미 캐릭터에게 스턴 이팩트를 띄워 줘
	// 들어가야 한다.

	CharEffectPacket pack;
	pack.SetTargetIndex(myIndex_);
	pack.SetAnimationName("StunEffect");

	FT::SendPacket(pack);

}

void Character::updateStun(float _deltaTime)
{
	float4 wp = GetTransform()->GetWorldPosition();
	stunEffect_->GetTransform()->SetLocalPosition(wp);

	timerStun_ -= _deltaTime;
	if (timerStun_ <= 0.0f)
	{
		destination_ = transform_.GetWorldPosition();
		destinations_.clear();
		mainState_ << "NormalState";
		normalState_ << "Watch";
		return;
	}
}

void Character::startKnockback()
{
	changeAnimationWait();
}

void Character::updateKnockback(float _deltaTime)
{
	timerKnockback_ -= _deltaTime;
	if (timerKnockback_ <= 0.0f)
	{
		destination_ = transform_.GetWorldPosition();
		destinations_.clear();
		mainState_ << "NormalState";
		normalState_ << "Watch";
		return;
	}

	transform_.AddWorldPosition(knockbackSpeed_ * _deltaTime);
}

void Character::startWallSlam()
{
	changeAnimationWait();
}

void Character::updateWallSlam(float _deltaTime)
{
	timerKnockback_ -= _deltaTime;
	if (timerKnockback_ <= 0.0f)
	{
		destination_ = transform_.GetWorldPosition();
		destinations_.clear();
		mainState_ << "NormalState";
		normalState_ << "Watch";
		return;
	}

	float4 moveSpeed = knockbackSpeed_ * _deltaTime;
	float4 nextMovePosition = transform_.GetWorldPosition() + moveSpeed;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		Stun(timerStun_);
		GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill03_Hit.wav");
		PacketSoundPlay packet;
		packet.SetSound("hyunwoo_Skill03_Hit.wav", transform_.GetWorldPosition());

		FT::SendPacket(packet);
	}
}

void Character::startBasicAttack()
{
	attackTime_ = 0.0f;
	ResetInfight();
	changeAnimationBasicAttack();
	FBXAnimation* curAni = renderer_->GetCurrentAnimation();
	if (nullptr != curAni)
	{
		curAni->FrameTime = 0.03333f / stat_.AttackSpeedRatio;
	}

	currentAnimation_ = eCurrentAnimation::BasicAttack;
}

void Character::updateBasicAttack(float _deltaTime)
{
	attackTime_ += _deltaTime;

	if (attackTime_ < stat_.AttackStartTime / stat_.AttackSpeed)
	{
		GameEngineCollision* rayCol = mouse_->GetRayCollision();

		bool result = false;
		float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
		if (GameEngineInput::Press("RButton") || GameEngineInput::Down("RButton"))
		{

			IUnit* otherCharacter = getMousePickedCharacter();

			if (target_ != otherCharacter)
			{
				inputProcess(_deltaTime);

				float4 worldPosition = GetTransform()->GetWorldPosition();
				worldPosition.y = destination_.y;
				if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
				{
					normalState_ << "Run";
				}

				mainState_ << "NormalState";
				return;
			}
		}
	}

	if (attackTime_ > stat_.AttackStartTime / stat_.AttackSpeed)
	{
		attackState_ << "BasicAttacking";
		return;
	}
}

void Character::startBasicAttacking()
{
	ResetInfight();
	onStartBasicAttacking(target_);
}

void Character::updateBasicAttacking(float _deltaTime)
{
	attackTime_ += _deltaTime;
	if (attackTime_ > stat_.AttackEndTime / stat_.AttackSpeed)
	{
		attackCooldown_ = 0.0f;
		mainState_ << "NormalState";
		return;
	}

	onUpdateBasicAttacking(target_, _deltaTime);
}

void Character::startQSkill()
{
	ResetInfight();
	onStartQSkill();
}

void Character::updateQSkill(float _deltaTime)
{
	onUpdateQSkill(_deltaTime);
}

void Character::startWSkill()
{
	ResetInfight();
	onStartWSkill();
}

void Character::updateWSkill(float _deltaTime)
{
	onUpdateWSkill(_deltaTime);
}

void Character::startESkill()
{
	ResetInfight();
	onStartESkill();
}

void Character::updateESkill(float _deltaTime)
{
	onUpdateESkill(_deltaTime);
}

void Character::startRSkill()
{
	ResetInfight();
	onStartRSkill();
}

void Character::updateRSkill(float _deltaTime)
{
	onUpdateRSkill(_deltaTime);
}

void Character::startDSkill()
{
	ResetInfight();
	onStartDSkill();
}

void Character::updateDSkill(float _deltaTime)
{
	onUpdateDSkill(_deltaTime);
}

void Character::startPlayerAlive()
{
	//  MainState::Death 의 아무 것도 안 하는 서브 스테이트입니다.
	// 이후 죽을 때만 PlayerDeathState 로 체인지 됩니다.
}

void Character::updatePlayerAlive(float _deltaTime)
{
	return;
}

void Character::startPlayerDeath()
{
	LumiaLevel* level = GetLevelConvert<LumiaLevel>();
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	stat_.HP = 0.0f;
	collision_->Off();

	level->SubtractSurvivorCount();
	// 총 플레이어 중 몇 위인가?
	int myRank = static_cast<int>(pm->GetPlayerList().size());

	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		if (i == myIndex_)
		{
			continue;
		}

		if (pm->GetPlayerList()[i].stat_->HP <= 0.0f)
		{
			myRank--;
		}
	}

	if (bFocused_)
	{
		uiController_->UIOff();
		uiController_->GetWinLoseUI()->SetPortrait(GetJobType(), false);



		if (fraggerIndex_ == -1)
		{
			uiController_->GetWinLoseUI()->SetText("플레이어가 자살했습니다.\n" + std::to_string(myRank) + " / " + std::to_string(pm->GetPlayerList().size()));
		}
		else if (-2 == fraggerIndex_)
		{
			uiController_->GetWinLoseUI()->SetText("플레이어가 몬스터에게 살해당했습니다.\n" + std::to_string(myRank) + " / " + std::to_string(pm->GetPlayerList().size()));
		}
		else if (-3 == fraggerIndex_)
		{
			uiController_->GetWinLoseUI()->SetText("금지구역에서 벗어나지 못했습니다.\n" + std::to_string(myRank) + " / " + std::to_string(pm->GetPlayerList().size()));
		}
		else
		{
			uiController_->GetWinLoseUI()->SetText(pm->GetPlayerList()[fraggerIndex_].playerNickname_ + " 에게 살해당했습니다.\n" + std::to_string(myRank) + " / " + std::to_string(pm->GetPlayerList().size()));
		}

		uiController_->GetWinLoseUI()->Activate();
	}


	onStartDeath();
}

void Character::updatePlayerDeath(float _deltaTime)
{

	onUpdateDeath(_deltaTime);
}

void Character::startPlayerWinner()
{
	isPlayerWon_ = true;
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	LumiaLevel* level = GetLevelConvert<LumiaLevel>();

	int myRank = static_cast<int>(pm->GetPlayerList().size());

	uiController_->UIOff();
	uiController_->GetWinLoseUI()->SetWinner();
	uiController_->GetWinLoseUI()->SetPortrait(GetJobType(), true);
	uiController_->GetWinLoseUI()->SetText("승리자 : " + pm->GetMyPlayer().playerNickname_ + "\n마지막까지 생존하였습니다.");
	uiController_->GetWinLoseUI()->Activate();

	changeAnimationWait();
}

void Character::updatePlayerWinner(float _deltaTime)
{
}

void Character::PlayEffect(const std::string& _effectName, int _originIndex, IUnit* _victim, float4 _pos, float _damage)
{
	//if (myIndex_ == _originIndex)
	//{
	//	return;
	//}

	if ("StunEffect" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		stunEffect_->GetTransform()->SetLocalPosition(wp);
		stunEffect_->PlayAwake(1.0f);
		return;
	}

	if ("Death" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		deathEffect_->GetTransform()->SetWorldPosition(wp);
		deathEffect_->PlayAwake(false);
		return;
	}

	if ("Death_Explode" == _effectName)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		deathEffect_->GetTransform()->SetWorldPosition(wp);
		deathEffect_->PlayAwake(true);
		return;
	}

	if ("DamagePopup" == _effectName)
	{
		int DMG = static_cast<int>(_damage);
		damagePopUp_->DamageFontAppear(GetTransform()->GetLocalPosition(), to_string(DMG));
	}

	onPlayEffect(_effectName, _victim, _pos);
}

void Character::RandomSoundPlay(const std::string& _sound0, const std::string& _sound1, const std::string& _sound2)
{
	int random = randomGenerator.RandomInt(0, 3);

	switch (random)
	{
	case 0:
		if ("" == _sound0)
		{
			break;
		}
		FT::PlaySoundAndSendPacket(_sound0, transform_.GetWorldPosition());
		break;
	case 1:
		if ("" == _sound1)
		{
			break;
		}
		FT::PlaySoundAndSendPacket(_sound1, transform_.GetWorldPosition());
		break;
	case 2:
		if ("" == _sound2)
		{
			break;
		}
		FT::PlaySoundAndSendPacket(_sound2, transform_.GetWorldPosition());
		break;
	case 3:
		break;
	default:
		break;
	}


}

void Character::InfightCheck(float _DeltaTime)
{
	infightTimer_ += _DeltaTime;

	if (5.0f <= infightTimer_)
	{
		// 비전투 상태 전환
		infightTimer_ = 0.0f;
		isInfight_ = false;
		return;
	}

}

void Character::ResetInfight()
{
	infightTimer_ = 0.0f;
	isInfight_ = true;
}

void Character::DebuffCheck(float _DeltaTime)
{
	SlowCheck(_DeltaTime);
}

void Character::SlowCheck(float _DeltaTIme)
{
	// 슬로우 같은건 CC로 퉁칠수가 없습니다.
	// (걸린 와중에도 이동속도만 느려지지 모든 행동을 할 수 있기 때문)

	// 따라서 업데이트 중에 플래그와 슬로우 지속시간을 체크하며, 주어진 슬로우 비율만큼 이동속도를 늦춥니다.
	int a = 0;

	if (true == bSlowFlag_)
	{
		float4 wp = GetTransform()->GetWorldPosition();
		slowEffect_->GetTransform()->SetLocalPosition(wp);
		stat_.MovementRatio = (1.0f - slowRatio_);

		if (slowTimer_ <= 0.0f)
		{
			slowTimer_ = 0.0f;
			stat_.MovementRatio = 1.0f;
			bSlowFlag_ = false;
			return;
		}

		slowTimer_ -= _DeltaTIme;

	}

}


void Character::CoolTimeCheck(float _DeltaTime)
{
	if (true == bCoolQ_)
	{
		coolTimer_Q_ -= _DeltaTime;
		int tmp = static_cast<int>(coolTimer_Q_) + 1;
		uiController_->GetSkillUI()->GetCoolTimeMaskQ()->TextSetting("굴림", to_string(tmp), 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });

		if (coolTimer_Q_ <= 0.0f) // 쿨이 다 되었다
		{
			// 초기화
			uiController_->GetSkillUI()->GetCoolTimeMaskQ()->TextSetting("굴림", " ", 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });
			// 쿨타임 마스크 하얀색으로 교체 -> 알파값 급격히 낮아져서 투명화 -> OFF() -> 다시 블루 마스크로 이미지 바꿈 -> 알파값 1.0f

			coolTimer_Q_ = DefaultCool_Q_;
			bCoolQ_ = false;
			bCoolWQ_ = true;
		}

	}

	if (false == bCoolQ_ && true == bCoolWQ_)
	{
		uiController_->GetSkillUI()->GetCoolTimeMaskQ()->SetImage("cooltime_whitemask.png", "LinerSmp");
		uiController_->GetSkillUI()->GetCoolTimeMaskQ()->SetAlpha(coolTimerW_Q_ / 0.5f);

		coolTimerW_Q_ -= _DeltaTime;

		if (0.0f >= coolTimerW_Q_)
		{
			coolTimerW_Q_ = 0.5f;
			uiController_->GetSkillUI()->GetCoolTimeMaskQ()->Off();
			uiController_->GetSkillUI()->GetCoolTimeMaskQ()->SetImage("cooltime_mask.png", "LinerSmp");
			uiController_->GetSkillUI()->GetCoolTimeMaskQ()->SetAlpha(1.0f);
			bCoolWQ_ = false;
		}
	}



	if (true == bCoolW_)
	{
		coolTimer_W_ -= _DeltaTime;
		int tmp = static_cast<int>(coolTimer_W_) + 1;
		uiController_->GetSkillUI()->GetCoolTimeMaskW()->TextSetting("굴림", to_string(tmp), 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });

		if (coolTimer_W_ <= 0.0f) // 쿨이 다 되었다
		{
			// 초기화
			uiController_->GetSkillUI()->GetCoolTimeMaskW()->TextSetting("굴림", " ", 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });

			coolTimer_W_ = DefaultCool_W_;

			bCoolW_ = false;
			bCoolWW_ = true;
		}
	}

	if (false == bCoolW_ && true == bCoolWW_)
	{
		uiController_->GetSkillUI()->GetCoolTimeMaskW()->SetImage("cooltime_whitemask.png", "LinerSmp");
		uiController_->GetSkillUI()->GetCoolTimeMaskW()->SetAlpha(coolTimerW_W_ / 0.5f);

		coolTimerW_W_ -= _DeltaTime;

		if (0.0f >= coolTimerW_W_)
		{
			coolTimerW_W_ = 0.5f;
			uiController_->GetSkillUI()->GetCoolTimeMaskW()->Off();
			uiController_->GetSkillUI()->GetCoolTimeMaskW()->SetImage("cooltime_mask.png", "LinerSmp");
			uiController_->GetSkillUI()->GetCoolTimeMaskW()->SetAlpha(1.0f);
			bCoolWW_ = false;
		}
	}

	if (true == bCoolE_)
	{
		coolTimer_E_ -= _DeltaTime;
		int tmp = static_cast<int>(coolTimer_E_) + 1;
		uiController_->GetSkillUI()->GetCoolTimeMaskE()->TextSetting("굴림", to_string(tmp), 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });

		if (coolTimer_E_ <= 0.0f) // 쿨이 다 되었다
		{
			// 초기화
			uiController_->GetSkillUI()->GetCoolTimeMaskE()->TextSetting("굴림", " ", 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });

			coolTimer_E_ = DefaultCool_E_;
			bCoolE_ = false;
			bCoolWE_ = true;
		}
	}

	if (false == bCoolE_ && true == bCoolWE_)
	{
		uiController_->GetSkillUI()->GetCoolTimeMaskE()->SetImage("cooltime_whitemask.png", "LinerSmp");
		uiController_->GetSkillUI()->GetCoolTimeMaskE()->SetAlpha(coolTimerW_E_ / 0.5f);

		coolTimerW_E_ -= _DeltaTime;

		if (0.0f >= coolTimerW_E_)
		{
			coolTimerW_E_ = 0.5f;
			uiController_->GetSkillUI()->GetCoolTimeMaskE()->Off();
			uiController_->GetSkillUI()->GetCoolTimeMaskE()->SetImage("cooltime_mask.png", "LinerSmp");
			uiController_->GetSkillUI()->GetCoolTimeMaskE()->SetAlpha(1.0f);
			bCoolWE_ = false;
		}
	}

	if (true == bCoolR_)
	{
		coolTimer_R_ -= _DeltaTime;
		int tmp = static_cast<int>(coolTimer_R_) + 1;
		uiController_->GetSkillUI()->GetCoolTimeMaskR()->TextSetting("굴림", to_string(tmp), 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });

		if (coolTimer_R_ <= 0.0f) // 쿨이 다 되었다
		{
			// 초기화
			uiController_->GetSkillUI()->GetCoolTimeMaskR()->TextSetting("굴림", " ", 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });

			coolTimer_R_ = DefaultCool_R_;
			bCoolR_ = false;
			bCoolWR_ = true;
		}
	}

	if (false == bCoolR_ && true == bCoolWR_)
	{
		uiController_->GetSkillUI()->GetCoolTimeMaskR()->SetImage("cooltime_whitemask.png", "LinerSmp");
		uiController_->GetSkillUI()->GetCoolTimeMaskR()->SetAlpha(coolTimerW_R_ / 0.5f);

		coolTimerW_R_ -= _DeltaTime;

		if (0.0f >= coolTimerW_R_)
		{
			coolTimerW_R_ = 0.5f;
			uiController_->GetSkillUI()->GetCoolTimeMaskR()->Off();
			uiController_->GetSkillUI()->GetCoolTimeMaskR()->SetImage("cooltime_mask.png", "LinerSmp");
			uiController_->GetSkillUI()->GetCoolTimeMaskR()->SetAlpha(1.0f);
			bCoolWR_ = false;
		}
	}

	if (true == bCoolD_)
	{
		coolTimer_D_ -= _DeltaTime;
		int tmp = static_cast<int>(coolTimer_D_) + 1;
		uiController_->GetSkillUI()->GetCoolTimeMaskD()->TextSetting("굴림", to_string(tmp), 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });

		if (coolTimer_D_ <= 0.0f) // 쿨이 다 되었다
		{
			// 초기화
			uiController_->GetSkillUI()->GetCoolTimeMaskD()->TextSetting("굴림", " ", 20, FW1_CENTER, float4::WHITE, { 0.0f, 12.0f });

			coolTimer_D_ = DefaultCool_D_;
			bCoolD_ = false;
			bCoolWD_ = true;
		}
	}

	if (false == bCoolD_ && true == bCoolWD_)
	{
		uiController_->GetSkillUI()->GetCoolTimeMaskD()->SetImage("cooltime_whitemask.png", "LinerSmp");
		uiController_->GetSkillUI()->GetCoolTimeMaskD()->SetAlpha(coolTimerW_D_ / 0.5f);

		coolTimerW_D_ -= _DeltaTime;

		if (0.0f >= coolTimerW_D_)
		{
			coolTimerW_D_ = 0.5f;
			uiController_->GetSkillUI()->GetCoolTimeMaskD()->Off();
			uiController_->GetSkillUI()->GetCoolTimeMaskD()->SetImage("cooltime_mask.png", "LinerSmp");
			uiController_->GetSkillUI()->GetCoolTimeMaskD()->SetAlpha(1.0f);
			bCoolWD_ = false;
		}
	}
}

void Character::EffectTransformCheck(float _DeltaTime)
{
	levelUpEffect_->GetTransform()->SetWorldPosition(transform_.GetWorldPosition());
	deathEffect_->GetTransform()->SetWorldPosition(transform_.GetWorldPosition());
	onEffectTransformCheck(_DeltaTime);
}

void Character::ProhibitedAreaCheck(float _DeltaTime)
{
	if (true == isPlayerDead_ || true == isPlayerWon_)
	{
		return;
	}

	LumiaLevel* level = GetLevelConvert<LumiaLevel>();
	if (nullptr == level)
	{
		return;
	}

	if (true == level->GetProhibitedAreaList()[static_cast<int>(curLocation_)]->IsProhibited())
	{
		prohibitedCounter_ -= _DeltaTime;
		int adjustedCounter = static_cast<int>(prohibitedCounter_) + 1;

		uiController_->GetTimeUI()->GetProhibitedRenderer()->SetTextColor(float4::RED);
		uiController_->GetTimeUI()->GetProhibitedRenderer()->AllDelText();
		uiController_->GetTimeUI()->GetProhibitedRenderer()->SetPrintText(std::to_string(adjustedCounter));

		prohibitTimer_ += _DeltaTime;

		if (1.0f <= prohibitTimer_)
		{
			prohibitTimer_ = 0.0f;
			FT::PlaySoundAndSendPacket("RestrictedCount.wav", transform_.GetWorldPosition());
		}

		if (0.0f >= prohibitedCounter_)
		{
			//
			fraggerIndex_ = -3;

			Damage(10000.0f, nullptr);
		}
	}
}
