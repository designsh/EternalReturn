#include "Precompile.h"
#include "LoadingLevel_LoadPercent.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineProgressBarRenderer.h>

#include "Enums.h"
#include "PlayerInfoManager.h"
#include "UserGame.h"
#include "GameServer.h"
#include "GameClient.h"

LoadingLevel_LoadPercent* LoadingLevel_LoadPercent::Percent = nullptr;

void LoadingLevel_LoadPercent::SetConnectPlayerCount()
{
	PlayerInfoManager* InfoManager = PlayerInfoManager::GetInstance();
	InGameConnectCount_ = static_cast<int>(InfoManager->GetPlayerList().size());
}

void LoadingLevel_LoadPercent::CheckLoadingPercent()
{
	// 해당 함수 호출마다 각 플레이어들의 로딩완료 Flag를 체크하여 ProgressBar 갱신
	PlayerInfoManager* InfoManager = PlayerInfoManager::GetInstance();
	int LoadingEndCount = 0;
	int TotalPlayerCnt = static_cast<int>(InfoManager->GetPlayerList().size());
	for (int i = 0; i < TotalPlayerCnt; ++i)
	{
		if (1 == InfoManager->GetPlayerList()[i].IsLoading_)
		{
			++LoadingEndCount;
		}
	}

	// ProgressBar Percent Update
	if (TotalPlayerCnt != LoadingEndCount)
	{
		float fLoadingEndCount = static_cast<float>(LoadingEndCount);
		float fTotalPlayerCnt = static_cast<float>(TotalPlayerCnt);
		LoadingPercent_ = fLoadingEndCount / fTotalPlayerCnt;
	}
	else
	{
		LoadingPercent_ = 1.0f;
		LoadingState_ = true;
	}

	LoadingBar_->SetPercent(LoadingPercent_);
}

void LoadingLevel_LoadPercent::UpdateExplanationImage()
{
	// 현재 표시되는 이미지 Off상태로 전환
	ExplanationImageList_[CurExplanationImageIndex_]->Off();

	// 다음 표시되는 이미지 On상태로 전환
	++CurExplanationImageIndex_;
	if (13 <= CurExplanationImageIndex_)
	{
		CurExplanationImageIndex_ = 0;
	}
	ExplanationImageList_[CurExplanationImageIndex_]->On();

	// Update Time Reset
	LoadingExplanationTime_ = 3.f;
}

void LoadingLevel_LoadPercent::Start()
{
	float4 WindowSize = GameEngineWindow::GetInst().GetSize();
	float4 WindowSizeHarf = WindowSize.halffloat4();

	// Create Loading Percent
	LoadingCircle_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UI));
	LoadingCircle_->SetImage("LeadingLevel_Circle.png");
	LoadingCircle_->GetTransform()->SetLocalScaling(float4(30.f, 30.f));
	LoadingCircle_->GetTransform()->SetLocalPosition(float4(WindowSizeHarf.x - 30.f, -WindowSizeHarf.y + 30.f));
	LoadingCircle_->SetAlpha(0.5f);

	// Create Loading ProgressBar
	LoadingBar_ = CreateTransformComponent<GameEngineProgressBarRenderer>(static_cast<int>(UIRenderOrder::UI));
	LoadingBar_->SetImage("LoadingLevel_ProgressBar.png");
	LoadingBar_->SetProgressBarDirect(static_cast<int>(ProgressBarDirect::RightToLeft));
	LoadingBar_->GetTransform()->SetLocalScaling(float4(WindowSize.x - 140.f, 4.f));
	LoadingBar_->GetTransform()->SetLocalPosition(float4(0.f, -WindowSizeHarf.y + 15.f));
	LoadingBar_->SetPercent(LoadingPercent_);

	// Create Loading Text
	LoadingText_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UI));
	LoadingText_->TextSetting("malgunbd", "다른 플레이어를 기다리는 중...", 12.f, FW1_CENTER | FW1_VCENTER, float4(1.0f, 1.0f, 1.0f, 0.7f));
	LoadingText_->GetTransform()->SetLocalPosition(float4(WindowSizeHarf.x - 150.f, -WindowSizeHarf.y + 30.f));

	// Create Loading Game Explanation Image List
	for (int Index = 0; Index < 13; ++Index)
	{
		std::string ImageName = "LoadingLevel_ExplanationImage_";
		ImageName += std::to_string(Index + 1);
		ImageName += ".png";

		GameEngineUIRenderer* NewExplanationImage = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UI));
		NewExplanationImage->SetImage(ImageName);
		NewExplanationImage->GetTransform()->SetLocalScaling(float4(280.0f, 200.f));
		NewExplanationImage->GetTransform()->SetLocalPosition(float4(0.0f, -WindowSizeHarf.y + 130.f));
		NewExplanationImage->Off();
		ExplanationImageList_.push_back(NewExplanationImage);
	}
	ExplanationImageList_[0]->On();
}

void LoadingLevel_LoadPercent::Update(float _DeltaTime)
{
	// Update LoadingCircle Rotation
	if (nullptr != LoadingCircle_ && false == LoadingState_)
	{
		LoadingCircle_->GetTransform()->AddLocalRotationDegreeZ(_DeltaTime * -30.f);
	}

	// Update Loading Guide Image
	LoadingExplanationTime_ -= _DeltaTime;
	if (0.0f >= LoadingExplanationTime_)
	{
		UpdateExplanationImage();
	}
}

LoadingLevel_LoadPercent::LoadingLevel_LoadPercent()
	: InGameConnectCount_(0)
	, LoadingState_(false)
	, LoadingPercent_(0.0f)
	, LoadingExplanationTime_(3.f)
	, CurExplanationImageIndex_(0)
	, LoadingCircle_(nullptr)
	, LoadingBar_(nullptr)
	, LoadingText_(nullptr)
{
	if (nullptr == LoadingLevel_LoadPercent::Percent)
	{
		LoadingLevel_LoadPercent::Percent = this;
	}
}

LoadingLevel_LoadPercent::~LoadingLevel_LoadPercent()
{
}
