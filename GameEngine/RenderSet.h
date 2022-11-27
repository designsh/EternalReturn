#pragma once

#include "GameEngineRendererBase.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

class RenderSet
{
public:
	GameEngineShaderResHelper* ShaderHelper;
	GameEngineRenderingPipeLine* PipeLine_;
	int Index;
	std::vector<float4x4> BoneData;

	float4 LocalPos;
	float Radius;
	bool isRender = true;
};