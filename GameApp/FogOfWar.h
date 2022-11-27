#pragma once

#include <GameEngine/GameEnginePostProcessRender.h>

struct FOWData
{
	float4 vertices[36];
};

class FogOfWar : public GameEnginePostProcessRender
{
public:
	FogOfWar(); 
	~FogOfWar();

	FogOfWar(const FogOfWar& _other) = delete; 
	FogOfWar(FogOfWar&& _other) = delete; 

	FogOfWar& operator=(const FogOfWar& _other) = delete;
	FogOfWar& operator=(const FogOfWar&& _other) = delete;

public:
	virtual void Initialize() override;
	virtual void Effect(float _DeltaTime) override;

public:
	void SetFilter(GameEngineRenderTarget* _renderTarget);
};

