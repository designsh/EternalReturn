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
	// �׸���ó�� ������ ������������ �����Ǿ������ʴٸ� ���� ��
	if (nullptr == DefaultLightShadowPipeLine_)
	{
		DefaultLightShadowPipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("LightShadow");		// Find DepthShadow PipeLine
		DefaultLightShadowPipeLine_->CreateLayOut();															// Create InputLayout

		LightShadowHelper_.ShaderResourcesCheck(DefaultLightShadowPipeLine_);										// DefaultShadowPipeLine_�� ������۸� üũ			: ������ ��Ī ������� üũ, ���̴����ۿ� ������ۿ� ������ ũ���� �޸��Ҵ�
		
		// ������� ��ũ
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

	// �׸��ڻ�� Flag On
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
	// �������� �޾Ƽ� ó���Ҳ���
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
