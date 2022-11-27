#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineTransform.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "CameraComponent.h"

void GameEngineRenderer::SetRenderingPipeLine(const std::string& _Value)
{
	std::string UpperName = GameEngineString::toupper(_Value);

	ShaderHelper.Clear();

	GameEngineRenderingPipeLine* FindPipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find(UpperName);
	if (nullptr == FindPipeLine_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 랜더링 파이프라인입니다." + UpperName);
		return;
	}

	if (nullptr == PipeLine_)
	{
		PipeLine_ = FindPipeLine_->Clone();
	}
	else
	{
		PipeLine_->Copy(FindPipeLine_);
	}

	ShaderHelper.ShaderResourcesCheck(PipeLine_->GetPixelShader());
	ShaderHelper.ShaderResourcesCheck(PipeLine_->GetVertexShader());

	if (true == ShaderHelper.IsConstantBuffer("TransformData"))
	{
		ShaderHelper.SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
	}

	if (true == ShaderHelper.IsConstantBuffer("LightsData"))
	{
		LightsData& Data = GetLevel()->GetMainCamera()->LightData_;
		ShaderHelper.SettingConstantBufferLink("LightsData", Data);
	}

	if (true == ShaderHelper.IsConstantBuffer("RendererData"))
	{
		ShaderHelper.SettingConstantBufferLink("RendererData", RendererDataInst);
	}

	if (nullptr == PipeLine_)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 파이프라인 입니다!");
	}

	SetRenderingPipeLineSettingNext();
}

void GameEngineRenderer::SetRenderingPipeLineSettingNext()
{
}

void GameEngineRenderer::SetMesh(const std::string& _ShaderName)
{
	SetMesh(_ShaderName, _ShaderName);
}

void GameEngineRenderer::SetMesh(const std::string& _VtxName, const std::string& _IdxName)
{
	if (nullptr == PipeLine_)
	{
		GameEngineDebug::MsgBoxError("랜더링 파이프라인을 먼저 세팅해주세요." + _VtxName);
		return;
	}

	PipeLine_->SetInputAssembler1VertexBufferSetting(_VtxName);
	PipeLine_->SetInputAssembler2IndexBufferSetting(_IdxName);
}

void GameEngineRenderer::SetMesh(GameEngineVertexBuffer* _VtxBuffer, GameEngineIndexBuffer* _IdxBuffer)
{
	if (nullptr == PipeLine_)
	{
		GameEngineDebug::MsgBoxError("랜더링 파이프라인을 먼저 세팅해주세요." + _VtxBuffer->GetName());
		return;
	}

	PipeLine_->SetInputAssembler1VertexBufferSetting(_VtxBuffer);
	PipeLine_->SetInputAssembler2IndexBufferSetting(_IdxBuffer);
}

void GameEngineRenderer::Render(float _DeltaTime, bool _IsDeferred)
{
	if (true == _IsDeferred && true == PipeLine_->IsDeferred())
	{
		ShaderHelper.Setting();
		PipeLine_->Rendering();
		ShaderHelper.ReSet();
		PipeLine_->Reset();
	}
	else if (false == _IsDeferred && false == PipeLine_->IsDeferred())
	{
		ShaderHelper.Setting();
		PipeLine_->Rendering();
		ShaderHelper.ReSet();
		PipeLine_->Reset();
	}
}

void GameEngineRenderer::LightShadowInit(GameEngineRenderingPipeLine* _ShadowPipe)
{
	_ShadowPipe->SetInputAssembler1VertexBufferSetting(PipeLine_->GetVertexBuffer());
	_ShadowPipe->SetInputAssembler2IndexBufferSetting(PipeLine_->GetIndexBuffer());
}

void GameEngineRenderer::Update(float _DeltaTime)
{
}

GameEngineRenderer::GameEngineRenderer() 
	: PipeLine_(nullptr)
{
}

GameEngineRenderer::~GameEngineRenderer() 
{
	if (nullptr != PipeLine_)
	{
		delete PipeLine_;
		PipeLine_ = nullptr;
	}
}
