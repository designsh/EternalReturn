#pragma once
#include "GameEngineRendererBase.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

// �з� : ������
// �뵵 : 
// ���� : �ϳ��� ���������� ����
class CameraComponent;
class GameEngineLevel;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineRenderingPipeLine;
class GameEngineRenderer : public GameEngineRendererBase
{
	friend GameEngineLevel;

public: // Public Inline Get Function
	inline GameEngineRenderingPipeLine* GetGameEngineRenderingPipeLine()
	{
		return PipeLine_;
	}

public:
	void SetRenderingPipeLine(const std::string& _Value);
	virtual void SetRenderingPipeLineSettingNext();

public:
	void SetMesh(const std::string& _ShaderName);
	void SetMesh(const std::string& _VtxName, const std::string& _IdxName);
	void SetMesh(GameEngineVertexBuffer* _VtxBuffer, GameEngineIndexBuffer* _IdxBuffer);

	void Render(float _DeltaTime, bool _IsDeferred) override;

protected: // Protected Shadow Initalize Related Function
	void LightShadowInit(GameEngineRenderingPipeLine* _ShadowPipe) override;

private:
	void Update(float _DeltaTime) override;

public:
	GameEngineRenderer();
	~GameEngineRenderer();

protected:
	GameEngineRenderer(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _Other) noexcept = delete;

private:
	GameEngineRenderer& operator=(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _Other) noexcept = delete;

public:
	GameEngineShaderResHelper ShaderHelper;

private:
	GameEngineRenderingPipeLine* PipeLine_;
};

