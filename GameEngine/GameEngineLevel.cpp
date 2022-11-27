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
	//GameEngineDebug::OutPutDebugString("������ ���� : " + std::to_string(time.GetDeltaTime()) + "��\n");

	// ������ ����
	// -> ������� ����Ÿ���� Ŭ���ϰ�, ����۸� ����(�������غ�)
	GameEngineDevice::RenderStart();

	// ����ī�޶� & UIī�޶� ����Ÿ�� Ŭ����
	MainCameraActor_->GetCamera()->ClearCameraTarget();
	UICameraActor_->GetCamera()->ClearCameraTarget();

	// ����ī�޶� ������
	MainCameraActor_->GetCamera()->Render(_DeltaTime);
	MainCameraActor_->GetCamera()->DebugRender();

	// UIī�޶� ������
	UICameraActor_->GetCamera()->Render(_DeltaTime);
	UICameraActor_->GetCamera()->DebugRender();

	// �����Ÿ�ٿ� ī�޶���� ����Ÿ�� ������ ó���ؾ��ϴ� �������� ����
	{
		std::vector<GameEnginePostProcessRender*>& PostCameraMergePrev = PostRender["CameraMergePrev"];
		for (size_t i = 0; i < PostCameraMergePrev.size(); i++)
		{
			PostCameraMergePrev[i]->Effect(_DeltaTime);
		}
	}

	// ����ī�޶� & UIī�޶� ����Ÿ�� ���� ����� Ÿ�ٿ� ����
	GameEngineDevice::GetBackBufferTarget()->Merge(MainCameraActor_->GetCamera()->GetCameraRenderTarget());
	GameEngineDevice::GetBackBufferTarget()->Merge(UICameraActor_->GetCamera()->GetCameraRenderTarget());

	// ��ó�� ����Ÿ�� ����
	{
		std::vector<GameEnginePostProcessRender*>& PostCameraMergeNext = PostRender["CameraMergeNext"];
		for (size_t i = 0; i < PostCameraMergeNext.size(); i++)
		{
			PostCameraMergeNext[i]->Effect(_DeltaTime);
		}
	}

	// IMGUI Window ������ ó��
	GameEngineGUI::GetInst()->GUIRenderStart();
	GameEngineGUI::GetInst()->GUIRenderEnd();

	// ������ ����
	// -> ����۰� ����ִ� Ÿ�ٿ� ������ ���
	GameEngineDevice::RenderEnd();

	//time.TimeCheck();
	//GameEngineDebug::OutPutDebugString("������ �� : " + std::to_string(time.GetDeltaTime()) + "��\n");
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

	// �浹ü ����
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

	// ���� ����
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

	//=================== �׸��ڰ���
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

		// ���� ī�޶󿡼� ��� ���͸� ������ ������ ������ �߰� ��
		MainCameraActor_->GetCamera()->NextLevelMoveRenderer(_NextLevel->GetMainCamera(), _Actor);
		UICameraActor_->GetCamera()->NextLevelMoveRenderer(_NextLevel->GetUICamera(), _Actor);

		// ���� ī�޶󿡼� ������ ������ �ѱ� ���͸� ����

		// �ݸ��� ����
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
	// �׸��� ����Ÿ�� ����� ����
	if (nullptr != LightShadowTexture_)
	{
		delete LightShadowTexture_;
		LightShadowTexture_ = nullptr;
	}

	// �׸��ڷ���Ÿ���� ���ҽ� ����
	for (int i = 0; i < static_cast<int>(LightShadowRenderTargets_.size()); ++i)
	{
		LightShadowRenderTargets_[i]->Release();
		LightShadowRenderTargets_[i] = nullptr;
	}
	LightShadowRenderTargets_.clear();

	// ����/���ٽǹ��� ���ҽ� ����
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
