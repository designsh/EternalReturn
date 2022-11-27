#include "Precompile.h"
#include "LoadingLevel_BackDrop.h"

#include <GameEngine/GameEngineUIRenderer.h>

#include "Enums.h"

void LoadingLevel_BackDrop::Start()
{
	float4 WindowSize = GameEngineWindow::GetInst().GetSize();
	float4 WindowSizeHarf = WindowSize.halffloat4();

	// BackDrop
	BackDropRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::BACKDROP));
	BackDropRenderer_->SetImage("LoadingLevel_BackDrop.png");
	BackDropRenderer_->GetTransform()->SetWorldScaling(WindowSize);

	// Logo
	LogoRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UI));
	LogoRenderer_->SetImage("LoadingLevel_Logo.png");
	LogoRenderer_->GetTransform()->SetWorldScaling(float4(250.f, 100.f));
	LogoRenderer_->GetTransform()->SetWorldPosition(float4(-WindowSizeHarf.x + 135.f, WindowSizeHarf.y - 60.f));

	// Bottom Panel
	BottomPanelRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UIPANEL0));
	BottomPanelRenderer_->SetImage("LoadingLevel_Bottom_Panel.png");
	BottomPanelRenderer_->GetTransform()->SetWorldScaling(float4(WindowSize.x, 350.f));
	BottomPanelRenderer_->GetTransform()->SetWorldPosition(float4(0.0f, -WindowSizeHarf.y + 175.f));
}

void LoadingLevel_BackDrop::Update(float _DeltaTime)
{
}

LoadingLevel_BackDrop::LoadingLevel_BackDrop()
	: BackDropRenderer_(nullptr)
	, LogoRenderer_(nullptr)
	, BottomPanelRenderer_(nullptr)
{
}

LoadingLevel_BackDrop::~LoadingLevel_BackDrop()
{
}
