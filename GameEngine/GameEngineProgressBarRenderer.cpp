#include "PreCompile.h"
#include "GameEngineProgressBarRenderer.h"
#include "GameEngineLevel.h"
#include "CameraComponent.h"
#include "GameEngineTransform.h"
#include "GameEngineTextureManager.h"
#include "GameEngineFolderTextureManager.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineWindow.h"
#include "GameEngineRenderTarget.h"

GameEngineProgressBarRenderer::GameEngineProgressBarRenderer() :
	ProgressBarData_{}
{
}

GameEngineProgressBarRenderer::~GameEngineProgressBarRenderer()
{
}

void GameEngineProgressBarRenderer::Start()
{
	GetLevel()->GetUICamera()->PushRenderer(GetOrder(), this);

	SetRenderingPipeLine("ProgressBarUI");
	ShaderHelper.SettingConstantBufferLink("ProgressBarCBuffer", ProgressBarData_);
}

void GameEngineProgressBarRenderer::Render(float _DeltaTime, bool _IsDeferred)
{
	if (true == _IsDeferred)
	{
		return;
	}

	GameEngineRenderer::Render(_DeltaTime, _IsDeferred);
}

void GameEngineProgressBarRenderer::SetRenderGroup(int _Order)
{
	GetLevel()->GetUICamera()->ChangeRendererGroup(_Order, this);
}

void GameEngineProgressBarRenderer::SetProgressBarDirect(int _ProgressBarDir)
{
	ProgressBarData_.PregressDirection = _ProgressBarDir;
}

void GameEngineProgressBarRenderer::SetPercent(float _Percent)
{
	ProgressBarData_.Percent = _Percent;
}
