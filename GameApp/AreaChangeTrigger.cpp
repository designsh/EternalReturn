#include "PreCompile.h"
#include "AreaChangeTrigger.h"
#include <GameEngine/GameEngineCollision.h>
#include "Character.h"
#include "PlayerUIController.h"
AreaChangeTrigger::AreaChangeTrigger() // default constructer 디폴트 생성자
	: entryPoint0_(Location::NONE), entryPoint1_(Location::NONE), entryCollision0_(nullptr), bIsIn_(false), entryCollision1_(nullptr), bIsIn2_(false)
{

}

AreaChangeTrigger::~AreaChangeTrigger() // default destructer 디폴트 소멸자
{

}

AreaChangeTrigger::AreaChangeTrigger(AreaChangeTrigger&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void AreaChangeTrigger::SetLocations(Location _loca0, const std::string& _loca0Name, Location _loca1, const std::string& _loca1Name)
{
	entryPoint0_ = _loca0;
	entryPoint1_ = _loca1;

	entryPointName0_ = _loca0Name;
	entryPointName1_ = _loca1Name;
}

void AreaChangeTrigger::SetCollision(float4 _entry0Pos, float4 _entryScale, float4 _entry1Pos)
{
	entryCollision0_->GetTransform()->SetWorldPosition(_entry0Pos);
	entryCollision0_->GetTransform()->SetLocalScaling(_entryScale);

	entryCollision1_->GetTransform()->SetWorldPosition(_entry1Pos);
	entryCollision1_->GetTransform()->SetLocalScaling(_entryScale);
}


void AreaChangeTrigger::Start()
{
	entryCollision0_ = CreateTransformComponent<GameEngineCollision>();
	entryCollision0_->SetCollisionGroup(eCollisionGroup::Trigger);
	entryCollision0_->SetCollisionType(CollisionType::AABBBox3D);

	entryCollision1_ = CreateTransformComponent<GameEngineCollision>();
	entryCollision1_->SetCollisionGroup(eCollisionGroup::Trigger);
	entryCollision1_->SetCollisionType(CollisionType::AABBBox3D);
}

void AreaChangeTrigger::Update(float _DeltaTime)
{
	auto colList = entryCollision0_->GetCollisionList(eCollisionGroup::Player);
	GetLevel()->PushDebugRender(entryCollision0_->GetTransform(), CollisionType::AABBBox3D, float4::RED);

	auto colList1 = entryCollision1_->GetCollisionList(eCollisionGroup::Player);
	GetLevel()->PushDebugRender(entryCollision1_->GetTransform(), CollisionType::AABBBox3D, float4::BLUE);

//if (true == bIsIn_ && false == entryCollision0_->Collision(eCollisionGroup::Player))
//{
//	bIsIn_ = false;
//	return;
//}

	for (GameEngineCollision* col : colList)
	{
		GameEngineActor* actor = col->GetActor();
		Character* character = nullptr;

		if (/*false == bIsIn_ && */nullptr != actor)
		{
			character = dynamic_cast<Character*>(actor);

			if (nullptr != character && true == character->IsFocused())
			{
				if (entryPoint0_ == static_cast<Location>(character->GetCurLocation()))
				{
					character->SetCurLocation(entryPoint0_);
				}
				else if (entryPoint1_ == static_cast<Location>(character->GetCurLocation()))
				{
					// 엔트리포인트 1 에 있다가 0으로 넘어간다는 코드블록
					character->SetCurLocation(entryPoint0_);

					if (false == bIsIn_)
					{
						character->GetUIController()->GetNoticeUI()->SetText(entryPointName0_, 3.f);
						bIsIn_ = true;
						bIsIn2_ = false;
					}
					
				}
				//else
				//{
				//	character->GetStat()->curLocation = static_cast<int>(entryPoint0_);
				//	character->GetUIController()->GetNoticeUI()->SetText(entryPointName0_, 3.f);
				//}

				
			}
		}
	}

	//if (true == bIsIn2_ && false == entryCollision1_->Collision(eCollisionGroup::Player))
	//{
	//	bIsIn2_ = false;
	//	return;
	//}

	for (GameEngineCollision* col : colList1)
	{
		GameEngineActor* actor = col->GetActor();
		Character* character = nullptr;

		if (/*false == bIsIn2_ && */nullptr != actor)
		{
			character = dynamic_cast<Character*>(actor);

			if (nullptr != character && true == character->IsFocused())
			{
				if (entryPoint0_ == static_cast<Location>(character->GetCurLocation()))
				{
					// 엔트리포인트 0 에 있다가 1로 넘어간다는 코드블록

					character->SetCurLocation(entryPoint1_);

					if (false == bIsIn2_)
					{
						character->GetUIController()->GetNoticeUI()->SetText(entryPointName1_, 3.f);
						bIsIn2_ = true;
						bIsIn_ = false;
					}
				}
				else if (entryPoint1_ == static_cast<Location>(character->GetCurLocation()))
				{
					// 엔트리포인트 1 에 있다가 0으로 넘어간다는 코드블록

					character->SetCurLocation(entryPoint1_);
				//	character->GetUIController()->GetNoticeUI()->SetText(entryPointName0_, 3.f);
				}
				//else
				//{
				//	GameEngineDebug::MsgBox("특정 AreaChangeTrigger 액터에 지정되지 않은 파라미터가 있습니다.");
				//}

			}
		}
	}
}

