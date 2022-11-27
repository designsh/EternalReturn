#include "PreCompile.h"
#include "GameEngineRendererBase.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineTransform.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "CameraComponent.h"

void GameEngineRendererBase::LightShadowOff()
{
	RendererDataInst.IsLightShadow = 0;
}

void GameEngineRendererBase::LightShadowOn()
{
	// 그림자처리 렌더링 파이프라인이 지정되어있지않다면 지정 후
	if (nullptr == DefaultLightShadowPipeLine_)
	{
		DefaultLightShadowPipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("LightShadow");		// Find DepthShadow PipeLine
		DefaultLightShadowPipeLine_->CreateLayOut();															// Create InputLayout

		LightShadowHelper_.ShaderResourcesCheck(DefaultLightShadowPipeLine_);										// DefaultShadowPipeLine_의 상수버퍼를 체크			: 동일한 명칭 상수버퍼 체크, 쉐이더헬퍼에 상수버퍼와 동일한 크기의 메모리할당
		
		// 상수버퍼 링크
		if (true == LightShadowHelper_.IsConstantBuffer("RendererData"))
		{
			LightShadowHelper_.SettingConstantBufferLink("RendererData", RendererDataInst);
		}

		if (true == LightShadowHelper_.IsConstantBuffer("TransformData"))
		{
			LightShadowHelper_.SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
		}

		if (true == LightShadowHelper_.IsConstantBuffer("LightsData"))
		{
			const LightsData& AllLightData = GetLevel()->GetMainCamera()->GetLightData();
			LightShadowHelper_.SettingConstantBufferLink("LightsData", AllLightData);
		}

		DefaultLightShadowPipeLine_->SetViewPort(4096, 4096, 0, 0);
	}

	// 그림자사용 Flag On
	RendererDataInst.IsLightShadow = 1;
}

void GameEngineRendererBase::Start()
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);
}

void GameEngineRendererBase::SetRenderGroup(int _Order)
{
	GetLevel()->GetMainCamera()->ChangeRendererGroup(_Order, this);
}

void GameEngineRendererBase::Render(float _DeltaTime, bool _IsDeferred)
{
}

void GameEngineRendererBase::LightShadowInit(GameEngineRenderingPipeLine* _ShadowPipe)
{
	// 하위에서 받아서 처리할꺼임
}

void GameEngineRendererBase::LightShadowRender(float _DeltaTime)
{
	if (nullptr == DefaultLightShadowPipeLine_)
	{
		return;
	}

	LightShadowInit(DefaultLightShadowPipeLine_);
	LightShadowHelper_.Setting();
	DefaultLightShadowPipeLine_->Rendering();
}

GameEngineRendererBase::GameEngineRendererBase()
	: IsPreprocessing_(false)
	, IsRendererShadow_(false)
	, DefaultLightShadowPipeLine_(nullptr)
	, LightShadowHelper_()
{
}

GameEngineRendererBase::~GameEngineRendererBase()
{
}
