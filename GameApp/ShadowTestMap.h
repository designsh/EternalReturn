#pragma once
#include "LumiaMap.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineRenderer;
class ShadowTestMap : public LumiaMap
{
public:
protected:
private:
	void makeAStarNode(float _intervalX, float _intervalZ);

private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	ShadowTestMap();
	~ShadowTestMap();

protected:
	ShadowTestMap(const ShadowTestMap& _other) = delete;
	ShadowTestMap(ShadowTestMap&& _other) noexcept = delete;

private:
	ShadowTestMap& operator=(const ShadowTestMap& _other) = delete;
	ShadowTestMap& operator=(const ShadowTestMap&& _other) = delete;

public:
protected:
private:
	GameEngineRenderer* Renderer_;
};

