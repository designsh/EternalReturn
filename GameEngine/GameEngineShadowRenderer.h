#pragma once
#include "GameEngineRenderer.h"

// 분류 : 렌더러
// 용도 : 그림자처리
// 설명 : 베이스렌더러를 회전시켜 그림자형태로 렌더링하는 렌더러
class GameEngineFBXRenderer;
class GameEngineShadowRenderer : public GameEngineRenderer
{
public:
	void SetBaseRenderer(GameEngineFBXRenderer* _BaseRenderer, std::string _PipeLineName);

protected:
private:
	void SetFBXMeshRender(std::string _PipeLineName, int _MeshIndex);

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime, bool _IsDeferred);

public:
	GameEngineShadowRenderer();
	~GameEngineShadowRenderer();

protected:
	GameEngineShadowRenderer(const GameEngineShadowRenderer& _Other) = delete;
	GameEngineShadowRenderer(GameEngineShadowRenderer&& _Other) noexcept = delete;

private:
	GameEngineShadowRenderer& operator=(const GameEngineShadowRenderer& _Other) = delete;
	GameEngineShadowRenderer& operator=(GameEngineShadowRenderer&& _Other) noexcept = delete;

public:
protected:
private:
	GameEngineFBXRenderer* BaseRenderer_;
	GameEngineFBXMesh* FBXMesh_;
	std::vector<RenderSet> RenderSets_;
};

