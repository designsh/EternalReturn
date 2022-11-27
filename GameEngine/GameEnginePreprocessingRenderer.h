#pragma once
#include "GameEngineRenderer.h"

// 분류 : 데이터
// 용도 : 상수버퍼 데이터셋팅
struct OutLineData
{
	float4 LineColor;
	float LineThickness;
};

// 분류 : 렌더러
// 용도 : 외곽선처리
// 설명 : 
class GameEngineFBXRenderer;
class GameEnginePreprocessingRenderer : public GameEngineRenderer
{
public: // Public Inline Get Function
	inline OutLineData GetOutLineData() const
	{
		return OutLineData_;
	}

public:
	RenderSet& GetRenderSet(unsigned int _Index);

public:
	void SetBaseRenderer(GameEngineFBXRenderer* _BaseRenderer, std::string _PipeLineName, bool _IsCharacter = false, bool _IsSilhouette = true);
	void SetOutLineData(const float4& _LineColor, float _LineThickness);

protected:
private:
	void SetMesh(std::string _PipeLineName, bool _IsCharacter);
	void SetFBXMeshRenderSet(std::string _PipeLineName, int _MeshIndex);
	void SetFBXMeshRenderSetCharacter(std::string _PipeLineName, int _MeshIndex);

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime, bool _IsDeferred) override;

public:
	GameEnginePreprocessingRenderer();
	~GameEnginePreprocessingRenderer();

protected:
	GameEnginePreprocessingRenderer(const GameEnginePreprocessingRenderer& _Other) = delete;
	GameEnginePreprocessingRenderer(GameEnginePreprocessingRenderer&& _Other) noexcept = delete;

private:
	GameEnginePreprocessingRenderer& operator=(const GameEnginePreprocessingRenderer& _Other) = delete;
	GameEnginePreprocessingRenderer& operator=(GameEnginePreprocessingRenderer&& _Other) noexcept = delete;

public:
protected:
private:
	OutLineData OutLineData_;

private:
	GameEngineFBXRenderer* BaseRenderer_;
	GameEngineFBXMesh* FBXMesh_;
	std::vector<RenderSet> RenderSets_;
};

