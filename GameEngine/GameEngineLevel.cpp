#include "PreCompile.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineRenderer.h"
#include "GameEngineDevice.h"
#include "GameEngineTransform.h"
#include "CameraActor.h"
#include "CameraComponent.h"
#include "GameEngineCollision.h"
#include "GameEngineDebugRenderData.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineFontManager.h"
#include "GameEngineFont.h"
#include "GameEngineUIRenderer.h"
#include "GameEngineGUI.h"
#include "GameEnginePostProcessRender.h"
#include "GameEngineDepthBuffer.h"

CameraActor* GameEngineLevel::GetMainCameraActor()
{
	return MainCameraActor_;
}

CameraComponent* GameEngineLevel::GetMainCamera()
{
	return MainCameraActor_->GetCamera();
}

CameraActor* GameEngineLevel::GetUICameraActor()
{
	return UICameraActor_;
}

CameraComponent* GameEngineLevel::GetUICamera()
{
	return UICameraActor_->GetCamera();
}

GameEngineTexture* GameEngineLevel::GetShadowTexture()
{
	return LightShadowTexture_;
}

void GameEngineLevel::SetLevelActorMove(GameEngineLevel* _NextLevel, GameEngineActor* _Actor)
{
	NextLevelActorsData_.push_back({ _Actor , _NextLevel });
}

void GameEngineLevel::ActorUpdate(float _DeltaTime)
{
	TimeEventUpdate();

	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;

		float Scale = GameEngineTime::GetInst().GetTimeScale(Pair.first);

		for (GameEngineActor* Actor : Actors)
		{
			if (false == Actor->IsUpdate())
			{
				continue;
			}

			Actor->Update(_DeltaTime * Scale);
			Actor->UpdateComponent(_DeltaTime * Scale);
		}
	}
}

void GameEngineLevel::Render(float _DeltaTime)
{
	//GameEngineTime time;
	//time.TimeCheckReset();

	//time.TimeCheck();
	//GameEngineDebug::OutPutDebugString("렌더링 시작 : " + std::to_string(time.GetDeltaTime()) + "초\n");

	// 렌더링 시작
	// -> 백버퍼의 렌더타겟을 클리하고, 백버퍼를 셋팅(렌더링준비)
	GameEngineDevice::RenderStart();

	// 메인카메라 & UI카메라 렌더타겟 클리어
	MainCameraActor_->GetCamera()->ClearCameraTarget();
	UICameraActor_->GetCamera()->ClearCameraTarget();

	// 메인카메라 렌더링
	MainCameraActor_->GetCamera()->Render(_DeltaTime);
	MainCameraActor_->GetCamera()->DebugRender();

	// UI카메라 렌더링
	UICameraActor_->GetCamera()->Render(_DeltaTime);
	UICameraActor_->GetCamera()->DebugRender();

	// 백버퍼타겟에 카메라들의 렌더타겟 머지전 처리해야하는 렌더러들 머지
	{
		std::vector<GameEnginePostProcessRender*>& PostCameraMergePrev = PostRender["CameraMergePrev"];
		for (size_t i = 0; i < PostCameraMergePrev.size(); i++)
		{
			PostCameraMergePrev[i]->Effect(_DeltaTime);
		}
	}

	// 메인카메라 & UI카메라 렌더타겟 각각 백버퍼 타겟에 머지
	GameEngineDevice::GetBackBufferTarget()->Merge(MainCameraActor_->GetCamera()->GetCameraRenderTarget());
	GameEngineDevice::GetBackBufferTarget()->Merge(UICameraActor_->GetCamera()->GetCameraRenderTarget());

	// 후처리 렌더타겟 머지
	{
		std::vector<GameEnginePostProcessRender*>& PostCameraMergeNext = PostRender["CameraMergeNext"];
		for (size_t i = 0; i < PostCameraMergeNext.size(); i++)
		{
			PostCameraMergeNext[i]->Effect(_DeltaTime);
		}
	}

	// IMGUI Window 렌더링 처리
	GameEngineGUI::GetInst()->GUIRenderStart();
	GameEngineGUI::GetInst()->GUIRenderEnd();

	// 렌더링 종료
	// -> 백버퍼가 들고있는 타겟에 렌더링 명령
	GameEngineDevice::RenderEnd();

	//time.TimeCheck();
	//GameEngineDebug::OutPutDebugString("렌더링 끝 : " + std::to_string(time.GetDeltaTime()) + "초\n");
}

void GameEngineLevel::Release(float _DeltaTime)
{
	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;

		for (GameEngineActor* Actor : Actors)
		{
			Actor->ReleaseUpdate(_DeltaTime);
		}
	}

	MainCameraActor_->GetCamera()->ReleaseRenderer();
	UICameraActor_->GetCamera()->ReleaseRenderer();

	// 충돌체 삭제
	{
		std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapBeginIter = CollisionList_.begin();
		std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapEndIter = CollisionList_.end();
		for (; RenderMapBeginIter != RenderMapEndIter; ++RenderMapBeginIter)
		{
			std::list<GameEngineCollision*>& Collisions = RenderMapBeginIter->second;

			std::list<GameEngineCollision*>::iterator BeginIter = Collisions.begin();
			std::list<GameEngineCollision*>::iterator EndIter = Collisions.end();

			for (; BeginIter != EndIter; )
			{
				GameEngineCollision* ReleaseCollision = *BeginIter;

				if (nullptr == ReleaseCollision)
				{
					GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
				}

				if (true == ReleaseCollision->IsDeath())
				{
					BeginIter = Collisions.erase(BeginIter);

					continue;
				}

				++BeginIter;
			}
		}
	}

	// 액터 삭제
	{
		std::map<int, std::list<GameEngineActor*>>::iterator ActorMapBeginIter = ActorList_.begin();
		std::map<int, std::list<GameEngineActor*>>::iterator ActorMapEndIter = ActorList_.end();
		for (; ActorMapBeginIter != ActorMapEndIter; ++ActorMapBeginIter)
		{
			std::list<GameEngineActor*>& Actors = ActorMapBeginIter->second;

			std::list<GameEngineActor*>::iterator BeginIter = Actors.begin();
			std::list<GameEngineActor*>::iterator EndIter = Actors.end();

			for (; BeginIter != EndIter; )
			{
				GameEngineActor* ReleaseActor = *BeginIter;
				if (nullptr == ReleaseActor)
				{
					GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
					return;
				}

				ReleaseActor->ComponentRelease();

				if (true == ReleaseActor->IsDeath())
				{
					delete* BeginIter;
					*BeginIter = nullptr;

					BeginIter = Actors.erase(BeginIter);

					continue;
				}

				++BeginIter;
			}
		}
	}
}

void GameEngineLevel::ClearAll()
{
	for (auto& Event : AllEvent_)
	{
		delete Event;
	}
	AddEvent_.clear();

	for (auto& Effects : PostRender)
	{
		for (auto& Effect : Effects.second)
		{
			delete Effect;
		}
	}
	PostRender.clear();

	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;
		for (GameEngineActor* Actor : Actors)
		{
			if (nullptr != Actor)
			{
				delete Actor;
				Actor = nullptr;
			}
		}
		Actors.clear();
	}
	ActorList_.clear();
	CollisionList_.clear();

	//=================== 그림자관련
	ReleaseLightShadowTarget();
}

void GameEngineLevel::PushCollision(GameEngineCollision* _Collision, int _Group)
{
	CollisionList_[_Group].push_back(_Collision);
}

void GameEngineLevel::PushDebugRender(GameEngineTransform* _Transform, CollisionType _Type, float4 _Color)
{
	MainCameraActor_->GetCamera()->PushDebugRender(_Transform, _Type, _Color);
}

void GameEngineLevel::PushDebugRenderUI(GameEngineTransform* _Transform, CollisionType _Type, float4 _Color)
{
	UICameraActor_->GetCamera()->PushDebugRender(_Transform, _Type, _Color);
}

void GameEngineLevel::AddTimeEvent(float _Time, std::function<void()> _Event)
{
	AddEvent_.push_back(new TimeEvent{ _Time, _Event });
}

void GameEngineLevel::ChangeCollisionGroup(int _Group, GameEngineCollision* _Collision)
{
	CollisionList_[_Collision->GetOrder()].remove(_Collision);

	_Collision->SetOrder(_Group);

	CollisionList_[_Collision->GetOrder()].push_back(_Collision);
}

void GameEngineLevel::Init()
{
	MainCameraActor_ = CreateActor<CameraActor>();
	UICameraActor_ = CreateActor<CameraActor>();

	UICameraActor_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	UICameraActor_->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));
}

void GameEngineLevel::SetLevelActorMoveProcess()
{
	for (size_t i = 0; i < NextLevelActorsData_.size(); i++)
	{
		GameEngineLevel* _NextLevel = NextLevelActorsData_[i].Level;
		GameEngineActor* _Actor = NextLevelActorsData_[i].Actor;

		// 현재 카메라에서 어떠한 액터를 빼내어 지정된 레벨에 추가 후
		MainCameraActor_->GetCamera()->NextLevelMoveRenderer(_NextLevel->GetMainCamera(), _Actor);
		UICameraActor_->GetCamera()->NextLevelMoveRenderer(_NextLevel->GetUICamera(), _Actor);

		// 현재 카메라에서 지정된 레벨로 넘긴 액터를 제거

		// 콜리전 삭제
		{
			std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapBeginIter = CollisionList_.begin();
			std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapEndIter = CollisionList_.end();


			for (; RenderMapBeginIter != RenderMapEndIter; ++RenderMapBeginIter)
			{
				std::list<GameEngineCollision*>& Collisions = RenderMapBeginIter->second;

				std::list<GameEngineCollision*>::iterator BeginIter = Collisions.begin();
				std::list<GameEngineCollision*>::iterator EndIter = Collisions.end();

				for (; BeginIter != EndIter; )
				{
					GameEngineCollision* ReleaseCollision = *BeginIter;

					if (nullptr == ReleaseCollision)
					{
						GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
					}

					if (ReleaseCollision->GetActor() == _Actor)
					{
						_NextLevel->CollisionList_[(*BeginIter)->GetOrder()].push_back(*BeginIter);

						BeginIter = Collisions.erase(BeginIter);

						continue;
					}

					++BeginIter;

				}
			}
		}

		{
			std::map<int, std::list<GameEngineActor*>>::iterator ActorMapBeginIter = ActorList_.begin();
			std::map<int, std::list<GameEngineActor*>>::iterator ActorMapEndIter = ActorList_.end();
			for (; ActorMapBeginIter != ActorMapEndIter; ++ActorMapBeginIter)
			{
				std::list<GameEngineActor*>& Actors = ActorMapBeginIter->second;

				std::list<GameEngineActor*>::iterator BeginIter = Actors.begin();
				std::list<GameEngineActor*>::iterator EndIter = Actors.end();
				for (; BeginIter != EndIter; )
				{
					GameEngineActor* ReleaseActor = *BeginIter;

					if (nullptr == ReleaseActor)
					{
						GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
					}

					if (ReleaseActor == _Actor)
					{
						_NextLevel->ActorList_[ReleaseActor->GetOrder()].push_back(ReleaseActor);

						ReleaseActor->SetLevel(_NextLevel);

						BeginIter = Actors.erase(BeginIter);

						continue;
					}

					++BeginIter;
				}

			}
		}
	}

	NextLevelActorsData_.clear();
}

void GameEngineLevel::LevelChangeEndActorEvent(GameEngineLevel* _NextLevel)
{
	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;
		for (GameEngineActor* Actor : Actors)
		{
			Actor->LevelChangeEndEvent(_NextLevel);
		}
	}
}

void GameEngineLevel::LevelChangeStartActorEvent(GameEngineLevel* _PrevLevel)
{
	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;

		for (GameEngineActor* Actor : Actors)
		{
			Actor->LevelChangeStartEvent(_PrevLevel);
		}
	}
}

void GameEngineLevel::TimeEventUpdate()
{
	for (auto& Event : AddEvent_)
	{
		AllEvent_.push_back(Event);
	}
	AddEvent_.clear();

	for (auto& Event : AllEvent_)
	{
		Event->Time_ -= GameEngineTime::GetInst().GetDeltaTime();
		if (0 >= Event->Time_)
		{
			Event->Event_();
		}
	}

	std::list<TimeEvent*>::iterator StartIter = AllEvent_.begin();
	std::list<TimeEvent*>::iterator EndIter = AllEvent_.end();
	for (; StartIter != EndIter; )
	{
		if (0 >= (*StartIter)->Time_)
		{
			delete* StartIter;
			StartIter = AllEvent_.erase(StartIter);
			continue;
		}
		++StartIter;
	}
}

void GameEngineLevel::ReleaseLightShadowTarget()
{
	// 그림자 렌더타겟 존재시 삭제
	if (nullptr != LightShadowTexture_)
	{
		delete LightShadowTexture_;
		LightShadowTexture_ = nullptr;
	}

	// 그림자렌더타겟의 리소스 삭제
	for (int i = 0; i < static_cast<int>(LightShadowRenderTargets_.size()); ++i)
	{
		LightShadowRenderTargets_[i]->Release();
		LightShadowRenderTargets_[i] = nullptr;
	}
	LightShadowRenderTargets_.clear();

	// 깊이/스텐실버퍼 리소스 삭제
	for (int i = 0; i < static_cast<int>(LightShadowDepths_.size()); ++i)
	{
		delete LightShadowDepths_[i];
		LightShadowDepths_[i] = nullptr;
	}
	LightShadowDepths_.clear();
}

GameEngineLevel::GameEngineLevel()
	: MainCameraActor_(nullptr)
	, UICameraActor_(nullptr)
	, LightShadowTexture_(nullptr)
{
	PostRender["CameraMergePrev"];
	PostRender["CameraMergeNext"];
}

GameEngineLevel::~GameEngineLevel()
{
	ClearAll();
}
